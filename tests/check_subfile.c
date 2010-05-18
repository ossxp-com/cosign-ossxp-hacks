#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include "../cgi/lang.h"
#include "../cgi/subfile.h"

char * str_replace(char *orig, char *seek, char *rep);

void
stub_do_macro_include( char *str, struct subfile_list *sl )
{
    printf(">> include: %s\n", str);
}

void
stub_do_macro_gettext( char *str )
{
    printf(">> gettext: %s\n", str_replace(str, "\n", "."));
}


START_TEST (test_str_replace)
{
  char *s;

  s = str_replace("Hello$tWorld","$t", ",");
  fail_if ( strcmp(s, "Hello,World") != 0, "result: %s", s);

  s = str_replace("Hello$tWorld","$t", ", I come from the real ");
  fail_if ( strcmp(s, "Hello, I come from the real World") != 0, "result: %s", s);

  s = str_replace("Hello World","Hello", "Software");
  fail_if ( strcmp(s, "Software World") != 0, "result: %s", s);

  s = str_replace("Hello World","o", "OO");
  fail_if ( strcmp(s, "HellOO WOOrld") != 0, "result: %s", s);

  s = str_replace("Hello World","l", "L");
  fail_if ( strcmp(s, "HeLLo WorLd") != 0, "result: %s", s);

  s = str_replace("Hello World","ld", "LD");
  fail_if ( strcmp(s, "Hello WorLD") != 0, "result: %s", s);
}
END_TEST


START_TEST (test_macro_process)
{
  char *s;
  int i;

  s = "include(file)";
  i = _macro_process ( s, strlen(s), NULL, "", 1);
  fail_if ( i != 13, "eaten: %d", i);

  s = "include(file).";
  i = _macro_process ( s, strlen(s), NULL, "", 1);
  fail_if ( i != 13, "eaten: %d", i);

  s = "include(file)xyz";
  i = _macro_process ( s, strlen(s), NULL, "", 1);
  fail_if ( i != 13, "eaten: %d", i);

  s = "include(\"file name\")xyz......";
  i = _macro_process ( s, strlen(s), NULL, "", 1);
  fail_if ( i != 20, "eaten: %d", i);

  s = "_(\"world\").";
  i = _macro_process ( s, strlen(s), NULL, "", 1);
  fail_if ( i != 10, "eaten: %d", i);
  fail_if ( strcmp(s+i,".") != 0, "left: %s", s+i);

  s = "_(\"world\")......";
  i = _macro_process ( s, strlen(s), NULL, "", 1);
  fail_if ( i != 10, "eaten: %d", i);
  fail_if ( strcmp(s+i,"......") != 0, "left: %s", s+i);

	s = "_(\"world.\")";
  i = _macro_process ( s, strlen(s), NULL, "", 1);
  fail_if ( i != 11, "eaten: %d", i);
  fail_if ( strcmp(s+i,"") != 0, "left: %s", s+i);

	s="_(\"hello,\" \n \" world.\") end of text.";
  i = _macro_process ( s, strlen(s), NULL, "", 1);
  fail_if ( i != 23, "eaten: %d", i);
  fail_if ( strcmp(s+i," end of text.") != 0, "left: %s", s+i);

  s = "_(\"hello\\\",\\\"\" \n \" world.\" \" baby\") end of paragraph1.";
  i = _macro_process ( s, strlen(s), NULL, "", 1);
  fail_if ( i != 35, "eaten: %d", i);
  fail_if ( strcmp(s+i," end of paragraph1.") != 0, "left: %s", s+i);

	s = "_(\"Please enter your authentication information and click the &quot;Log&nbsp;In&quot; button\"\n \t\t\t\t\t\" to continue.\")</p>";
  i = _macro_process ( s, strlen(s), NULL, "", 1);
  fail_if ( i != 116, "eaten: %d,\n s = %s", i, s+i);
  fail_if ( strcmp(s+i,"</p>") != 0, "left: %s", s+i);
}
END_TEST

Suite *
ossxp_suite (void)
{
  Suite *s = suite_create ("Subfile");

  /* str_replace test case */
  TCase *tc_str = tcase_create ("str");
  tcase_add_test (tc_str, test_str_replace);
  suite_add_tcase (s, tc_str);

  /* macro_process test case */
  TCase *tc_macro = tcase_create ("macro");
  tcase_add_test (tc_macro, test_macro_process);
  suite_add_tcase (s, tc_macro);

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
