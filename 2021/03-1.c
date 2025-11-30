#include <stdio.h>
#include "common.h"


#define N_BITS 12


int main() {
  i32 n_rows = 0;
  u32 one_counts[N_BITS] = {0};
  FILE *f = fopen("data/3", "r");

  char current = '\0';
  i32 col = 0;
  while (current != EOF) {
    current = (char)fgetc(f);
    if (current == '\n') {
      col = 0;
      n_rows += 1;
    } else {
      if (current == '1') {
        one_counts[col] += 1;
      }
      col += 1;
    }
  };

  u32 gamma = 0;
  for (i32 i = 0; i < N_BITS; i++) {
    if (one_counts[i] >= n_rows / 2) {
      gamma = gamma | (1 << (N_BITS - 1 - i));
    }
  }
  u32 epsilon = ~gamma & ((1 << N_BITS) - 1);

  printf("%u * %u = %u\n", gamma, epsilon, gamma * epsilon);

  return 0;
}
