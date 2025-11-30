#include <stdio.h>
#include <stdlib.h>
#include "common.h"


#define MAX_N_NUMBERS 1000
i32 numbers[MAX_N_NUMBERS] = {0};
i32 n_numbers = 0;


int compare(const void *a, const void *b) {
  i32 int_a = *((i32*)a);
  i32 int_b = *((i32*)b);

  if (int_a == int_b) {
    return 0;
  } else if (int_a < int_b) {
    return -1;
  } else {
    return 1;
  }
}


int main() {
  FILE *f = fopen("data/7", "r");

  i32 number = 0;
  while (fscanf(f, "%d,", &number) > 0) {
    numbers[n_numbers++] = number;
  }

  qsort(numbers, n_numbers, sizeof(i32), compare);

  i32 median = numbers[n_numbers / 2];

  i32 delta_sum = 0;
  for (i32 i = 0; i < n_numbers; i++) {
    delta_sum += abs(numbers[i] - median);
  }

  printf("%d\n", delta_sum);

  return 0;
}
