#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "common.h"


#define MAX_LINE_LEN 256
#define MAX_N_SCORES 90


int compare(const void *a, const void *b) {
  u64 int_a = *((u64*)a);
  u64 int_b = *((u64*)b);

  if (int_a == int_b) {
    return 0;
  } else if (int_a < int_b) {
    return -1;
  } else {
    return 1;
  }
}


i32 score(char c) {
  if (c == '(') {
    return 1;
  } else if (c == '[') {
    return 2;
  } else if (c == '{') {
    return 3;
  } else if (c == '<') {
    return 4;
  }
  printf("WARNING: invalid char to score\n");
  return -1;
}


bool is_opener(char c) {
  return c == '(' ||
    c == '[' ||
    c == '{' ||
    c == '<';
}


bool is_closer(char c) {
  return c == ')' ||
    c == ']' ||
    c == '}' ||
    c == '>';
}


char get_open_version(char c) {
  if (c == ')') {
    return '(';
  } else if (c == ']') {
    return '[';
  } else if (c == '}') {
    return '{';
  } else if (c == '>') {
    return '<';
  }
  printf("WARNING: invalid closed char to get open version for\n");
  return '\0';
}


int main() {
  FILE *f = fopen("data/10", "r");

  char stack[MAX_LINE_LEN] = {};
  i32 stack_size = 0;

  u64 scores[MAX_N_SCORES] = {};
  i32 n_scores = 0;

  while (true) {
    char c = (char)fgetc(f);

    if (c == EOF) {
      break;
    } else if (c == '\n') {
      if (stack_size != 0) {
        u64 subscore = 0;
        for (i32 idx_stack = stack_size - 1; idx_stack >= 0; idx_stack--) {
          subscore = subscore * 5 + score(stack[idx_stack]);
          printf("%c", stack[idx_stack]);
        }
        printf(" %ld\n", subscore);
        scores[n_scores++] = subscore;
      }
      memset(stack, 0, sizeof(stack));
      stack_size = 0;
      continue;
    }

    if (is_opener(c)) {
      stack[stack_size++] = c;
    } else if (is_closer(c)) {
      if (stack[stack_size - 1] == get_open_version(c)) {
        stack[stack_size - 1] = '\0';
        stack_size--;
      } else {
        while (c != '\n') {
          c = (char)fgetc(f);
        }
        memset(stack, 0, sizeof(stack));
        stack_size = 0;
        continue;
      }
    }
  }

  qsort(scores, n_scores, sizeof(u64), compare);

  printf("%d scores: ", n_scores);
  for (i32 idx_score = 0; idx_score < n_scores; idx_score++) {
    printf("%ld ", scores[idx_score]);
  }
  printf("\n");

  printf("%ld\n", scores[n_scores / 2]);

  return 0;
}
