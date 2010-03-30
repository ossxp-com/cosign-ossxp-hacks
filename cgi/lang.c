#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lang.h"

char **
get_accept_language()
{
    char *accept_language=NULL;
    char *p, *pnext, *buff;
    char **lang;
    int found = 0;
    int i;
    int _INIT_SIZE=3;

    lang = calloc(sizeof(char*),_INIT_SIZE+1);
    for (i=0; i<_INIT_SIZE; i++)
        lang[i]=NULL;

    accept_language = getenv( "HTTP_ACCEPT_LANGUAGE" );
    p = accept_language;

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
                if (strcmp(buff, "zh-cn")==0 || strcmp(buff, "zh_cn")==0)
                {
                    free(buff);
                    buff = strdup("zh");
                }
                else if (strncmp(buff, "en", 2)==0)
                {
                    free(buff);
                    buff = strdup("en");
                }
                found = 0;
                for (i=0; i< _INIT_SIZE; i++)
                {
                    if (lang[i]!=NULL && buff !=NULL && strcmp(lang[i],buff)==0)
                    {
                        found = 1;
                        break;
                    }
                    else if (lang[i] == NULL)
                    {
                        break;
                    }
                }
                if (!found)
                {
                    if (i==_INIT_SIZE)
                    {
                        _INIT_SIZE = _INIT_SIZE * 2 + 1;
                        lang = realloc(lang, _INIT_SIZE * sizeof(char *));
                    }
                    lang[i] = buff;
                    lang[i+1] = NULL;
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
    for(i=0; i< _INIT_SIZE; i++)
    {
        fprintf(stderr, "lang[%d] = %s\n", i, lang[i]);
    }
    */
    return lang;
}
