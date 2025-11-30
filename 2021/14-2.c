#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "common.h"


#define MAX_SEQUENCE_SIZE 128
char sequence[MAX_SEQUENCE_SIZE] = {};
#define MAX_N_RULES 128
char rules_from[MAX_N_RULES][2] = {};
char rules_to[MAX_N_RULES] = {};
i32 n_rules = 0;
#define N_CHARS 26
u64 pair_freqs[N_CHARS][N_CHARS] = {};
u64 pair_freqs_delta[N_CHARS][N_CHARS] = {};
#define N_STEPS 40
i32 step = 0;


void print_pair_freqs() {
  for (char c1 = 'A'; c1 <= 'Z'; c1++) {
    for (char c2 = 'A'; c2 <= 'Z'; c2++) {
      if (pair_freqs[c1 - 'A'][c2 - 'A'] > 0) {
        printf("%c%c: %lu\n", c1, c2, pair_freqs[c1 - 'A'][c2 - 'A']);
      }
    }
  }
}


void print_most_and_least_common() {
  u64 freqs[N_CHARS] = {};
  for (char c1 = 'A'; c1 <= 'Z'; c1++) {
    for (char c2 = 'A'; c2 <= 'Z'; c2++) {
      u64 pair_freq = pair_freqs[c1 - 'A'][c2 - 'A'];
      if (pair_freq > 0) {
        freqs[c1 - 'A'] += pair_freq;
        // We have no way of knowing if the sequence ends in this character,
        // in which case we should add one more of it. Oops.
        /* freqs[c2 - 'A'] += 1; */
      }
    }
  }

  u64 min = ~0;
  u64 max = 0;
  i32 idx_min = 0;
  i32 idx_max = 0;
  for (i32 i = 0; i < N_CHARS; i++) {
    if (freqs[i] > 0) {
      if (freqs[i] < min) {
        min = freqs[i];
        idx_min = i;
      }
      if (freqs[i] > max) {
        max = freqs[i];
        idx_max = i;
      }
    }
  }

  printf("Most common: %c (%lu)\n", ('A' + idx_max), max);
  printf("Least common: %c (%lu)\n", ('A' + idx_min), min);
  printf("max - min: %lu\n", max - min);
}


int main() {
  FILE *f = fopen("data/14", "r");

  assert(fscanf(f, "%s\n\n", sequence) > 0);
  for (i32 i = 1; i < strlen(sequence); i++) {
    pair_freqs[sequence[i - 1] - 'A'][sequence[i] - 'A']++;
  }

  char rule_from[3];
  char rule_to[1];
  while (fscanf(f, "%s -> %c\n", rule_from, rule_to) > 0) {
    rules_from[n_rules][0] = rule_from[0];
    rules_from[n_rules][1] = rule_from[1];
    rules_to[n_rules] = rule_to[0];
    n_rules++;
  }

  while (step < N_STEPS) {
    printf("=== Step %d\n", (step + 1));
    memset(pair_freqs_delta, 0, sizeof(pair_freqs_delta));

    for (i32 idx_rule = 0; idx_rule < n_rules; idx_rule++) {
      i32 first = rules_from[idx_rule][0] - 'A';
      i32 last = rules_from[idx_rule][1] - 'A';
      i32 via = rules_to[idx_rule] - 'A';
      u64 n_matches = pair_freqs[first][last];
      if (n_matches == 0) {
        continue;
      }
      printf("%c%c -> %c\n", rules_from[idx_rule][0], rules_from[idx_rule][1], rules_to[idx_rule]);
      pair_freqs_delta[first][via] += n_matches;
      pair_freqs_delta[via][last] += n_matches;
      pair_freqs_delta[first][last] -= n_matches;
    }

    for (i32 i = 0; i < N_CHARS; i++) {
      for (i32 j = 0; j < N_CHARS; j++) {
        pair_freqs[i][j] += pair_freqs_delta[i][j];
      }
    }

    print_pair_freqs();

    step++;
  }

  print_most_and_least_common();

  return 0;
}
