#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "lang.h"

    char **
get_accept_language()
{
    static char **accept_languages = NULL;
    char *p, *pnext, *buff;
    char **lang;
    int found = 0;
    int i,j,k,m;
    int array_size=8;

    if (accept_languages != NULL)
	return accept_languages;

    lang = calloc(sizeof(char*),array_size+1);
    for (i=0; i<array_size; i++)
	lang[i]=NULL;

    p = getenv( "HTTP_ACCEPT_LANGUAGE" );

    if (p != NULL)
    {
	while(1)
	{
	    int i=0;
	    while (1)
	    {
		char c = *(p+i);
		if ( c=='\0' || c== ';' || c==',')
		    break;
		i++;
	    }
	    if (i>0)
		buff = strndup(p,i);
	    else
		buff = NULL;

	    if (buff!=NULL)
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

    /*
    // Debug: print to stderr will be logged in apache log file.
    for(i=0; i< array_size; i++)
    {
	if (lang[i] == NULL)
	    break;
	fprintf(stderr, "lang[%d] = %s\n", i, lang[i]);
    }
    */

    return accept_languages = lang;
}

    void
init_locale()
{
#define MAX_LANG_CODE	20
    char	**lang;
    char	*p;
    char	lang_utf8[MAX_LANG_CODE+7] = "";

    bindtextdomain("cosign", _LOCALEDIR);
    textdomain("cosign");
    lang = get_accept_language();
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
	    break;
	} else if (strcmp(*lang, "zh")==0) {
	    if (setlocale( LC_ALL, "zh_CN.UTF-8") != NULL)
		break;
	    else if (setlocale( LC_ALL, "zh_CN") != NULL)
		break;
	    else if (setlocale( LC_ALL, *lang) != NULL)
		break;
	} else if (setlocale( LC_ALL, *lang) != NULL) {
	    break;
	} else if (strncmp(*lang, "en", 2)==0) {
	    setlocale( LC_ALL, "C");
	    break;
	}
	lang++;
    }
}


// vim: noet ts=8 sw=4
