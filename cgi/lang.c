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
    #define _ACCEPT_LANGS_ARRAY 8
    char **accept_languages = NULL;
#else
    #define _ACCEPT_LANGS_ARRAY 8
    static char **accept_languages = NULL;
#endif
    char *p, *pnext, *buff;
    char **lang;
    int found = 0;
    int i,j,k,m;
    int array_size = _ACCEPT_LANGS_ARRAY;

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

    return accept_languages = lang;
}

#ifdef _DEBUG
#define getenv mygetenv

char *my_lang_buff[] = {
    "",
    "zh",
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
    "it,en_us,en,zh-sg,zh,zh-sg,zh-cn,zh-tw,ar,it,it,en_us,en,zh-sg,zh,zh-sg,zh-cn,zh-tw,ar,it,ko,jp-jp",
    "it,en_us,en,zh-sg,zh,zh-sg,zh-cn,zh,zh-tw,ar,it",
    "it,en_us,en,zh-sg,zh,zh-sg,zh-cn,zh,zh-tw,ar,it,zr1",
    "it,en_us,en,zh-sg,zh,zh-sg,zh-cn,zh,zh-tw,ar,it,zr1,zr2",
    "it,en_us,en,zh-sg,zh,zh-sg,zh-cn,zh,zh-tw,ar,it,zr1,zr2,zr3",
    "it,en_us,en,zh-sg,zh,zh-sg,zh-cn,zh,zh-tw,ar,it,zr1,zr2,zr3,zr4",
    "it,en_us,en,zh-sg,zh,zh-sg,zh-cn,zh,zh-tw,ar,it,zr1,zr2,zr3,zr4,zr5",
    "it,en_us,en,zh-sg,zh,zh-sg,zh-cn,zh,zh-tw,ar,it,zr1,zr2,zr3,zr4,zr5, zr6",
    "it,en_us,en,zh-sg,zh,zh-sg,zh-cn,zh,zh-tw,ar,it,zr1,zr2,zr3,zr4,zr5, zr6;q=1,zr7",
    "it,en_us,en,zh-sg,zh,zh-sg,zh-cn,zh,zh-tw,ar,it,zr1,zr2,zr3,zr4,zr5, zr6;q=1,zr7,zr8",
    "it,en_us,en,zh-sg,zh,zh-sg,zh-cn,zh,zh-tw,ar,it,zr1,zr2,zr3,zr4,zr5, zr6;q=1,zr7,zr8,zr9",
    };

    char *mygetenv( char *ignore)
{
    static int		loop = -1;
    int num;

    loop++;
    num = sizeof(my_lang_buff)/sizeof(*my_lang_buff);

    return my_lang_buff[loop%num];
}

    int
main()
{
    char	**lang, **orig;
    int		i,j;
    int num = sizeof(my_lang_buff)/sizeof(*my_lang_buff);

    for (i=0; i< num; i++)
    {
	lang = get_accept_language();
	orig = lang;
	j=0;
	while (*(lang++) != NULL) j++;
	printf ("num: %d. ", j);
	lang = orig;
	while (*lang != NULL) {
	    printf("%s,", *lang);
	    lang++;
	}
	printf("\n");

    }	
}

#endif

// vim: noet ts=8 sw=4
