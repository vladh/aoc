#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "common.h"


#define MAX_SEQUENCE_SIZE 2048
char sequence[MAX_SEQUENCE_SIZE] = {};
char sequence_temp[MAX_SEQUENCE_SIZE] = {};
#define MAX_N_RULES 128
char rules_from[MAX_N_RULES][2] = {};
char rules_to[MAX_N_RULES] = {};
i32 n_rules = 0;
#define N_STEPS 4
i32 step = 0;


void get_most_and_least_common() {
  char frequencies[256] = {};

  for (i32 i = 0; i < strlen(sequence); i++) {
    frequencies[sequence[i]]++;
  }

  i32 max = 0;
  i32 min = INT_MAX;
  i32 idx_max;
  i32 idx_min;

  for (i32 i = 'A'; i <= 'Z'; i++) {
    if (frequencies[i] > max) {
      max = frequencies[i];
      idx_max = i;
    }
    if (frequencies[i] < min) {
      min = frequencies[i];
      idx_min = i;
    }
  }

  printf("Most common: %c (%d)\n", idx_max, max);
  printf("Least common: %c (%d)\n", idx_min, min);
  printf("max - min = %d\n", max - min);
}


int main() {
  FILE *f = fopen("data/14", "r");

  assert(fscanf(f, "%s\n\n", sequence) > 0);

  char rule_from[3];
  char rule_to[1];
  while (fscanf(f, "%s -> %c\n", rule_from, rule_to) > 0) {
    rules_from[n_rules][0] = rule_from[0];
    rules_from[n_rules][1] = rule_from[1];
    rules_to[n_rules] = rule_to[0];
    n_rules++;
  }

  while (step < N_STEPS) {
    printf("\n=== Step %d\n", step);
    printf("%s\n", sequence);
    i32 idx = 0;
    while (true) {
      for (i32 idx_rule = 0; idx_rule < n_rules; idx_rule++) {
        // i 1 2 3 4 5 6
        // A B Q Q Q Q
        // A X B Q Q Q Q
        if (rules_from[idx_rule][0] == sequence[idx] && rules_from[idx_rule][1] == sequence[idx + 1]) {
          printf("%c%c -> %c\n", rules_from[idx_rule][0], rules_from[idx_rule][1], rules_to[idx_rule]);
          strcpy(sequence_temp, &sequence[idx + 1]);
          strcpy(&sequence[idx + 2], sequence_temp);
          sequence[idx + 1] = rules_to[idx_rule];
          i32 len = strlen(sequence);
          sequence[len + 1] = '\0';
          idx++;
          break;
        }
      }

      printf("%s\n", sequence);

      idx++;
      if (idx >= strlen(sequence) - 1) {
        break;
      }
    }
    step++;
  }

  printf("\n%s\n", sequence);

  get_most_and_least_common();

  return 0;
}
