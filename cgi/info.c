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
#include "subfile.h"
#include "lang.h"

#include <locale.h>
#include <libintl.h>
#define _(String) gettext (String)
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)

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
#define SL_LOGIN	0
        { 'l', SUBF_STR_ESC, NULL },
#define SL_TITLE	1
        { 't', SUBF_STR, NULL },
#define SL_ERROR	2
        { 'e', SUBF_STR, NULL },
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
    char		**lang;

    bindtextdomain("cosign", _LOCALEDIR);
    textdomain("cosign");
    lang = get_accept_language();
    while(*lang !=NULL)
    {
	if (strcmp(*lang, "zh")==0 || strcmp(*lang, "zh_CN")==0) {
	    setlocale( LC_ALL, "zh_CN.UTF-8");
	    break;
	} else if (setlocale( LC_ALL, *lang) != NULL) {
	    break;
	}
    }

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
	sl[ SL_TITLE ].sl_data = _("Error: Server Error");
	sl[ SL_ERROR ].sl_data = _("REMOTE_ADDR not set");
	tmpl = ERROR_HTML;
	subfile( tmpl, sl, 0 );
	exit( 0 );
    }

    if (( script = getenv( "SCRIPT_NAME" )) == NULL ) {
	sl[ SL_TITLE ].sl_data = _("Error: Server Error");
	sl[ SL_ERROR ].sl_data = _("SCRIPT_NAME not set");
	tmpl = ERROR_HTML;
	subfile( tmpl, sl, 0 );
	exit( 0 );
    }

    if (( method = getenv( "REQUEST_METHOD" )) == NULL ) {
	sl[ SL_TITLE ].sl_data = _("Error: Server Error");
	sl[ SL_ERROR ].sl_data = _("REQUEST_METHOD not set");
	tmpl = ERROR_HTML;
	subfile( tmpl, sl, 0 );
	exit( 0 );
    }

    if ( strcmp( method, "GET" ) == 0 ) {
	/* this is not a POST, display verify screen */
	if ((( qs = getenv( "QUERY_STRING" )) == NULL ) || ( *qs == '\0' )) {
	    qs = getenv( "PATH_INFO" );
	    while ( qs != NULL && ( *qs == '/' )) {
		qs++;
	    }
	}
	if ( qs != NULL && ( *qs != '\0' )) {
	    if (strncasecmp(qs, "looping", 4)==0) {
		tmpl = LOOPING_HTML;
	    } else if (strncasecmp(qs, "post_error", 4)==0) {
		tmpl = POST_ERROR_HTML;
	    } else if (strncasecmp(qs, "validation_error", 5)==0) {
		tmpl = VALIDATION_ERROR_HTML;
	    } else {
		sl[ SL_TITLE ].sl_data = _("Error: Server Error");
		sl[ SL_ERROR ].sl_data = _("Unable to determine template from query string.");
		tmpl = ERROR_HTML;
	    }
	    subfile ( tmpl, sl, 0 );
	    exit( 0 );
	}
    }

    tmpl = LOGIN_HTML;
    sl[ SL_TITLE ].sl_data = _("Authentication Required");
    subfile( tmpl, sl, 0 );
    exit( 0 );
}

// vim: noet ts=8 sw=4
