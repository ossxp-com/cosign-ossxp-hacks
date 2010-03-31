/*
 * Copyright (c) 2002 Regents of The University of Michigan.
 * All Rights Reserved.  See LICENSE.
 */

#include "config.h"

#include <sys/types.h>
#include <sys/param.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <snet.h>

#include "cgi.h"
#include "conf.h"
#include "cosigncgi.h"
#include "network.h"
#include "subfile.h"

extern char	*cosign_version;
char		*tmpldir = _COSIGN_TMPL_DIR;
char		*cosign_conf = _COSIGN_CONF;

struct cgi_list cl[] = {
#define CL_VERIFY	0
        { "verify", CGI_TYPE_STRING, NULL },
#define CL_URL 		1
        { "url", CGI_TYPE_STRING, NULL },
        { NULL, CGI_TYPE_UNDEF, NULL },
};

static struct subfile_list sl[] = {
#define SL_URL	0
        { 'u', SUBF_STR, NULL },
#define SL_TITLE	1
        { 't', SUBF_STR, NULL },
#define SL_ERROR		2
        { 'r', SUBF_STR_ESC, NULL },
        { '\0', 0, NULL },
};


    int
main( int argc, char *argv[] )
{
    CGIHANDLE		*cgi;
    char		*tmpl = ERROR_HTML;
    char		*ip_addr, *qs;
    char		*method = NULL;
    char		*script;

    if ( argc == 2 && ( strncmp( argv[ 1 ], "-V", 2 ) == 0 )) {
	printf( "%s\n", cosign_version );
	exit( 0 );
    }

    if ( cosign_config( cosign_conf ) < 0 ) {
	fprintf( stderr, "Couldn't read %s\n", cosign_conf );
	exit( 1 );
    }
    if ( chdir( tmpldir ) < 0 ) {
	perror( tmpldir );
	exit( 1 );
    }

    if (( ip_addr = getenv( "REMOTE_ADDR" )) == NULL ) {
	sl[ SL_TITLE ].sl_data = "Error: Server Error";
	sl[ SL_ERROR ].sl_data = "REMOTE_ADDR not set";
	tmpl = ERROR_HTML;
	subfile( tmpl, sl, 0 );
	exit( 0 );
    }

    if (( script = getenv( "SCRIPT_NAME" )) == NULL ) {
	sl[ SL_TITLE ].sl_data = "Error: Server Error";
	sl[ SL_ERROR ].sl_data = "SCRIPT_NAME not set";
	tmpl = ERROR_HTML;
	subfile( tmpl, sl, 0 );
	exit( 0 );
    }

    if (( method = getenv( "REQUEST_METHOD" )) == NULL ) {
	sl[ SL_TITLE ].sl_data = "Error: Server Error";
	sl[ SL_ERROR ].sl_data = "REQUEST_METHOD not set";
	tmpl = ERROR_HTML;
	subfile( tmpl, sl, 0 );
	exit( 0 );
    }

    if ( strcmp( method, "GET" ) == 0 ) {
	/* this is not a POST, display verify screen */
	if ((( qs = getenv( "QUERY_STRING" )) != NULL ) && ( *qs != '\0' ))
	{
	    fprintf( stderr, ">>> qs: %s", qs);
	}
	tmpl = ERROR_HTML;
	sl[ SL_TITLE ].sl_data = "Info";
	sl[ SL_ERROR ].sl_data = qs;
	subfile ( tmpl, sl, 0 );
	exit( 0 );
    }

    if (( cgi = cgi_init()) == NULL ){
	sl[ SL_TITLE ].sl_data = "Error: Server Error";
	sl[ SL_ERROR ].sl_data = "cgi_init failed";
	tmpl = ERROR_HTML;
	subfile( tmpl, sl, 0 );
	exit( 0 );
    }

    /* clobber the cosign cookie */
    fputs( "Expires: Mon, 16 Apr 1973 13:10:00 GMT\n"
	    "Last-Modified: Mon, 16 Apr 1973 13:10:00 GMT\n"
	    "Cache-Control: no-store, no-cache, must-revalidate\n"
	    "Cache-Control: pre-check=0, post-check=0, max-age=0\n"
	    "Pragma: no-cache\n", stdout );

    printf( "Location: %s\n\n", sl[ SL_URL ].sl_data );
    exit( 0 );
}

// vim: noet ts=8 sw=4
