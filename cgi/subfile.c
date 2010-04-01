#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "subfile.h"
#include "lang.h"
#include <unistd.h>

#include <libgen.h>
#include <glob.h>

#define MAX_CMD		18
#define MAX_FN_LEN	512
#define MAX_LINE_BUFF	4096

void _subfile( char *, struct subfile_list *, int );
int _do_macro_include ( char *, struct subfile_list * );
void do_macro_include ( char *, struct subfile_list * );
char * str_replace(char *, char *, char *);
char * macro_process ( char *, struct subfile_list *, char *);

/*
 * Warning: if the first two arguments are the same, and rep is longer than
 * orig, it will fail.
 */
    char *
str_replace(char *str, char *orig, char *rep)
{
    static char buffer[MAX_LINE_BUFF+1];
    char *p;

    if(!(p = strstr(str, orig)))
	return str;

    strncpy(buffer, str, p-str);
    buffer[p-str] = '\0';

    sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));

    return buffer;
}

    char *
macro_process ( char *str, struct subfile_list *sl, char *filename )
{
    char	cmd[MAX_CMD+1];
    char	arg[MAX_LINE_BUFF+1];
    char	*p, *orig, *text;
    int		ch;
    char	*pbuff = NULL;
    char	*pdir;
    char	incfile[MAX_FN_LEN+1];

    //dirname change orignal buffer, so strdup before dirname.
    pbuff = strdup(filename);
    pdir = dirname (pbuff);

    p = orig = text = str;
    while ( *p && *p != '(' )
	p++;

    if (!*p || *p != '(' || p-text >= MAX_CMD)
	return orig;

    strncpy(cmd, text, p-text);
    cmd[p-text] = '\0';

    text = ++p;
    while ( *p && isspace(*p) ) {
	p++;
    }
    text = p;
    ch = *p;

    if (ch == '"' || ch == '\'') {
	do {
	    if (*p == '\\' && *(p+1))
		p++;
	    p++;
	} while (*p && *p != ch);
    }

    while ( *p && *p != ')' ) {
	p++;
    }

    if (!*p || *p != ')' || p-text >= MAX_LINE_BUFF)
	return orig;
    else
	orig = p+1;

    do {
	--p;
    } while ( isspace(*p) && p!=text );

    if ( (ch == '"' || ch == '\'') && *p == ch ) {
	text++;
	p--;
	strncpy(arg, text, p-text+1);
	arg[p-text+1] = '\0';
	p = str_replace(arg, "\\'", "'");
	p = str_replace(p, "\\\"", "\"");
    } else {
	strncpy(arg, text, p-text+1);
	arg[p-text+1] = '\0';
	p = arg;
    }

    if ( strcasecmp(cmd, "include") == 0 ) {
	snprintf(incfile, MAX_FN_LEN, "%s/%s", pdir, p);
	do_macro_include(incfile, sl);
    } else {
	orig = str;
    }

    if (pbuff != NULL)
        free(pbuff);

    return orig;
}

/*
 * Macro for include other files.
 * Usage: $!include(filename)
 */
    void
do_macro_include ( char *incfile, struct subfile_list *sl )
{
    char	**lang;
    char	*newfile;

    if (strstr(incfile, "%lang%") != NULL) {
	lang = get_accept_language();

	// substitue %lang% to zh_CN, zh_TW, ..., en
	while(*lang !=NULL)
	{
	    newfile = str_replace(incfile, "%lang%", *lang);
	    if (_do_macro_include ( newfile, sl ) == 0 )
	    {
		return;
	    }
	    if (strncmp(*lang, "zh_", 3)==0)
	    {
		newfile = str_replace(incfile, "%lang%", "zh");
		if (_do_macro_include ( newfile, sl ) == 0 )
		    return;
	    }
	    else if (strncmp(*lang, "en_", 3)==0)
	    {
		newfile = str_replace(incfile, "%lang%", "en");
		if (_do_macro_include ( newfile, sl ) == 0 )
		    return;
	    }
	    lang++;
	}

	// fallback to en
	newfile = str_replace(incfile, "%lang%", "en");
	if (_do_macro_include ( newfile, sl ) == 0 )
	    return;
	
	// remove %lang%/, or %lang%
	newfile = (strstr(incfile, "%lang%/") != NULL) ? 
	    str_replace(incfile, "%lang%/", "") : str_replace(incfile, "%lang%", "");
	if (_do_macro_include ( newfile, sl ) == 0 )
	    return;
    } else {
	_do_macro_include ( incfile, sl );
    }
    return;
}


    int
