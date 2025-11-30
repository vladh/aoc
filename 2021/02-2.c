#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "common.h"


int main() {
  i32 aim = 0;
  i32 x = 0;
  i32 y = 0;
  char instruction[16];

  FILE *f = fopen("data/2", "r");

  while (fscanf(f, "%s", instruction) > 0) {
    i32 amount = -1;
    assert(fscanf(f, "%d", &amount) > 0);

    if (strcmp(instruction, "forward") == 0) {
      x += amount;
      y += aim * amount;
    } else if (strcmp(instruction, "up") == 0) {
      aim -= amount;
    } else if (strcmp(instruction, "down") == 0) {
      aim += amount;
    }
  };

  printf("x %d * y %d = %d\n", x, y, x * y);

  return 0;
};
