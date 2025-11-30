#include <stdio.h>
#include "common.h"


#define N_ROWS 100
#define N_COLS 100
i8 heightmap[N_ROWS][N_COLS] = {-1};


int main() {
  FILE *f = fopen("data/9", "r");
  for (i32 row = 0; row < N_ROWS; row++) {
    for (i32 col = 0; col < N_COLS; col++) {
      fscanf(f, "%1hhd", &heightmap[row][col]);
    }
  }

  i32 risk_sum = 0;

  for (i32 row = 0; row < N_ROWS; row++) {
    for (i32 col = 0; col < N_COLS; col++) {
      if (
        (row == 0 || heightmap[row][col] < heightmap[row - 1][col]) &&
        (row == N_ROWS - 1 || heightmap[row][col] < heightmap[row + 1][col]) &&
        (col == 0 || heightmap[row][col] < heightmap[row][col - 1]) &&
        (col == N_COLS - 1 || heightmap[row][col] < heightmap[row][col + 1])
      ) {
        risk_sum += heightmap[row][col] + 1;
      }
    }
  }

  printf("%d\n", risk_sum);
}