_do_macro_include ( char *incfile, struct subfile_list *sl )
{
    int i, ret = 1;

    if ( access(incfile, F_OK) == 0 ) {
	_subfile( incfile, sl, -1);
	ret = 0;
    }
    else
    {
	glob_t globfiles;
	if ( glob(incfile, 0, NULL, &globfiles) == 0)
	{
	    for (i=0; i< globfiles.gl_pathc; i++)
	    {
		if ( access(globfiles.gl_pathv[i], F_OK) == 0 )
		{
		    _subfile( globfiles.gl_pathv[i], sl, -1);
		    ret = 0;
		}
	    }
	    globfree(&globfiles);
	}
    }
    return ret;
}

/*
 * Substitute template file with multiple language support.
 * Add language code prefix before template file, and
 * call real _subfile procedure.
 */
    void
subfile( char *filename, struct subfile_list *sl, int nocache )
{
    char	**lang;
    char	newfile[MAX_FN_LEN + 1];

    lang = get_accept_language();
    while(*lang !=NULL)
    {
        snprintf(newfile, MAX_FN_LEN, "%s/%s", *lang, filename);
        if (access(newfile, F_OK)==0)
        {
            filename = newfile;
            break;
        }
	if (strncmp(*lang, "zh_", 3)==0)
	{
	    strcpy(*lang, "zh");
	    continue;
	}
	else if (strncmp(*lang, "en_", 3)==0)
	{
	    strcpy(*lang, "en");
	    continue;
	}
        lang++;
    }

    // fallback to en
    if (access(filename, F_OK)!=0)
    {
        snprintf(newfile, MAX_FN_LEN, "%s/%s", "en", filename);
        if (access(newfile, F_OK)==0)
        {
            filename = newfile;
        }
    }

    _subfile( filename, sl, nocache );

    return;
}

    
/*
 * Real subfile procedure. Do macro substitute.
 */
    void
_subfile( char *filename, struct subfile_list *sl, int nocache )
{
    FILE	*fs;
    int 	c, i, j;
    char	nasties[] = "<>(){}[]'`\" \\";

    if ( nocache > 0 ) {
	fputs( "Expires: Mon, 16 Apr 1973 13:10:00 GMT\n"
		"Last-Modified: Mon, 16 Apr 1973 13:10:00 GMT\n"
		"Cache-Control: no-store, no-cache, must-revalidate\n"
		"Cache-Control: pre-check=0, post-check=0, max-age=0\n"
		"Pragma: no-cache\n", stdout );
    }

    if ( nocache >= 0 ) {
	fputs( "Content-type: text/html\n\n", stdout );
    }

    if (( fs = fopen( filename, "r" )) == NULL ) {
	perror( filename );
	exit( 1 );
    }

    while (( c = getc( fs )) != EOF ) {
	if ( c == '$' ) {
	    if (( c = getc( fs )) == EOF ) {
		putchar( '$' );
		break;
	    }

	    if ( c == '$' ) {
		putchar( c );
		continue;
	    }
	    if ( c == '!' ) {
		// begin macro process
		char *s = malloc(MAX_CMD+MAX_LINE_BUFF+1);
		char *p;
		fgets(s, MAX_CMD+MAX_LINE_BUFF, fs);
		p = macro_process(s, sl, filename);
		if (p==s) {
		    putchar( '$' );
		    putchar( c );
		    fseek(fs, -1*strlen(p), SEEK_CUR);
		} else {
		    fseek(fs, -1*strlen(p), SEEK_CUR);
		}
		if (s != NULL)
		    free(s);
		s = NULL;
		continue;
	    }

	    for ( i = 0; sl[ i ].sl_letter != '\0'; i++ ) {
		if ( sl[ i ].sl_letter == c ) {
		    break;
		}
	    }
	    if ( sl[ i ].sl_letter == '\0' ) {
		putchar( '$' );
		putchar( c );
	    } else if ( sl[ i ].sl_data != NULL ) {
		if ( sl[ i ].sl_type == SUBF_STR ) {
		    printf( "%s", sl[ i ].sl_data );
		} else if ( sl[ i ].sl_type == SUBF_STR_ESC ) {

		    /* block XSS attacks while printing */
                    for ( j = 0; j < strlen( sl[ i ].sl_data ); j++ ) {
                        if ( strchr( nasties, sl[ i ].sl_data[ j ] ) != NULL ||
                                sl[ i ].sl_data[ j ] <= 0x1F ||
				sl[ i ].sl_data[ j ] >= 0x7F ) {

			    printf( "%%%x", sl[ i ].sl_data[ j ] );
                        } else {
                            putc( sl[ i ].sl_data[ j ], stdout );
                        }
		    }
		}
	    }
	} else {
	    putchar( c );
	}
    }

    if ( fclose( fs ) != 0 ) {
	perror( filename );
    }

    return;
}

/* vim: set noet ts=8 sw=4 : */
