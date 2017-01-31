#ifndef __DIFFSTATS_H__
#define __DIFFSTATS_H__ 1

enum diff_stats_err_t {
  HUNK_WITHOUT_DIFF = 1
};

enum diff_stats_line_t {
  DIFF_NOTHING  = 0,
  DIFF_HEADER   = 1,
  HUNK_HEADER   = 2,
  ADDITION_LINE = 3,
  DELETION_LINE = 4
};

struct diff_stats_t {
  long  starts_at;
  long  additions;
  long  deletions;
  long  count;
  long  hunks;
  long* hunks_at;
};

/**
 * From the current point in text, is there a `diff`?
 * @param {char*} text   - the string.
 * @param {long}  column - text column (updated every new line).
 * @return {int}
 */
int diff_stats_is_diff_header(const char* text);

/**
 * From the current point in text, is there a `@@`?
 * @param {char*} text   - the string.
 * @param {long}  column - text column (updated every new line).
 * @return {int}
 */
int diff_stats_is_hunk_header(const char* text);

/**
 * Find the type of the line.
 * @param {const char*} text - text.
 * @return {int}
 */
int diff_stats_line_type(const char* text);

/**
 * Add a found hunk to the list.
 * @param {struct diff_stats_t*} d - diff stats object.
 * @param {long}                 i - index in the text where it was found.
 * @return {void}
 */
void diff_stats_add_hunk_at(struct diff_stats_t* d, long i);

/**
 * Print all matches.
 * @param {struct diff_stats_t*} d     - diff stats object.
 * @param {long}                 count - number of diffs.
 * @return {void}
 */
void diff_stats_print_list(struct diff_stats_t* d, long count);

/**
 * Print the state of the object.
 * @param {struct diff_stats_t*} d - diff stats object.
 * @return {void}
 */
void diff_stats_print(struct diff_stats_t* d);

/**
 * Free the object.
 * @param {struct diff_stats_t*} d     - diff stats object.
 * @param {long}                 count - number of diffs.
 * @return {void}
 */
void diff_stats_free(struct diff_stats_t* d, long count);

/**
 * Parse a string.
 * @param {char*} str     - reference to the string.
 * @param {long*} counter - [out] number of found diffs.
 * @param {int*}  err     - [out] error status.
 * @return {struct diff_stats_t*}
 */
struct diff_stats_t* diff_stats_parser(const char* str, long* counter, int* err);

#endif // __DIFFSTATS_H__
