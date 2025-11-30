#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include "external/hashmap.h"
#include "external/hashmap.c"
#include "common.h"


typedef enum Space {
  EMPTY = 0,
  A     = 1,
  B     = 2,
  C     = 3,
  D     = 4,
} Space;

typedef struct State {
  struct State *prev;
  i32 cost;
  Space corridor[11];
  Space cols[4][4];
} State;

i32 const COL_FOR_SPACE[5] = {-1, 0, 1, 2, 3};
i32 const COLUMN_POSITIONS[4] = {2, 4, 6, 8};
i32 const COSTS[5] = {0, 1, 10, 100, 1000};
char const SPACE_CHARS[5] = {'.', 'A', 'B', 'C', 'D'};
Space const SPACE_FOR_COL[4] = {A, B, C, D};

#define MAX_N_GLOBAL_MOVES 50000
State *global_moves[MAX_N_GLOBAL_MOVES] = {};

State best_finish = {};

struct hashmap *map;


i32 sign(i32 n) {
  if (n == 0) {
    return 0;
  } else if (n < 0) {
    return -1;
  } else {
    return 1;
  }
}


int state_compare(const void *a, const void *b, void *udata) {
  const State *sa = a;
  const State *sb = b;
  return memcmp(&sa->corridor, &sb->corridor, sizeof(Space) * 27);
}


u64 state_hash(const void *item, u64 seed0, u64 seed1) {
  const State *state = item;
  return hashmap_sip(&state->corridor, sizeof(Space) * 27, seed0, seed1);
}


void state_print(State *state) {
  printf(
    "#############\n"
    "#%c%c%c%c%c%c%c%c%c%c%c#\n"
    "###%c#%c#%c#%c###\n"
    "  #%c#%c#%c#%c#  \n"
    "  #%c#%c#%c#%c#  \n"
    "  #%c#%c#%c#%c#  \n"
    "  #########  \n"
    "(cost %d)\n",

    SPACE_CHARS[state->corridor[0]],
    SPACE_CHARS[state->corridor[1]],
    SPACE_CHARS[state->corridor[2]],
    SPACE_CHARS[state->corridor[3]],
    SPACE_CHARS[state->corridor[4]],
    SPACE_CHARS[state->corridor[5]],
    SPACE_CHARS[state->corridor[6]],
    SPACE_CHARS[state->corridor[7]],
    SPACE_CHARS[state->corridor[8]],
    SPACE_CHARS[state->corridor[9]],
    SPACE_CHARS[state->corridor[10]],

    SPACE_CHARS[state->cols[0][0]],
    SPACE_CHARS[state->cols[1][0]],
    SPACE_CHARS[state->cols[2][0]],
    SPACE_CHARS[state->cols[3][0]],

    SPACE_CHARS[state->cols[0][1]],
    SPACE_CHARS[state->cols[1][1]],
    SPACE_CHARS[state->cols[2][1]],
    SPACE_CHARS[state->cols[3][1]],

    SPACE_CHARS[state->cols[0][2]],
    SPACE_CHARS[state->cols[1][2]],
    SPACE_CHARS[state->cols[2][2]],
    SPACE_CHARS[state->cols[3][2]],

    SPACE_CHARS[state->cols[0][3]],
    SPACE_CHARS[state->cols[1][3]],
    SPACE_CHARS[state->cols[2][3]],
    SPACE_CHARS[state->cols[3][3]],

    state->cost);
}


void state_print_all(State *state) {
  printf("-- full state printout -----------------\n");
  while (state != NULL) {
    state_print(state);
    printf("\n");
    assert(state != state->prev);
    state = state->prev;
  }
  printf("----------------------------------------\n");
}


bool is_column_position(i32 n) {
  return n == 2 || n == 4 || n == 6 || n == 8;
}


bool is_in_final_col(Space space, i32 col) {
  return (space == A && col == 0) ||
    (space == B && col == 1) ||
    (space == C && col == 2) ||
    (space == D && col == 3);
}


bool is_col_only_final(State *state, i32 col) {
  for (i32 colpos = 0; colpos < 4; colpos++) {
    if (!(state->cols[col][colpos] == EMPTY || is_in_final_col(state->cols[col][colpos], col))) {
      return false;
    }
  }
  return true;
}


void find_possible_corridor_moves(State *state, i32 col, i32 corpos[32], i32 *n_corpos) {
  *n_corpos = 0;
  i32 corpos_above = COLUMN_POSITIONS[col];

  for (i32 dir = -1; dir <= 1; dir += 2) {
    i32 offset = 1;
    i32 pos = corpos_above + (offset * dir);
    while (state->corridor[pos] == EMPTY) {
      if (pos < 0 || pos > 10) {
        break;
      }
      if (!is_column_position(pos)) {
        corpos[*n_corpos] = pos;
        (*n_corpos)++;
      }
      offset++;
      pos = corpos_above + (offset * dir);
    }
  }
}


bool have_path_to_col(State *state, i32 start, i32 dest) {
  i32 dir = sign(dest - start);
  assert(start != dest);
  for (i32 pos = start + dir; pos != dest; pos += dir) {
    if (state->corridor[pos] != EMPTY) {
      return false;
    }
  }
  return true;
}


bool is_state_complete(State *state) {
  for (i32 col = 0; col < 4; col++) {
    for (i32 row = 0; row < 4; row++) {
      if (state->cols[col][row] != SPACE_FOR_COL[col]) {
        return false;
      }
    }
  }
  return true;
}


