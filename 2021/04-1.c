#include <stdio.h>
#include "common.h"


#define MAX_N_BOARDS 256
#define MAX_N_DRAWS 512
#define BOARD_SIZE 5


typedef struct {
  i8 number;
  b8 is_marked;
} Tile;


i32 n_draws = 0;
i8 draws[MAX_N_DRAWS];
i32 n_boards = 0;
Tile boards[MAX_N_BOARDS][BOARD_SIZE][BOARD_SIZE];


i32 check_boards_for_win() {
  for (i32 idx_board = 0; idx_board < n_boards; idx_board++) {
    for (i32 i = 0; i < BOARD_SIZE; i++) {
      i32 n_marked_on_row = 0;
      i32 n_marked_on_column = 0;
      for (i32 j = 0; j < BOARD_SIZE; j++) {
        if (boards[idx_board][i][j].is_marked) {
          n_marked_on_row++;
        }
        if (boards[idx_board][j][i].is_marked) {
          n_marked_on_column++;
        }
      }
      if (n_marked_on_row == BOARD_SIZE || n_marked_on_column == BOARD_SIZE) {
        return idx_board;
      }
    }
  }
  return -1;
}


i32 get_board_unmarked_sum(i32 idx_board) {
  i32 sum = 0;
  for (i32 i = 0; i < BOARD_SIZE; i++) {
    for (i32 j = 0; j < BOARD_SIZE; j++) {
      Tile *tile = &boards[idx_board][i][j];
      if (!tile->is_marked) {
        sum += tile->number;
      }
    }
  }
  return sum;
}


int main() {
  i32 current = -1;
  i32 answer = -1;
  FILE *f = fopen("data/4", "r");

  // Read draws
  while (fscanf(f, "%d,", &current) > 0) {
    draws[n_draws++] = current;

    char c = fgetc(f);
    if (c == '\n') {
      fgetc(f);
      break;
    }
    ungetc(c, f);
  }

  // Read boards
  while (true) {
    for (i32 i = 0; i < BOARD_SIZE; i++) {
      for (i32 j = 0; j < BOARD_SIZE; j++) {
        if (fscanf(f, "%d", &current) < 1) {
          goto end_read_boards;
        }
        Tile *tile = &boards[n_boards][i][j];
        tile->number = current;
        tile->is_marked = false;
      }
    }
    n_boards++;
  }
end_read_boards:

  // Mark draws
  for (i32 idx_draw = 0; idx_draw < n_draws; idx_draw++) {
    i32 draw = draws[idx_draw];
    for (i32 idx_board = 0; idx_board < n_boards; idx_board++) {
      for (i32 i = 0; i < BOARD_SIZE; i++) {
        for (i32 j = 0; j < BOARD_SIZE; j++) {
          Tile *tile = &boards[idx_board][i][j];
          if (tile->number == draw) {
            tile->is_marked = true;
          }
        }
      }
    }

    i32 idx_winning_board = check_boards_for_win();
    if (idx_winning_board >= 0) {
      i32 winning_board_unmarked_sum = get_board_unmarked_sum(idx_winning_board);
      answer = winning_board_unmarked_sum * draw;
      printf("idx_winning_board: %d\n", idx_winning_board);
      printf("winning_board_unmarked_sum: %d\n", winning_board_unmarked_sum);
      printf("draw: %d\n", draw);
      break;
    }
  }

  printf("answer: %d\n", answer);

  return 0;
}
