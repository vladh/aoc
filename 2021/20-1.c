#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "common.h"


#define INPUT_GRID_SIZE 100
#define FILENAME "data/20"
/* #define INPUT_GRID_SIZE 5 */
/* #define FILENAME "data/20-test" */

#define N_ENHANCES 50
#define MAX_GRID_SIZE 2000
#define GRID_OFFSET (MAX_GRID_SIZE / 8 * 3)
u8 grid[MAX_GRID_SIZE][MAX_GRID_SIZE] = {};
u8 next_grid[MAX_GRID_SIZE][MAX_GRID_SIZE] = {};
#define CONVMATRIX_SIZE 512
u8 convmatrix[CONVMATRIX_SIZE] = {};
u8 infinity_state = 0;


void print_grid(u8 grid[MAX_GRID_SIZE][MAX_GRID_SIZE], i32 min_row, i32 min_col, i32 max_row, i32 max_col) {
  for (i32 row = min_row; row <= max_row; row++) {
    for (i32 col = min_col; col <= max_col; col++) {
      if (grid[row][col] == 1) {
        printf("#");
      } else {
        printf(".");
      }
    }
    printf("\n");
  }
  printf("\n");
}


void read_convmatrix(FILE *f) {
  char c = '\0';
  i32 idx = 0;
  while (true) {
    c = fgetc(f);
    if (c == '\n') {
      break;
    }
    if (c == '#') {
      convmatrix[idx] = 1;
    }
    idx++;
  }
  c = fgetc(f);
  assert(c == '\n');
}


void read_grid(FILE *f) {
  char c = '\0';
  for (i32 row = 0; row < INPUT_GRID_SIZE; row++) {
    for (i32 col = 0; col < INPUT_GRID_SIZE; col++) {
      c = fgetc(f);
      if (c == '#') {
        i32 full_row = GRID_OFFSET + row;
        i32 full_col = GRID_OFFSET + col;
        grid[full_row][full_col] = 1;
      }
    }
    c = fgetc(f);
    assert(c == '\n');
  }
}


i32 make_binary_number(u8 bits[9]) {
  i32 number = 0;
  for (i32 i = 0; i < 9; i++) {
    if (bits[i] == 1) {
      number += pow(2.0f, 8 - i);
    }
  }
  return number;
}


u8 get(i32 row, i32 col) {
  if (row < 0 || row >= MAX_GRID_SIZE || col < 0 || col >= MAX_GRID_SIZE) {
    return infinity_state;
  }
  return grid[row][col];
}


void convolve() {
  memset(next_grid, 0, sizeof(next_grid));
  for (i32 row = 0; row < MAX_GRID_SIZE; row++) {
    for (i32 col = 0; col < MAX_GRID_SIZE; col++) {
      u8 bits[9] = {
        get(row - 1, col - 1), get(row - 1, col    ), get(row - 1, col + 1),
        get(row    , col - 1), get(row    , col    ), get(row    , col + 1),
        get(row + 1, col - 1), get(row + 1, col    ), get(row + 1, col + 1),
      };
      i32 idx_convmatrix = make_binary_number(bits);
      if (convmatrix[idx_convmatrix] == 1) {
        next_grid[row][col] = 1;
      }
    }
  }
  if (convmatrix[0] == 1) {
    infinity_state = (infinity_state == 0) ? 1 : 0;
  }
  memcpy(grid, next_grid, sizeof(grid));
}


void print_corner() {
  for (i32 i = 0; i < 20; i++) {
    for (i32 j = 0; j < 20; j++) {
      if (grid[i][j]) {
        printf("#");
      } else {
        printf(".");
      }
    }
    printf("\n");
  }
  printf("\n");
}


i32 count_pixels() {
  i32 sum = 0;
  for (i32 row = 0; row < MAX_GRID_SIZE; row++) {
    for (i32 col = 0; col < MAX_GRID_SIZE; col++) {
      if (grid[row][col] == 1) {
        sum++;
      }
    }
  }
  return sum;
}


int main() {
  FILE *f = fopen(FILENAME, "r");
  read_convmatrix(f);
  read_grid(f);

  printf("Original grid:\n");
  print_grid(grid, GRID_OFFSET - 10, GRID_OFFSET - 10, GRID_OFFSET + 110, GRID_OFFSET + 110);
  print_corner();

  for (i32 idx_enhance = 0; idx_enhance < N_ENHANCES; idx_enhance++) {
    printf("Convolution %d:\n", idx_enhance + 1);
    convolve();
    print_grid(grid, GRID_OFFSET - 10, GRID_OFFSET - 10, GRID_OFFSET + 110, GRID_OFFSET + 110);
    print_corner();
  }

  printf("Sum: %d\n", count_pixels());

  return 0;
}
