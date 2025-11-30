#include <stdio.h>
#include "common.h"


#define GRID_SIZE 1000

u8 grid[GRID_SIZE][GRID_SIZE] = {0};


int main() {
  FILE *f = fopen("data/5", "r");
  u32 x0, y0, x1, y1;

  while (fscanf(f, "%u,%u -> %u,%u", &x0, &y0, &x1, &y1) > 0) {
    if ((x0 == x1) || (y0 == y1)) {
      if (x0 > x1) {
        u32 tmp = x0;
        x0 = x1;
        x1 = tmp;
      }
      if (y0 > y1) {
        u32 tmp = y0;
        y0 = y1;
        y1 = tmp;
      }
      for (i32 i = x0; i <= x1; i++) {
        for (i32 j = y0; j <= y1; j++) {
          grid[i][j] += 1;
        }
      }
    } else {
      u32 xstep = 1;
      u32 ystep = 1;
      if (x0 > x1) {
        xstep = -1;
      }
      if (y0 > y1) {
        ystep = -1;
      }
      for (i32 i = x0, j = y0; i != x1 + xstep, j != y1 + ystep; i += xstep, j += ystep) {
        grid[i][j] += 1;
      }
    }
  }

  u32 n_overlapping = 0;
  for (i32 i = 0; i < GRID_SIZE; i++) {
    for (i32 j = 0; j < GRID_SIZE; j++) {
      if (grid[i][j] >= 2) {
        n_overlapping++;
      }
    }
  }

  printf("%d\n", n_overlapping);

  return 0;
}
