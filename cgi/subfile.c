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

#include <netinet/in.h>
#include <openssl/ssl.h>
#include <snet.h>
#include "cosigncgi.h"

#define MAX_CMD		18
#define MAX_FN_LEN	512
#define MAX_LINE_BUFF	4096

void _subfile( char *, struct subfile_list *, int );
int _do_macro_include ( char *, struct subfile_list * );
void do_macro_include ( char *, struct subfile_list * );
char * str_replace(char *, char *, char *);
size_t macro_process ( char *, size_t, struct subfile_list *, char *);

#ifdef	_DEBUG

void stub_do_macro_include ( char *, struct subfile_list * );

#endif


    char *
str_replace(char *orig, char *seek, char *rep)
{
    static char buffer[MAX_LINE_BUFF+1];
    char *p, *phead;
    char *str = strdup(orig);

    phead = str;
    *buffer = '\0';
    while (strstr(str, seek))  {
	p = strstr(str, seek);
	int i = strlen(buffer);
	strncpy(buffer+i, str, p-str);
	buffer[i+p-str] = '\0';
	strcat(buffer, rep);
	str = p + strlen(seek);
    }
    strcat(buffer, str);
    if (phead != NULL)
	free (phead);
    return buffer;
}

/*
 * return chars be eatten
 */
   size_t
macro_process ( char *str, size_t size, struct subfile_list *sl, char *filename )
{
    char	cmd[MAX_CMD+1];
    char	arg[MAX_LINE_BUFF+1]="";
    char	*p, *pa, *pz;
    char	*pbuff = NULL;
    char	*pdir;
    char	incfile[MAX_FN_LEN+1];

    //dirname change orignal buffer, so strdup before dirname.
    pbuff = strdup(filename);
    pdir = dirname (pbuff);

    p = pa = str;
    while ( p - str <= size && *p != '(' )
	p++;

    if (p - str > size || *p != '(' || p-pa >= MAX_CMD)
	return 0;

    strncpy(cmd, pa, p-pa);
    cmd[p-pa] = '\0';

    p++;

    // while not ')'
    while (p - str <= size && *p != ')') {
	if (*p == '"' || *p == '\'') {
	    // mark begin
	    pa = p;

	    do {
		if (*p == '\\' && p - str <= size)
		    p++;
		p++;
	    } while (p - str <= size && *p != *pa);

	    // mark end
	    pz = p;

	    // strcat from pa to pz
	    if ( *pa == *pz && pz > pa +1 ) {
		pa++;
		pz--;
		arg[strlen(arg)+pz-pa+1] = '\0';
		strncat(arg, pa, pz-pa+1);
		strcpy(arg, str_replace(arg, "\\'", "'"));
		strcpy(arg, str_replace(arg, "\\\"", "\""));
	    }

	    // *p is ' or "
	    if (p - str <= size)
		p++;
	} else {
	    // lstrip
	    while ( p - str <= size && isspace(*p) )
		p++;

	    // mark begin
	    pa = p;

	    while ( p - str <= size && *p != '"' && *p != '\'' && *p != ')') {
		p++;
	    }

	    // prematurate
	    if (p - str > size)
		break;

	    // mark end
	    pz = p-1;

	    // rstrip
	    while (isspace(*pz) && pz > pa ) {
		pz--;
	    }

	    // strcat from pa to pz
	    if ( pz > pa ) {
		arg[strlen(arg)+pz-pa+1] = '\0';
		strncat(arg, pa, pz-pa+1);
	    }
	}
    }

    if (*p != ')') {
	return 0;
    } else {
	p++;
    }

    if ( strcasecmp(cmd, "include") == 0 ) {
	snprintf(incfile, MAX_FN_LEN, "%s/%s", pdir, arg);
#ifndef _DEBUG
	do_macro_include(incfile, sl);
#else
	stub_do_macro_include(incfile, sl);
#endif
    } else {
	p = str;
    }

    if (pbuff != NULL)
        free(pbuff);

    return p-str;
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
    size_t	size, size_p;

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

    if (( fs = fopen( filename, "rb" )) == NULL ) {
	perror( filename );
	exit( 1 );
    }

    while (( c = getc( fs )) != EOF && !feof(fs)) {
	if ( c == '$' ) {
	    if (( c = getc( fs )) == EOF  && !feof(fs)) {
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
		//fgets(s, MAX_CMD+MAX_LINE_BUFF, fs);
		size = fread(s, 1, MAX_CMD+MAX_LINE_BUFF, fs);
		size_p = macro_process(s, size, sl, filename);
		if (size_p==0) {
		    putchar( '$' );
		    putchar( c );
		    fseek(fs, -1* size, SEEK_CUR);
		} else {
		    fseek(fs, -1* (size - size_p), SEEK_CUR);
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

#ifdef _DEBUG

    void
stub_do_macro_include( char *str, struct subfile_list *sl )
{
    printf(">> include: %s\n", str);
}


    void
test_macro_process()
{
    size_t size;
    char *buff[] = {
	"include(file).",
	"include(\"file\").",
	"_(\"world\").",
	"_(\"world.\")",
	"_(\"hello,\" \n \" world.\") end of text.",
	"_(\"hello\\\",\\\"\" \n \" world.\" \" baby\") end of paragraph1.",
	"hello, world.",
	"_(\"Please enter your authentication information and click the &quot;Log&nbsp;In&quot; button\"\n \t\t\t\t\t\" to continue.\")</p>",
	};
    int num, i;

    num = sizeof(buff)/sizeof(*buff);
    for (i=0; i < num; i++)
    {
	printf("====================\n");
	printf("buff: %s\n", buff[i]);
	size = macro_process ( buff[i], strlen(buff[i]), NULL, "");
	printf("eatten: %ld\n", size);
    }
    return;
}

    void
test_subfile()
{
    subfile("/opt/cosign/lib/templates/inc/help_infobox.inc", NULL, 0);
}

    void
test_str_replace()
{
    char *str, *seek, *rep;

    str = "hello, world."; seek = "e"; rep = "E";
    printf("replace %s in %s with %s>> \"%s\"\n", seek, str, rep, str_replace(str, seek, rep));

    str = "hello, world."; seek = "o"; rep = "O";
    printf("replace %s in %s with %s>> \"%s\"\n", seek, str, rep, str_replace(str, seek, rep));

    str = "hheelllloo, wwoorrlldd."; seek = "ll"; rep = "XX";
    printf("replace %s in %s with %s>> \"%s\"\n", seek, str, rep, str_replace(str, seek, rep));

    str = "hello, world."; seek = "o"; rep = "OO";
    str = str_replace(str, seek, rep);
    str = str_replace(str, seek, rep);
    printf("replacement: %s\n", str);
}

    int
main()
{
    //test_str_replace();
    //test_macro_process();
    //test_subfile();

    return 0;
}

#endif

/* vim: set noet ts=8 sw=4 : */