void make_moves(State *state, State moves[128], i32 *n_moves) {
  *n_moves = 0;

  // Move from corridor to final column
  for (i32 corpos = 0; corpos < 11; corpos++) {
    Space moved_letter = state->corridor[corpos];
    if (moved_letter == EMPTY) {
      continue;
    }
    i32 target_col = COL_FOR_SPACE[moved_letter];
    assert(target_col >= 0);

    // Check if we can put this letter in its target column
    if (
      is_col_only_final(state, target_col) &&
      have_path_to_col(state, corpos, COLUMN_POSITIONS[target_col])
    ) {
      // We can!
      // Find how far down we have to move it into the column and down the corridor
      i32 idx_insert = 3;
      for (i32 idx_top = 0; idx_top < 4; idx_top++) {
        if (state->cols[target_col][idx_top] != EMPTY) {
          idx_insert = idx_top - 1;
          break;
        }
      }
      i32 corridor_dist = abs(corpos - COLUMN_POSITIONS[target_col]);

      // Create the move
      memcpy(&moves[*n_moves], state, sizeof(State));
      moves[*n_moves].prev = state;
      moves[*n_moves].corridor[corpos] = EMPTY;
      moves[*n_moves].cols[target_col][idx_insert] = moved_letter;
      moves[*n_moves].cost = state->cost + (
          // Moving left/right the correct amount
          corridor_dist +
          // Getting to the top of the column
          idx_insert +
          // Entering the column
          1
        ) * COSTS[moved_letter];
      (*n_moves)++;
    }
  }

  // Move top of column out
  for (i32 col = 0; col < 4; col++) {
    // Don't do anything if it's only full of final letters
    if (is_col_only_final(state, col)) {
      continue;
    }

    // Find its top --- if it's empty, go to the next column
    i32 idx_top = 0;
    while (true) {
      if (idx_top > 3) {
        goto next_col;
      }
      if (state->cols[col][idx_top] != EMPTY) {
        break;
      }
      idx_top++;
    }

    // Find the possible corridor positions where we can go
    Space moved_letter = state->cols[col][idx_top];
    i32 corpos[32];
    i32 corpos_costs[32];
    i32 n_corpos;
    find_possible_corridor_moves(state, col, corpos, &n_corpos);
    for (i32 idx_corpos = 0; idx_corpos < n_corpos; idx_corpos++) {
      corpos_costs[idx_corpos] = (
          // Moving left/right the correct amount
          abs(COLUMN_POSITIONS[col] - corpos[idx_corpos]) +
          // Getting to the top of the column
          idx_top +
          // Exiting the column
          1
        ) * COSTS[moved_letter];
    }

    // Create all the moves
    for (i32 idx_corpos = 0; idx_corpos < n_corpos; idx_corpos++) {
      memcpy(&moves[*n_moves], state, sizeof(State));
      moves[*n_moves].prev = state;
      moves[*n_moves].cost = state->cost + corpos_costs[idx_corpos];
      moves[*n_moves].cols[col][idx_top] = EMPTY;
      moves[*n_moves].corridor[corpos[idx_corpos]] = moved_letter;
      (*n_moves)++;
    }

next_col:
    ;
  }
}


State* walk(State *state) {
  while (true) {
    /* printf("\e[1;1H\e[2J"); */
    /* state_print(state); */
    /* printf("\n"); */

    if (is_state_complete(state)) {
      return state;
    } else {
      // Get new moves from current state
      // This leaks, whatever
      State *new_moves = (State*)calloc(128, sizeof(State));
      i32 n_new_moves;
      make_moves(state, new_moves, &n_new_moves);

      // Add each move to our global list of moves
      i32 idx_global_move = 0;
      for (i32 idx_new_move = 0; idx_new_move < n_new_moves; idx_new_move++) {
        // Only make this move if we haven't made it before, or if we have made it before but this version of move has a
        // lower cost
        State *alternative_move = hashmap_get(map, &new_moves[idx_new_move]);
        if (alternative_move != NULL && new_moves[idx_new_move].cost >= alternative_move->cost) {
          continue;
        }
        hashmap_set(map, &new_moves[idx_new_move]);

        // Find the next place to put the new move
        while (global_moves[idx_global_move] != NULL) {
          idx_global_move++;
          assert(idx_global_move < MAX_N_GLOBAL_MOVES);
        }

        // And add it
        global_moves[idx_global_move] = &new_moves[idx_new_move];
      }
    }

    // Get next state (the one with lowest cost)
    i32 lowest_cost = INT_MAX;
    i32 idx_lowest_cost = -1;
    for (i32 idx_global_move = 0; idx_global_move < MAX_N_GLOBAL_MOVES; idx_global_move++) {
      if (global_moves[idx_global_move] != NULL && global_moves[idx_global_move]->cost < lowest_cost) {
        lowest_cost = global_moves[idx_global_move]->cost;
        idx_lowest_cost = idx_global_move;
      }
    }
    if (!idx_lowest_cost == -1) {
      printf("Ran out of moves\n");
      break;
    }
    state = global_moves[idx_lowest_cost];
    global_moves[idx_lowest_cost] = NULL;
  }

  return NULL;
}


int main() {
  map = hashmap_new(sizeof(State), 0, 0, 0, state_hash, state_compare, NULL, NULL);

  State state = {
    .prev = NULL,
    .cost = 0,
    .corridor = {EMPTY},
    .cols = {
      {D, D, D, B},
      {A, C, B, A},
      {B, B, A, D},
      {C, A, C, C},
    },
  };

  State *end_state = walk(&state);
  assert(end_state != NULL);
  state_print_all(end_state);

  return 0;
}
