/*
 * Copyright (c) 2008 JiangXin, worldhello.net AT gmail.com
 * All Rights Reserved.  See LICENSE.
 */

#ifndef _LANG_H
#define _LANG_H

#include <locale.h>
#include <libintl.h>
#define _(String) gettext (String)
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)

char ** get_accept_language();
char ** _get_accept_language( char * );
void init_locale();
void _init_locale( char * );

#endif
