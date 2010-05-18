#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include "../cgi/lang.h"

int
get_lang_size(char **lang)
{
  int i = 0;
  char **p = lang;

	while (*(p++) != NULL) i++;
  return i;
}

void
print_lang(char **lang)
{
  char **p = lang;
  int i = get_lang_size(lang);

	printf ( "Count: %3d.", i );

  if (i > 0)
      printf(" Lang: ");

	while (*p != NULL) {
	    printf("%s,", *p);
	    p++;
	}
	printf("\n");
}

START_TEST (test_lang_one)
{
  char **lang;

  lang = _get_accept_language( "zh-hk" );
  fail_if ( strcmp(lang[0], "zh_TW") != 0, "%s != zh_TW", lang[0]);
  fail_if ( lang[1] != NULL, "not NULL");

  lang = _get_accept_language( "zh_tW" );
  fail_if ( strcmp(lang[0], "zh_TW") != 0, "%s != zh_TW", lang[0]);
  fail_if ( lang[1] != NULL, "not NULL");

  lang = _get_accept_language( "zh-cn" );
  fail_if ( strcmp(lang[0], "zh_CN") != 0, "%s != zh_CN", lang[0]);
  fail_if ( lang[1] != NULL, "not NULL");

  lang = _get_accept_language( "zh-sg" );
  fail_if ( strcmp(lang[0], "zh_CN") != 0, "%s != zh_CN", lang[0]);
  fail_if ( lang[1] != NULL, "not NULL");

  lang = _get_accept_language( "zh" );
  fail_if ( strcmp(lang[0], "zh") != 0, "%s != zh", lang[0]);
  fail_if ( lang[1] != NULL, "not NULL");

  lang = _get_accept_language( "en" );
  fail_if ( strcmp(lang[0], "en") != 0, "%s != en", lang[0]);
  fail_if ( lang[1] != NULL, "not NULL");

  lang = _get_accept_language( "en-us" );
  fail_if ( strcmp(lang[0], "en_US") != 0, "%s != en_US", lang[0]);
  fail_if ( lang[1] != NULL, "not NULL");

  lang = _get_accept_language( "xxx" );
  fail_if ( strcmp(lang[0], "xxx") != 0, "%s != xxx", lang[0]);
  fail_if ( lang[1] != NULL, "not NULL");
}
END_TEST

START_TEST (test_lang_two)
{
  char **lang;

  lang = _get_accept_language( "zh-hk,zh" );
  fail_if ( strcmp(lang[0], "zh_TW") != 0, "%s != zh_TW", lang[0]);
  fail_if ( strcmp(lang[1], "zh") != 0, "%s != zh", lang[1]);
  fail_if ( lang[2] != NULL, "not NULL");

  lang = _get_accept_language( "en;q=0.1,zh_tW;q=2" );
  fail_if ( strcmp(lang[0], "en") != 0, "%s != en", lang[0]);
  fail_if ( strcmp(lang[1], "zh_TW") != 0, "%s != zh_TW", lang[1]);
  fail_if ( lang[2] != NULL, "not NULL");

  lang = _get_accept_language( "en;,zh_tW;q=2" );
  fail_if ( strcmp(lang[0], "en") != 0, "%s != en", lang[0]);
  fail_if ( strcmp(lang[1], "zh_TW") != 0, "%s != zh_TW", lang[1]);
  fail_if ( lang[2] != NULL, "not NULL");

  lang = _get_accept_language( "en;,zh_tW;q" );
  fail_if ( strcmp(lang[0], "en") != 0, "%s != en", lang[0]);
  fail_if ( strcmp(lang[1], "zh_TW") != 0, "%s != zh_TW", lang[1]);
  fail_if ( lang[2] != NULL, "not NULL");

  lang = _get_accept_language( "zh-cn,zh-sg" );
  fail_if ( strcmp(lang[0], "zh_CN") != 0, "%s != zh_CN", lang[0]);
  fail_if ( lang[1] != NULL, "not NULL");

  lang = _get_accept_language( "zh-hk;q=0.8,zh-tw;q=0.5" );
  fail_if ( strcmp(lang[0], "zh_TW") != 0, "%s != zh_TW", lang[0]);
  fail_if ( lang[1] != NULL, "not NULL");

  lang = _get_accept_language( "zh-sg,zh-hk;q=0.5" );
  fail_if ( strcmp(lang[0], "zh_CN") != 0, "%s != zh_CN", lang[0]);
  fail_if ( strcmp(lang[1], "zh_TW") != 0, "%s != zh_TW", lang[1]);
  fail_if ( lang[2] != NULL, "not NULL");

  lang = _get_accept_language( "zh-sg,zh-hk;q=0.5," );
  fail_if ( strcmp(lang[0], "zh_CN") != 0, "%s != zh_CN", lang[0]);
  fail_if ( strcmp(lang[1], "zh_TW") != 0, "%s != zh_TW", lang[1]);
  fail_if ( lang[2] != NULL, "not NULL");

  lang = _get_accept_language( "zh-sg,zh-hk;q=0.5,," );
  fail_if ( strcmp(lang[0], "zh_CN") != 0, "%s != zh_CN", lang[0]);
  fail_if ( strcmp(lang[1], "zh_TW") != 0, "%s != zh_TW", lang[1]);
  fail_if ( lang[2] != NULL, "not NULL");

  lang = _get_accept_language( "zh-sg , zh-hk ;q=0.5 , , " );
  fail_if ( strcmp(lang[0], "zh_CN") != 0, "%s != zh_CN", lang[0]);
  fail_if ( strcmp(lang[1], "zh_TW") != 0, "%s != zh_TW", lang[1]);
  fail_if ( get_lang_size(lang) != 2, "wrong size");
}
END_TEST

