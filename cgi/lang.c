#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "lang.h"

#ifdef _DEBUG
    #define getenv mygetenv
    char *mygetenv( char *);
#endif

    char **
get_accept_language()
{
#ifdef _DEBUG
    #define _ACCEPT_LANGS_ARRAY 2
    char **accept_languages = NULL;
#else
    #define _ACCEPT_LANGS_ARRAY 8
    static char **accept_languages = NULL;
#endif

    if (accept_languages != NULL)
	return accept_languages;

    return accept_languages = _get_accept_language(NULL);
}

    char **
_get_accept_language( char *env )
{
    char *p, *pnext, *buff;
    char **lang;
    int found = 0;
    int i,j,k,m;
    int array_size = _ACCEPT_LANGS_ARRAY;

    lang = calloc(sizeof(char*),array_size+1);
    for (i=0; i<array_size; i++)
	lang[i]=NULL;

    if (env == NULL)
	p = getenv( "HTTP_ACCEPT_LANGUAGE" );
    else
	p = env;

    if (p != NULL)
    {
	while(1)
	{
	    int i=0;

	    while (isspace(*p)) p++;

	    while (1)
	    {
		char c = *(p+i);
		if ( c=='\0' || c== ';' || c==',')
		    break;
		i++;
	    }
	    if (i>0)
	    {
		buff = strndup(p,i);
		while (--i >= 0 && isspace(buff[i])) buff[i] = '\0';
	    }
	    else
	    {
		buff = NULL;
	    }

	    if (buff!=NULL && strlen(buff)>0)
	    {
		for (j=0,k=0; j < strlen(buff); j++)
		{
		    if ( *(buff+j) == '-' || *(buff+j) == '_')
		    {
			*(buff+j) = '_';
			k = 1;
			continue;
		    }
		    if (k==0)
			*(buff+j) = tolower(*(buff+j));
		    else
			*(buff+j) = toupper(*(buff+j));

		}

		if (strcmp(buff, "zh_SG")==0)
		{
		    free(buff);
		    buff = strdup("zh_CN");
		}
		else if (strcmp(buff, "zh_HK")==0)
		{
		    free(buff);
		    buff = strdup("zh_TW");
		}
		found = 0;
		for (m=0; m< array_size; m++)
		{
		    if (lang[m]!=NULL && buff !=NULL && strcmp(lang[m],buff)==0)
		    {
			found = 1;
			break;
		    }
		    else if (lang[m] == NULL)
		    {
			break;
		    }
		}
		if (!found)
		{
		    if (m==array_size)
		    {
			array_size = array_size * 2 + 1;
			lang = realloc(lang, array_size * sizeof(char *) + 1);
		    }
		    lang[m] = buff;
		    lang[m+1] = NULL;
		}
	    }

	    pnext = strchr(p, ',');
	    if (pnext)
		p = pnext+1;
	    else
		break;
	}
    }

    return lang;
}

    void
init_locale()
{
    return _init_locale(NULL);
}

    void
_init_locale( char *env )
{
#define MAX_LANG_CODE	20
    char	**lang;
    char	*p;
    char	lang_utf8[MAX_LANG_CODE+7] = "";
    int		found = 0;

    bindtextdomain("cosign", _LOCALEDIR);
    bindtextdomain("template",  _TEMPLATE_LOCALEDIR);
    textdomain("cosign");
    if (env == NULL)
	lang = get_accept_language();
    else
	lang = _get_accept_language(env);
    while(*lang !=NULL)
    {
	p = strchr(*lang, '.');
	lang_utf8[0] = '\0';
	if (p!=NULL && p-*lang < MAX_LANG_CODE) {
	    strncpy(lang_utf8, *lang, p-*lang);
	    lang_utf8[p-*lang] = '\0';
	    strcat(lang_utf8, ".UTF-8");
	} else if (strlen(*lang) < MAX_LANG_CODE) {
	    strcpy(lang_utf8, *lang);
	    strcat(lang_utf8, ".UTF-8");
	}

	if (lang_utf8[0] != '\0' && setlocale( LC_ALL, lang_utf8) != NULL) {
	    found = 1;
	} else if (strcmp(*lang, "zh")==0) {
	    if (setlocale( LC_ALL, "zh_CN.UTF-8") != NULL)
		found = 1;
	    else if (setlocale( LC_ALL, "zh_CN") != NULL)
		found = 1;
	    else if (setlocale( LC_ALL, *lang) != NULL)
		found = 1;
	} else if (setlocale( LC_ALL, *lang) != NULL) {
	    found = 1;
	} else if (strncmp(*lang, "en", 2)==0) {
	    setlocale( LC_ALL, "C");
	    found = 1;
	}
	if (found)
	    break;
	lang++;
    }

    if (!found)
	setlocale( LC_ALL, "C");

    return;
}


