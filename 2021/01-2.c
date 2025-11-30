#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "common.h"


int main() {
  i32 window[3];
  i32 previous_sum = 0;
  i32 current_sum = 0;
  i32 n_increases = 0;

  FILE *f = fopen("data/1", "r");

  assert(fscanf(f, "%d", &window[0]) == 1);
  assert(fscanf(f, "%d", &window[1]) == 1);
  assert(fscanf(f, "%d", &window[2]) == 1);

  while (true) {
    previous_sum = window[0] + window[1] + window[2];

    window[0] = window[1];
    window[1] = window[2];

    if (fscanf(f, "%d", &window[2]) < 1) {
      break;
    }

    current_sum = window[0] + window[1] + window[2];

    if (current_sum > previous_sum) {
      n_increases += 1;
    }
  }

  printf("%d\n", n_increases);

  return 0;
}
