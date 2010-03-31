#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "subfile.h"
#include "lang.h"
#include <unistd.h>

#include <libgen.h>
#include <glob.h>


/*
 * Macro for include other files.
 * Usage: $!include(filename)
 */
    void
macro_include ( char *incfile, struct subfile_list *sl )
{
	int i;

	if ( access(incfile, F_OK) == 0 ) {
		_subfile( incfile, sl, -1);
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
				}
			}
			globfree(&globfiles);
		}
	}
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
    char	*filename_i18n;

    int tmplen = 0;

    tmplen = strlen(filename)+5;
    filename_i18n = malloc(tmplen+1);
    lang = get_accept_language();
    while(*lang !=NULL)
    {
        if (strlen(*lang)+1+strlen(filename) > tmplen)
        {
            tmplen +=5;
            filename_i18n=realloc(filename_i18n,tmplen+1);
        }
        snprintf(filename_i18n, tmplen, "%s/%s", *lang, filename);
        if (access(filename_i18n, F_OK)==0)
        {
            filename = filename_i18n;
            break;
        }
        if (strcmp(*lang, "zh_TW")==0)
        {
            strcpy(*lang, "zh");
            continue;
        }
        lang++;
    }
    if (access(filename, F_OK)!=0)
    {
        snprintf(filename_i18n, tmplen, "%s/%s", "en", filename);
        if (access(filename_i18n, F_OK)==0)
        {
            filename = filename_i18n;
        }
    }

    _subfile( filename, sl, nocache );

    if (filename_i18n != NULL)
        free(filename_i18n);

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

    char	*pbuff = NULL;
    char	*pdir;

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

    pbuff = strdup(filename);
    pdir = dirname (pbuff);

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
		char *s = malloc(9);
		char *incfile = malloc(255);
		fgets(s, 9, fs);
		if (strncmp(s, "include(", 8) == 0)
		{
			strcpy(incfile, pdir);
			strcat(incfile, "/");
			i = strlen(incfile)-1;
    			while (( c = getc( fs )) != EOF && i++ < 254 ) {
				if (c==')')
					break;
				incfile[i] = c;
			}
			incfile[i] = '\0';
			if (c==')')
			{
				macro_include(incfile, sl);
			}
			else
			{
				printf ("include(%s", incfile+strlen(pdir)+1);
				if (c != EOF)
					putchar( c );
			}
		}
		else
		{
			putchar( '$' );
			putchar( c );
			fseek(fs, -1*strlen(s), SEEK_CUR);
		}
		if (s != NULL)
			free(s);
		if (incfile != NULL)
			free(incfile);
		s = NULL;
		incfile = NULL;
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

    if (pbuff != NULL)
        free(pbuff);
    return;
}

/* vim: set noet ts=8 sw=8 : */
