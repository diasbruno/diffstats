#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <diffstats/diffstats.h>

static struct diff_stats_t* diff_stats_prepare_item(struct diff_stats_t* d) {
  d->starts_at = -1;
  d->additions =  0;
  d->deletions =  0;
  d->count     =  0;
  d->hunks     = 10;
  d->hunks_at  = (long*)malloc(sizeof(long) * 10);
  memset(d->hunks_at, 0, sizeof(long) * 10);
  return d;
}

int diff_stats_is_diff_header(const char* text) {
  return (*text == 'd' && strncmp(text, (char*)"diff", 4) == 0);
}

int diff_stats_is_hunk_header(const char* text) {
  return (*text == '@' && strncmp(text, (char*)"@@", 2) == 0);
}

int diff_stats_line_type(const char* text) {
  if (diff_stats_is_diff_header(text)) {
    return DIFF_HEADER;
  }
  if (diff_stats_is_hunk_header(text)) {
    return HUNK_HEADER;
  }
  switch (*text) {
  case '-': { return DELETION_LINE; } break;
  case '+': { return ADDITION_LINE; } break;
  default:  { return DIFF_NOTHING;  } break;
  }
}

void diff_stats_add_hunk_at(struct diff_stats_t* d, long i) {
  if (d->count + 1 == d->hunks) {
    d->hunks = d->hunks + 10;
    d->hunks_at = (long*)realloc(d->hunks_at, sizeof(long) * d->hunks);
  }
  d->hunks_at[d->count] = i;
  d->count += 1;
}

void diff_stats_print_list(struct diff_stats_t* d, long count) {
  printf("-> print base %p\n", d);
  for (int i = 0; i < count; i++) {
    printf("-> print item %p\n", &d[i]);
    diff_stats_print(&d[i]);
  }
}

void diff_stats_print(struct diff_stats_t* d) {
  printf("diff starts at %ld with %ld hunk(s).\n",
         d->starts_at,
         d->count);
  for (int j = 0; j < d->count; j++) {
    printf("hunk at %ld\n", d->hunks_at[j]);
  }
}

void diff_stats_free(struct diff_stats_t* d, long count) {
  for (int i = 0 ; i < count; i++) {
    free(d[i].hunks_at);
  }
  free(d);
}

char* diff_stats_find_hunk(const char* str) {
  char* text = (char*)str;
  while (*text && *(text + 1)) {
    if (*text == '\n' && *(text + 1) == '@') {
      break;
    }
    text++;
  }
  return text;
}

struct diff_stats_t* diff_stats_parser(const char* str, long* counter, int* err) {
  char* hold   = (char*)str;
  char* text   = hold;
  long  index  = 0;
  long  count  = 12;
  struct diff_stats_t* diff =
    (struct diff_stats_t*)malloc(sizeof(struct diff_stats_t) * count);
  struct diff_stats_t* head = NULL;

  while (*text) {
    int type = diff_stats_line_type(text);

    switch (type) {
    case DIFF_HEADER: {
      if (index + 1 <= count) {
        count += 12;
        diff = (struct diff_stats_t*)realloc(diff,
                                             sizeof(struct diff_stats_t) * count);
      }
      head = diff_stats_prepare_item(diff + index);
      head->starts_at = (long)(text - hold);
      index++;

      text = diff_stats_find_hunk(text);
      continue;
    } break;
    case HUNK_HEADER: {
      if (!head) {
        *err = HUNK_WITHOUT_DIFF;
        free(diff);
        diff = NULL;
        goto diff_stats_end;
      }
      diff_stats_add_hunk_at(head, (long)(text - hold));
    } break;
    case ADDITION_LINE: { head->additions++; goto diff_stats_skip; } break;
    case DELETION_LINE: { head->deletions++; goto diff_stats_skip; } break;
    default: {
diff_stats_skip:
      while (*text && *text != '\n') text++;
    }
    }

    text++;
  }

  *err = 0;
  *counter = index;

diff_stats_end:

  return diff;
}
