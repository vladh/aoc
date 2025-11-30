#include <stdio.h>
#include "common.h"


#define TIMER_START 8
#define TIMER_RESET 6
#define TARGET_DAY 256

i64 counters[TIMER_START + 1];


int main() {
  FILE *f = fopen("data/6", "r");

  i32 number = 0;
  while (fscanf(f, "%d,", &number) > 0) {
    counters[number] += 1;
  }

  for (i64 day = 1; day <= TARGET_DAY; day++) {
    i64 sum = 0;
    for (i64 i = 0; i <= TIMER_START; i++) {
      sum += counters[i];
    }

    i64 n_spawners = counters[0];
    for (i64 i = 0; i < TIMER_START; i++) {
      counters[i] = counters[i + 1];
    }
    counters[TIMER_START] = n_spawners; // new fish
    counters[TIMER_RESET] += n_spawners; // reset spawners
  }

  i64 sum = 0;
  for (i64 i = 0; i <= TIMER_START; i++) {
    sum += counters[i];
  }

  printf("%ld\n", sum);
}
