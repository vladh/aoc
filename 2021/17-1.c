#include <stdio.h>
#include <stdlib.h>
#include "common.h"


#define MAX_N_HITS 1024
#define MIN_VX -80
#define MAX_VX 80
#define MIN_VY -80
#define MAX_VY 80


i32 sign(i32 n) {
  if (n == 0) {
    return 0;
  } else if (n < 0) {
    return -1;
  } else {
    return 1;
  }
}


typedef struct v2 {
  i32 x;
  i32 y;
} v2;


void step(v2 *pos, v2 *velocity) {
  pos->x = pos->x + velocity->x;
  pos->y = pos->y + velocity->y;

  velocity->x += -sign(velocity->x);
  velocity->y -= 1;
}


bool did_hit(v2 pos, v2 target_0, v2 target_1) {
  return pos.x >= target_0.x && pos.x <= target_1.x &&
    pos.y >= target_0.y && pos.y <= target_1.y;
}


bool did_pass(v2 pos, v2 target_0, v2 target_1) {
  return (
    (
      abs(pos.x) > abs(target_1.x) &&
      sign(pos.x) == sign(target_1.x)
    ) || (
      abs(pos.y) > abs(target_1.y) &&
      sign(pos.y) == sign(target_1.y)
    )
  );
}


int main() {
  FILE *f = fopen("data/17-test", "r");

  v2 target_0;
  v2 target_1;
  fscanf(f, "target area: x=%d..%d, y=%d..%d",
    &target_0.x, &target_1.x, &target_0.y, &target_1.y);

  i32 hit_max_y = -99999;

  for (i32 vx = MIN_VX; vx < MAX_VX; vx++) {
    for (i32 vy = MIN_VY; vy < MAX_VY; vy++) {
      v2 pos = {0, 0};
      v2 velocity = {vx, vy};
      i32 local_max_y = -99999;
      printf("Trying velocity (%d, %d)\n", vx, vy);

      printf("pos: (%d, %d)\n", pos.x, pos.y);
      while (!did_hit(pos, target_0, target_1) && !did_pass(pos, target_0, target_1)) {
        step(&pos, &velocity);
        if (pos.y > local_max_y) {
          local_max_y = pos.y;
        }
        printf("pos: (%d, %d)\n", pos.x, pos.y);
      }
      if (did_hit(pos, target_0, target_1)) {
        printf("hit\n");
        if (local_max_y > hit_max_y) {
          hit_max_y = local_max_y;
        }
      }
      if (did_pass(pos, target_0, target_1)) {
        printf("passed\n");
        // Forgot the break? lmao
        /* break; */
      }
      printf("\n");
    }
  }

  printf("hit_max_y: %d\n", hit_max_y);

  return 0;
}
