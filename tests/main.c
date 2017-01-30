#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <diffstats/diffstats.h>

static void test_check_for_diff_header(void) {
  int ok = diff_stats_is_diff_header("diff", 0);
  assert(ok);
  int fail = !diff_stats_is_diff_header("diff", 1);
  assert(fail);
}

static void test_check_for_hunk_header(void) {
  int ok = diff_stats_is_hunk_header("@@", 0);
  assert(ok);
  int fail = !diff_stats_is_hunk_header("@@", 1);
  assert(fail);
}

static void test_simple(void) {
  static char* const diff_simple_test =
    "diff --git a/page b/page\n"
    "--- a/page\n"
    "+++ b/page\n"
    "@@ -1,1 +1,1 @@\n"
    "-test\n"
    "+test 2\n";

  printf("\ntesting\n%s\n", diff_simple_test);
  long count = 0;
  int err = 0;
  struct diff_stats_t* st = diff_stats_parser(diff_simple_test, &count, &err);
  assert(st[0].starts_at == 0);
  assert(st[0].count == 1);
  free(st);
}

static void test_with_2_hunks(void) {
  static char* const diff_with_2_hunks_test =
    "diff --git a/page b/page\n"
    "--- a/page\n"
    "+++ b/page\n"
    "@@ -1,8 +1,8 @@\n"
    "-test 3\n"
    "+test 3 iasdf\n"
    "\n"
    "\n"
    "\n"
    "-test 2\n"
    "+test 2 asdfasdf\n"
    "\n"
    "\n"
    "\n"
    "@@ -21,8 +21,8 @@\n"
    "\n"
    "asdasdfasdfasd\n"
    "\n"
    "-asdfasdf\n"
    "-asdfa\n"
    "+asdfasdfs dasfd\n"
    "+asdfa asdfasdf\n"
    "sdfa\n"
    "sdf\n"
    "asdf";

  printf("\ntesting\n%s\n", diff_with_2_hunks_test);
  long count = 0;
  int err = 0;
  struct diff_stats_t* st = diff_stats_parser(diff_with_2_hunks_test, &count, &err);
  assert(st[0].starts_at == 0);
  assert(st[0].count == 2);
  free(st);
}

static void test_mixed(void) {
  static const char* const diff_mixed_test =
    "diff --git a/page b/page\n"
    "--- a/page\n"
    "+++ b/page\n"
    "@@ -1,1 +1,1 @@\n"
    "-test\n"
    "+test 2\n"
    "diff --git a/page b/page\n"
    "--- a/page\n"
    "+++ b/page\n"
    "@@ -1,1 +1,1 @@\n"
    "-test\n"
    "+test 2\n";

  printf("\ntesting\n%s\n", diff_mixed_test);
  long count = 0;
  int err = 0;
  struct diff_stats_t* st = diff_stats_parser(diff_mixed_test, &count, &err);
  assert(st[0].starts_at == 0);
  assert(st[0].count == 1);
  assert(st[1].starts_at == 77);
  assert(st[1].count == 1);
  free(st);
}

static void test_hunk_without_diff(void) {
  static const char* const diff_hunk_without_diff_test =
    "@@ -1,1 +1,1 @@\n"
    "-test\n"
    "+test 2\n";

  printf("\ntesting\n%s\n", diff_hunk_without_diff_test);
  long count = 0;
  int err = 0;
  struct diff_stats_t* st = diff_stats_parser(diff_hunk_without_diff_test, &count, &err);
  assert(st == NULL);
  assert(err == HUNK_WITHOUT_DIFF);
}

int main(int argc, char* argv[]) {
  test_check_for_diff_header();
  test_check_for_hunk_header();
  test_simple();
  test_with_2_hunks();
  test_mixed();
  test_hunk_without_diff();
  return 0;
}
