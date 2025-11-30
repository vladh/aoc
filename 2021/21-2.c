#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "common.h"


#define BLUE "\033[0;34m"
#define RED "\033[0;31m"
#define WHITE "\033[0;37m"

i32 const start_boardpos[2] = {2, 8};
i32 const roll_probabilities[10] = {0, 0, 0, 1, 3, 6, 7, 6, 3, 1};
// The number of universes where player 1 has score i on board position j
// and player 2 has score k on board position l. [i][j][k][l]
u64 universes[22][11][22][11] = {};


bool do_non_winning_scores_exist() {
  for (i32 p1_score = 0; p1_score < 21; p1_score++) {
    for (i32 p1_boardpos = 1; p1_boardpos <= 10; p1_boardpos++) {
      for (i32 p2_score = 0; p2_score < 21; p2_score++) {
        for (i32 p2_boardpos = 1; p2_boardpos <= 10; p2_boardpos++) {
          if (universes[p1_score][p1_boardpos][p2_score][p2_boardpos] > 0) {
            return true;
          }
        }
      }
    }
  }
  return false;
}


i32 mod10(i32 n) {
  while (n > 10) {
    n -= 10;
  }
  return n;
}


void update_universes() {
  for (i32 idx_player = 0; idx_player <= 1; idx_player++) {
    u64 universes_old[22][11][22][11];
    memcpy(universes_old, universes, sizeof(universes));
    for (i32 p1_score = 0; p1_score < 21; p1_score++) {
      for (i32 p1_boardpos = 1; p1_boardpos <= 10; p1_boardpos++) {
        for (i32 p2_score = 0; p2_score < 21; p2_score++) {
          for (i32 p2_boardpos = 1; p2_boardpos <= 10; p2_boardpos++) {
            for (i32 roll = 3; roll <= 9; roll++) {
              i32 roll_probability = roll_probabilities[roll];
              if (idx_player == 0) {
                i32 new_boardpos = mod10(p1_boardpos + roll);
                i32 new_score = p1_score + new_boardpos;
                if (new_score > 21) {
                  new_score = 21;
                }
                universes[new_score][new_boardpos][p2_score][p2_boardpos] +=
                  roll_probability * universes_old[p1_score][p1_boardpos][p2_score][p2_boardpos];
              } else {
                i32 new_boardpos = mod10(p2_boardpos + roll);
                i32 new_score = p2_score + new_boardpos;
                if (new_score > 21) {
                  new_score = 21;
                }
                universes[p1_score][p1_boardpos][new_score][new_boardpos] +=
                  roll_probability * universes_old[p1_score][p1_boardpos][p2_score][p2_boardpos];
              }
            }
            universes[p1_score][p1_boardpos][p2_score][p2_boardpos] -=
              universes_old[p1_score][p1_boardpos][p2_score][p2_boardpos];
          }
        }
      }
    }
  }
}


void print_universes() {
  printf("     ");
  for (i32 i = 0; i <= 1; i++) {
    printf(RED"                       p%d"WHITE, i + 1);
  }
  printf("\n");
  for (i32 our_score = 0; our_score < 22; our_score++) {
    printf(RED"%5d"WHITE, our_score);
    for (i32 idx_player = 0; idx_player <= 1; idx_player++) {
      u64 sum = 0;
      for (i32 our_boardpos = 1; our_boardpos <= 10; our_boardpos++) {
        for (i32 their_score = 0; their_score < 22; their_score++) {
          for (i32 their_boardpos = 1; their_boardpos <= 10; their_boardpos++) {
            if (idx_player == 0) {
              sum += universes[our_score][our_boardpos][their_score][their_boardpos];
            } else {
              sum += universes[their_score][their_boardpos][our_score][our_boardpos];
            }
          }
        }
      }
      printf("%25ld", sum);
    }
    printf("\n");
  }
}


int main() {
  universes[0][start_boardpos[0]][0][start_boardpos[1]] = 1;
  print_universes();
  usleep(500 * 1000);

  i32 idx_step = 1;
  while (do_non_winning_scores_exist()) {
    printf(BLUE"\nStep %d\n"WHITE, idx_step);
    printf(BLUE"----------------------------------------------------\n"WHITE);
    update_universes();
    print_universes();
    idx_step++;
    usleep(500 * 1000);
  }

  return 0;
}