START_TEST (test_lang_many)
{
  char **lang;
  char *env;
  int i = 0;

  lang = _get_accept_language( "zz,zz ;q=3,zz  ;q=2, zz    ;q=1," );
  fail_if ( get_lang_size(lang) != 1, "wrong size: %d", get_lang_size(lang));

  lang = _get_accept_language( "l1;q=1,l2;q=2,l2,l3" );
  fail_if ( get_lang_size(lang) != 3, "wrong size: %d", get_lang_size(lang));

  lang = _get_accept_language( "l1;q=1,l2;q=2,l2,l3,l4" );
  fail_if ( get_lang_size(lang) != 4, "wrong size: %d", get_lang_size(lang));

  env = malloc(200*4+50);
  strcpy(env, "l0");
  for (i=1; i< 200; i++)
  {
    sprintf(env, "%s,l%d", env, i);
    lang = _get_accept_language( env );
    fail_if ( get_lang_size(lang) != i+1, "Lang %s.\nWrong size: %d != %d", env, get_lang_size(lang), i);
  }
}
END_TEST


START_TEST (test_locale_one)
{
  char **lang;
  char *locale;

  _init_locale( "zh-hk" );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "zh_TW.UTF-8") != 0, "locale is %s, no equal.", locale);

  _init_locale( "zh_tW" );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "zh_TW.UTF-8") != 0, "locale is %s, no equal.", locale);

  _init_locale( "zh-cn" );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "zh_CN.UTF-8") != 0, "locale is %s, no equal.", locale);

  _init_locale( "zh-sg" );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "zh_CN.UTF-8") != 0, "locale is %s, no equal.", locale);

  _init_locale( "zh" );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "zh_CN.UTF-8") != 0, "locale is %s, no equal.", locale);

  _init_locale( "en" );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "C") != 0, "locale is %s, no equal.", locale);

  _init_locale( "en-us" );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "en_US.UTF-8") != 0, "locale is %s, no equal.", locale);

  _init_locale( "xxx" );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "C") != 0, "locale is %s, no equal.", locale);
}
END_TEST

START_TEST (test_locale_many)
{
  char **lang;
  char *locale;

  _init_locale( "zh-hk,zh,en,en_US" );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "zh_TW.UTF-8") != 0, "locale is %s, no equal.", locale);

  _init_locale( "zh-hk;q=0.8,zh,en;q=1,en_US" );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "zh_TW.UTF-8") != 0, "locale is %s, no equal.", locale);

  _init_locale( "zh,zh-hk;zh_tw" );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "zh_CN.UTF-8") != 0, "locale is %s, no equal.", locale);

  _init_locale( "xx,zh-hk,en" );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "zh_TW.UTF-8") != 0, "locale is %s, no equal.", locale);

  _init_locale( "xx,yy,zh" );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "zh_CN.UTF-8") != 0, "locale is %s, no equal.", locale);

  _init_locale( "en;q=0.1,zh_tW;q=2" );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "C") != 0, "locale is %s, no equal.", locale);

  _init_locale( "en;,zh_tW;q=2" );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "C") != 0, "locale is %s, no equal.", locale);

  _init_locale( "en;,zh_tW;q" );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "C") != 0, "locale is %s, no equal.", locale);

  _init_locale( "zh-cn,zh-sg" );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "zh_CN.UTF-8") != 0, "locale is %s, no equal.", locale);

  _init_locale( "zh-hk;q=0.8,zh-tw;q=0.5" );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "zh_TW.UTF-8") != 0, "locale is %s, no equal.", locale);

  _init_locale( "zh-sg,zh-hk;q=0.5" );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "zh_CN.UTF-8") != 0, "locale is %s, no equal.", locale);

  _init_locale( "zh-sg,zh-hk;q=0.5," );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "zh_CN.UTF-8") != 0, "locale is %s, no equal.", locale);

  _init_locale( "zh-sg,zh-hk;q=0.5,," );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "zh_CN.UTF-8") != 0, "locale is %s, no equal.", locale);

  _init_locale( "zh-sg , zh-hk ;q=0.5 , , " );
	locale = setlocale( LC_ALL, NULL );
  fail_if ( strcmp(locale, "zh_CN.UTF-8") != 0, "locale is %s, no equal.", locale);
}
END_TEST


Suite *
ossxp_suite (void)
{
  Suite *s = suite_create ("Language");

  /* Lang test case */
  TCase *tc_lang = tcase_create ("Lang");
  /* tcase_add_checked_fixture (tc_lang, setup, teardown); */
  tcase_add_test (tc_lang, test_lang_one);
  tcase_add_test (tc_lang, test_lang_two);
  tcase_add_test (tc_lang, test_lang_many);
  suite_add_tcase (s, tc_lang);

  /* Locale test case */
  TCase *tc_locale = tcase_create ("Locale");
  tcase_add_test (tc_locale, test_locale_one);
  tcase_add_test (tc_locale, test_locale_many);
  suite_add_tcase (s, tc_locale);

  /* Limits test case */
  /*
  TCase *tc_limits = tcase_create ("Limits");
  tcase_add_test (tc_limits, test_money_create_neg);
  tcase_add_test (tc_limits, test_money_create_zero);
  suite_add_tcase (s, tc_limits);
  */

  return s;
}

int
main (void)
{
  int number_failed;
  Suite *s = ossxp_suite ();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
