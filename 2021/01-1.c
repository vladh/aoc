#include <stdio.h>
#include <stdbool.h>
#include "common.h"


int main() {
  i32 last_number = -1;
  i32 current_number = -1;
  i32 n_increases = 0;

  FILE *f = fopen("data/1", "r");

  while (true) {
    last_number = current_number;
    if (fscanf(f, "%d", &current_number) <= 0) {
      break;
    }
    if (last_number > 0 && current_number > 0 && current_number > last_number) {
      n_increases += 1;
    }
  }

  printf("%d\n", n_increases);

  return 0;
}
