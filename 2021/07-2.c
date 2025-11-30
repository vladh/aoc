#include <stdio.h>
#include <stdlib.h>
#include "common.h"


#define MAX_N_NUMBERS 1000
#define MAX_NUMBER 2000
i32 numbers[MAX_N_NUMBERS] = {0};
i32 n_numbers = 0;
i32 delta_sums[MAX_NUMBER + 1] = {0};
i32 max_number = 0;
i32 min_delta_sum = 9999;


int main() {
  FILE *f = fopen("data/7", "r");

  i32 number = 0;
  while (fscanf(f, "%d,", &number) > 0) {
    numbers[n_numbers++] = number;
    if (number > max_number) {
      max_number = number;
    }
  }

  for (i32 candidate = 0; candidate <= max_number; candidate++) {
    for (i32 i = 0; i < n_numbers; i++) {
      i32 diff = abs(numbers[i] - candidate);
      delta_sums[candidate] += (diff * (diff + 1) * 0.5);
    }
  }

  for (i32 candidate = 0; candidate <= max_number; candidate++) {
    if (candidate == 0 || delta_sums[candidate] < min_delta_sum) {
      min_delta_sum = delta_sums[candidate];
    }
  }

  printf("%d\n", min_delta_sum);

  return 0;
}
