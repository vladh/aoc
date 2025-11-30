#include <stdio.h>
#include <stdlib.h>
#include "common.h"


#define N_FILE_ROWS 100
#define N_FILE_COLS 100
#define N_ROWS 100
#define N_COLS 100
/* #define N_ROWS 5 */
/* #define N_COLS 10 */
#define N_MAX_LOW_POINTS 500
i32 heightmap[N_FILE_ROWS][N_FILE_COLS] = {};
i32 basinmap[N_ROWS][N_COLS] = {};


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


void fill_basin(i32 id, i32 row, i32 col) {
  if (
    row < 0 ||
    col < 0 ||
    row >= N_ROWS ||
    col >= N_COLS ||
    heightmap[row][col] == 9
  ) {
    return;
  }
  if (basinmap[row][col] != 0) {
    if (basinmap[row][col] != id) {
      printf("WARNING: id clash, got %d\n", basinmap[row][col]);
      basinmap[row][col] = -1;
    }
    return;
  }
  basinmap[row][col] = id;
  fill_basin(id, row - 1, col);
  fill_basin(id, row + 1, col);
  fill_basin(id, row, col - 1);
  fill_basin(id, row, col + 1);
}


int main() {
  FILE *f = fopen("data/9", "r");
  /* FILE *f = fopen("data/9-test", "r"); */
  for (i32 row = 0; row < N_FILE_ROWS; row++) {
    for (i32 col = 0; col < N_FILE_COLS; col++) {
      fscanf(f, "%1d", &heightmap[row][col]);
    }
  }

  i32 n_low_points = 0;
  i32 low_point_rows[N_MAX_LOW_POINTS] = {};
  i32 low_point_cols[N_MAX_LOW_POINTS] = {};

  for (i32 row = 0; row < N_ROWS; row++) {
    for (i32 col = 0; col < N_COLS; col++) {
      if (
        (row == 0 || heightmap[row][col] < heightmap[row - 1][col]) &&
        (row == N_ROWS - 1 || heightmap[row][col] < heightmap[row + 1][col]) &&
        (col == 0 || heightmap[row][col] < heightmap[row][col - 1]) &&
        (col == N_COLS - 1 || heightmap[row][col] < heightmap[row][col + 1])
      ) {
        low_point_rows[n_low_points] = row;
        low_point_cols[n_low_points] = col;
        n_low_points++;
      }
    }
  }

  for (i32 i = 0; i < n_low_points; i++) {
    fill_basin(i + 1, low_point_rows[i], low_point_cols[i]);
  }

  i32 basin_sums[N_MAX_LOW_POINTS] = {};
  for (i32 row = 0; row < N_ROWS; row++) {
    for (i32 col = 0; col < N_COLS; col++) {
      if (basinmap[row][col] > 0) {
        basin_sums[basinmap[row][col] - 1]++;
      }
    }
  }

  qsort(basin_sums, n_low_points, sizeof(i32), compare);

  i32 answer = basin_sums[n_low_points - 1] *
    basin_sums[n_low_points - 2] *
    basin_sums[n_low_points - 3];
  printf("answer: %d\n", answer);
}
