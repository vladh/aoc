#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "common.h"


#define GRID_SIZE 10
i8 grid[GRID_SIZE][GRID_SIZE] = {};
b8 flashes[GRID_SIZE][GRID_SIZE] = {};
i64 n_flashes = 0;
#define N_STEPS 1000


void increase_surrounding(i32 row, i32 col);


bool flash(i32 row, i32 col) {
  if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE) {
    return false;
  }
  if (flashes[row][col]) {
    return false;
  }
  flashes[row][col] = true;
  n_flashes++;
  increase_surrounding(row, col);
  return true;
}


void increase(i32 row, i32 col) {
  if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE) {
    return;
  }
  grid[row][col]++;
  if (grid[row][col] > 9) {
    flash(row, col);
  }
}


void increase_surrounding(i32 row, i32 col) {
  increase(row - 1, col - 1);
  increase(row - 1, col);
  increase(row - 1, col + 1);
  increase(row, col - 1);
  increase(row, col + 1);
  increase(row + 1, col - 1);
  increase(row + 1, col);
  increase(row + 1, col + 1);
}


int main() {
  FILE *f = fopen("data/11", "r");

  for (i32 row = 0; row < GRID_SIZE; row++) {
    for (i32 col = 0; col < GRID_SIZE; col++) {
      fscanf(f, "%1hhd", &grid[row][col]);
    }
  }

  i32 curr_step = 0;
  bool did_all_flash;

  while (curr_step < N_STEPS) {
    memset(flashes, 0, sizeof(flashes));

    for (i32 row = 0; row < GRID_SIZE; row++) {
      for (i32 col = 0; col < GRID_SIZE; col++) {
        grid[row][col]++;
      }
    }

    bool did_flash = true;
    while (did_flash) {
      did_flash = false;
      for (i32 row = 0; row < GRID_SIZE; row++) {
        for (i32 col = 0; col < GRID_SIZE; col++) {
          if (grid[row][col] > 9) {
            did_flash = flash(row, col);
          }
        }
      }
    }

    for (i32 row = 0; row < GRID_SIZE; row++) {
      for (i32 col = 0; col < GRID_SIZE; col++) {
        if (flashes[row][col]) {
          grid[row][col] = 0;
        }
      }
    }

    did_all_flash = true;
    for (i32 row = 0; row < GRID_SIZE; row++) {
      for (i32 col = 0; col < GRID_SIZE; col++) {
        if (!flashes[row][col]) {
          did_all_flash = false;
        }
      }
    }

    #if 0
    for (i32 row = 0; row < GRID_SIZE; row++) {
      for (i32 col = 0; col < GRID_SIZE; col++) {
        printf("%d", grid[row][col]);
      }
      printf("\n");
    }
    printf("\n");

    usleep(50 * 1000);
    #endif

    if (did_all_flash) {
      break;
    }

    curr_step++;
  }

  printf("step %d\n", curr_step);
  printf("did_all_flash: %d\n", did_all_flash);
  printf("n_flashes: %ld\n", n_flashes);

  return 0;
}