#ifdef _DEBUG
#define getenv mygetenv

char *my_lang_buff[] = {
    "zz",
    "zh",
    "zh-tw",
    "zh-sg",
    "zh-hk",
    "zz,zz,",
    "zz,zz ,zz  ",
    "zz,zz ;q=3,zz  ;q=2, zz    ;q=1,",
    "hh,hh,hh, ,, ,   ,    ,",
    "h1;q=1,h2;q=2",
    "h1;q=1,h2;q=2,h2",
    "h1;q=1,h2;q=2,h2,h3",
    "h1;q=1,h2;q=2,h2,h3,h4",
    "h1;q=1,h2;q=2,h3,h4,h5",
    "h1;q=1,h2;q=2,h3,h4,h5,h6",
    "h1;q=1,h2;q=2,h3,h4,h5,h6,h7",
    "h1;q=1,h2;q=2,h3,h4,h5,h6,h7,h8",
    "h1;q=1,h2;q=2,h3,h4,h5,h6,h7,h8,h9",
    "h1;q=1,h2;q=2,h3,h4,h5,h6,h7,h8,h9,h10",
    "h1;q=1,h2;q=2,h3,h4,h5,h6,h7,h8,h9,h10,h11",
    "h1;q=1,h2;q=2,h3,h4,h5,h6,h7,h8,h9,h10,h11,h12",
    "zh,zh-cn;q=0.5,zh-tw;q=0.8,zh-hk",
    "en,zh-sg,zh,zh-sg,zh-cn,zh-tw,ar,it,",
    "it,en_us,en,zh-sg,zh,zh-sg,zh-cn,zh-tw,ar,it,",
    "it,en,en_us,zh-sg,zh,zh-sg,zh-cn,zh-tw,ar,it,it,en_us,en,zh-sg,zh,zh-sg,zh-cn,zh-tw,ar,it,ko,jp-jp",
    "it,zh-sg,en_us,en,zh,zh-sg,zh-cn,zh,zh-tw,ar,it",
    "it,en_us,en,zh-sg,zh,zh-sg,zh-cn,zh,zh-tw,ar,it,zr1",
    "it,zh,en_us,en,zh-sg,zh-sg,zh-cn,zh,zh-tw,ar,it,zr1,zr2",
    "it,zh,zh-cn,en_us,en,zh-sg,zh-sg,zh,zh-tw,ar,it,zr1,zr2,zr3",
    "it,zh-hk,en_us,en,zh-sg,zh,zh-sg,zh,zh-cn,ar,it,zr1,zr2,zr3,zr4",
    "it,zh-tw,en_us,en,zh-sg,zh,zh-sg,zh-cn,zh,ar,it,zr1,zr2,zr3,zr4,zr5",
    "it,zh,en_us,en,zh-aa,zh,zh-ab,zh-ac,zh,zh-ad,ar,it,zr1,zr2,zr3,zr4,zr5, zr6",
    "it,en_us,en,zh-sg,zh,zh-sg,zh-cn,zh,zh-tw,ar,it,zr1,zr2,zr3,zr4,zr5, zr6;q=1,zr7",
    "it,en_us,en,zh-sg,zh,zh-sg,zh-cn,zh,zh-tw,ar,it,zr1,zr2,zr3,zr4,zr5, zr6;q=1,zr7,zr8",
    "it,en_us,en,zh-sg,zh,zh-sg,zh-cn,zh,zh-tw,ar,it,zr1,zr2,zr3,zr4,zr5, zr6;q=1,zr7,zr8,zr9",
    };

    char *mygetenv( char *ignore)
{
    static int		loop = -1;
    int 		num = sizeof(my_lang_buff)/sizeof(*my_lang_buff);
    return ++loop < num ? my_lang_buff[loop] : NULL;
}

    int
main()
{
    char	**lang, **orig;

    while (lang = get_accept_language())
    {
	if (*lang == NULL)
	    break;
	orig = lang;
	while (*(lang++) != NULL);
	printf ("num: %d. ", lang - orig -1);
	lang = orig;
	while (*lang != NULL) {
	    printf("%s,", *lang);
	    lang++;
	}
	printf("\n");
    }
    for (i=0; i< num; i++)
    {
	char *p;
	init_locale();
	p = setlocale( LC_ALL, NULL );
	printf ("locale: %s.\n", p == NULL? "" : p);
    }	
}

#endif

// vim: noet ts=8 sw=4
