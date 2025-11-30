#include <stdio.h>
#include "common.h"


#define MAX_GRID_SIZE 1500
#define MAX_N_FOLDS 64
b8 grid[MAX_GRID_SIZE][MAX_GRID_SIZE];
i32 x_size = 0;
i32 y_size = 0;
i32 fold_pos[MAX_N_FOLDS] = {};
b8 is_fold_x[MAX_N_FOLDS] = {};


i32 count_dots() {
  i32 sum = 0;
  for (i32 ix = 0; ix < x_size; ix++) {
    for (i32 iy = 0; iy < y_size; iy++) {
      if (grid[ix][iy]) {
        sum++;
      }
    }
  }
  return sum;
}


void print_grid() {
  for (i32 iy = 0; iy < y_size; iy++) {
    for (i32 ix = 0; ix < x_size; ix++) {
      if (grid[ix][iy]) {
        printf("#");
      } else {
        printf(".");
      }
    }
    printf("\n");
  }
  printf("\n");
}


void fold_x(i32 fold_pos) {
  for (i32 d = 0; d < x_size - fold_pos; d++) {
    for (i32 iy = 0; iy < y_size; iy++) {
      if (grid[fold_pos + d][iy]) {
        grid[fold_pos - d][iy] = true;
      }
    }
  }
  x_size = fold_pos;
}


void fold_y(i32 fold_pos) {
  for (i32 d = 0; d < y_size - fold_pos; d++) {
    for (i32 ix = 0; ix < x_size; ix++) {
      if (grid[ix][fold_pos + d]) {
        grid[ix][fold_pos - d] = true;
      }
    }
  }
  y_size = fold_pos;
}


int main() {
  FILE *f = fopen("data/13", "r");

  i32 curr_x, curr_y;
  while (fscanf(f, "%d,", &curr_x) > 0) {
    fscanf(f, "%d", &curr_y);
    grid[curr_x][curr_y] = true;
    if (curr_x >= x_size) {
      x_size = curr_x + 1;
    }
    if (curr_y >= y_size) {
      y_size = curr_y + 1;
    }
    printf("%d, %d\n", curr_x, curr_y);
  }

  print_grid();

  char curr_fold_dir;
  i32 curr_fold_pos;
  fscanf(f, "fold along %c=%d", &curr_fold_dir, &curr_fold_pos);
  if (curr_fold_dir == 'x') {
    fold_x(curr_fold_pos);
  } else {
    fold_y(curr_fold_pos);
  }

  print_grid();

  printf("sum: %d\n", count_dots());

  return 0;
}
