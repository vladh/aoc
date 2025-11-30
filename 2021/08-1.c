#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "common.h"


#define N_DIGITS 10
#define N_DIGIT_PATTERNS 4
#define MAX_N_LETTERS 16


void find_digits(char example_patterns[N_DIGITS][MAX_N_LETTERS], char solved_patterns[N_DIGITS][MAX_N_LETTERS]) {
  char solved_letters[MAX_N_LETTERS] = {0};

  for (i32 i = 0; i < N_DIGITS; i++) {
    // Find 1
    if (strlen(example_patterns[i]) == 2) {
      strcpy(solved_patterns[1], example_patterns[i]);
    }

    // Find 7
    if (strlen(example_patterns[i]) == 3) {
      strcpy(solved_patterns[7], example_patterns[i]);
    }

    // Find 4
    if (strlen(example_patterns[i]) == 4) {
      strcpy(solved_patterns[4], example_patterns[i]);
    }

    // Find 8
    if (strlen(example_patterns[i]) == 7) {
      strcpy(solved_patterns[8], example_patterns[i]);
    }
  }

  // Find 9
  for (i32 i = 0; i < N_DIGITS; i++) {
    if (strlen(example_patterns[i]) == 6) {
      bool is_9 = true;
      for (i32 idx_4 = 0; idx_4 < strlen(solved_patterns[4]); idx_4++) {
        bool found_4_segment_in_curr = false;
        for (i32 idx_curr = 0; idx_curr < strlen(example_patterns[i]); idx_curr++) {
          if (solved_patterns[4][idx_4] == example_patterns[i][idx_curr]) {
            found_4_segment_in_curr = true;
          }
        }
        if (!found_4_segment_in_curr) {
          is_9 = false;
        }
      }
      if (is_9) {
        strcpy(solved_patterns[9], example_patterns[i]);
      }
    }
  }

  // Find 6, 0
  for (i32 i = 0; i < N_DIGITS; i++) {
    if (strlen(example_patterns[i]) == 6) {
      if (strcmp(example_patterns[i], solved_patterns[9]) == 0) {
        continue;
      }
      bool one_segments_present[2] = {false};
      for (i32 idx_curr = 0; idx_curr < strlen(example_patterns[i]); idx_curr++) {
        for (i32 idx_one = 0; idx_one < 2; idx_one++) {
          if (example_patterns[i][idx_curr] == solved_patterns[1][idx_one]) {
            one_segments_present[idx_one] = true;
          }
        }
      }
      if (!one_segments_present[0] && !one_segments_present[1]) {
        continue;
      } else if (one_segments_present[0] && one_segments_present[1]) {
        strcpy(solved_patterns[0], example_patterns[i]);
      } else {
        strcpy(solved_patterns[6], example_patterns[i]);
        if (one_segments_present[0]) {
          solved_letters[2] = solved_patterns[1][1];
        } else {
          solved_letters[2] = solved_patterns[1][0];
        }
      }
    }
  }

  // Find 3
  for (i32 i = 0; i < N_DIGITS; i++) {
    if (strlen(example_patterns[i]) == 5) {
      i32 n_one_segments_present = 0;
      for (i32 idx_curr = 0; idx_curr < strlen(example_patterns[i]); idx_curr++) {
        if (
          example_patterns[i][idx_curr] == solved_patterns[1][0] ||
          example_patterns[i][idx_curr] == solved_patterns[1][1]
        ) {
          n_one_segments_present++;
        }
      }
      if (n_one_segments_present == 2) {
        strcpy(solved_patterns[3], example_patterns[i]);
      }
    }
  }

  // Find 2, 5
  for (i32 i = 0; i < N_DIGITS; i++) {
    if (strlen(example_patterns[i]) == 5) {
      if (strcmp(example_patterns[i], solved_patterns[3]) == 0) {
        continue;
      }
      bool has_c_segment = false;
      for (i32 idx_curr = 0; idx_curr < strlen(example_patterns[i]); idx_curr++) {
        if (example_patterns[i][idx_curr] == solved_letters[2]) {
          has_c_segment = true;
        }
      }
      if (has_c_segment) {
        strcpy(solved_patterns[2], example_patterns[i]);
      } else {
        strcpy(solved_patterns[5], example_patterns[i]);
      }
    }
  }

  for (i32 i = 0; i < N_DIGITS; i++) {
    printf("(%d %s) ", i, solved_patterns[i]);
  }
  printf("\n");
  for (i32 i = 0; i < N_DIGITS; i++) {
    assert(solved_patterns[i][0] != '\0');
  }
}


bool is_anagram(char *s1, char *s2) {
  i32 s1_len = strlen(s1);
  i32 s2_len = strlen(s2);
  if (s1_len != s2_len) {
    return false;
  }
  for (i32 i = 0; i < s1_len; i++) {
    bool did_find_char = false;
    for (i32 j = 0; j < s2_len; j++) {
      if (s1[i] == s2[j]) {
        did_find_char = true;
      }
    }
    if (!did_find_char) {
      return false;
    }
  }
  return true;
}


int main() {
  FILE *f = fopen("data/8", "r");

  i64 digit_counts[N_DIGITS] = {0};
  i64 full_sum = 0;

  while (true) {
    char example_patterns[N_DIGITS][MAX_N_LETTERS] = {0};
    char digit_patterns[N_DIGIT_PATTERNS][MAX_N_LETTERS] = {0};
    for (i32 i = 0; i < N_DIGITS; i++) {
      if (fscanf(f, "%s ", example_patterns[i]) < 1) {
        goto end_loop;
      }
    }

    char solved_patterns[N_DIGITS][MAX_N_LETTERS] = {0};
    find_digits(example_patterns, solved_patterns);

    fscanf(f, " | ");

    for (i32 idx_pattern = 0 ; idx_pattern < N_DIGIT_PATTERNS; idx_pattern++) {
      fscanf(f, "%s ", digit_patterns[idx_pattern]);

      i32 digit_value = -1;
      for (i32 idx_digit = 0; idx_digit < N_DIGITS; idx_digit++) {
        if (is_anagram(digit_patterns[idx_pattern], solved_patterns[idx_digit])) {
          digit_value = idx_digit;
        }
      }
      assert(digit_value >= 0);
      digit_counts[digit_value]++;
      full_sum += digit_value * (i32)pow(10.0, (f32)(N_DIGIT_PATTERNS - 1 - idx_pattern));
    }
  }
end_loop:
  ;

  for (i32 idx_digit = 0; idx_digit < N_DIGITS; idx_digit++) {
    printf("(%d %ld) ", idx_digit, digit_counts[idx_digit]);
  }
  printf("\n");

  printf("1,4,7,8: %ld\n", digit_counts[1] + digit_counts[4] + digit_counts[7] + digit_counts[8]);
  printf("full_sum: %ld\n", full_sum);

  return 0;
}
