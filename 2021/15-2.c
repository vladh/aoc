#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "common.h"


/* #define DATA_FILE "data/15-test" */
/* #define COSTS_GRID_SIZE 10 */
#define DATA_FILE "data/15"
#define COSTS_GRID_SIZE 100
#define GRID_SIZE (COSTS_GRID_SIZE * 5)

i64 costs[COSTS_GRID_SIZE][COSTS_GRID_SIZE] = {};
i64 dist[GRID_SIZE][GRID_SIZE] = {};
b8 visited[GRID_SIZE][GRID_SIZE] = {};

typedef struct {
  i32 x;
  i32 y;
} Point;
bool point_eq(Point a, Point b) { return a.x == b.x && a.y == b.y; }


i64 get_cost(i32 x, i32 y) {
  i32 x_offset = x / COSTS_GRID_SIZE;
  i32 y_offset = y / COSTS_GRID_SIZE;
  i32 cost = costs[x % COSTS_GRID_SIZE][y % COSTS_GRID_SIZE] + x_offset + y_offset;
  while (cost > 9) {
    cost -= 9;
  }
  return cost;
}


void update_tentative(Point origin, Point tentative) {
  if (
    tentative.x < 0 || tentative.x >= GRID_SIZE ||
    tentative.y < 0 || tentative.y >= GRID_SIZE ||
    visited[tentative.x][tentative.y]
  ) {
    return;
  }
  dist[tentative.x][tentative.y] = min(
    dist[tentative.x][tentative.y],
    dist[origin.x][origin.y] + get_cost(tentative.x, tentative.y)
  );
}


Point get_lowest_dist_unvisited_point() {
  i64 min_dist = LONG_MAX;
  Point res = {-1, -1};
  for (i32 row = 0; row < GRID_SIZE; row++) {
    for (i32 col = 0; col < GRID_SIZE; col++) {
      if (dist[row][col] < min_dist && !visited[row][col]) {
        min_dist = dist[row][col];
        res.x = row;
        res.y = col;
      }
    }
  }
  return res;
}


void print_visited() {
  // Clear terminal
  printf("\e[1;1H\e[2J");

  for (i32 row = 0; row < GRID_SIZE; row++) {
    for (i32 col = 0; col < GRID_SIZE; col++) {
      if (visited[row][col]) {
        printf("#");
      } else {
        printf(".");
      }
    }
    printf("\n");
  }
  printf("\n");

  // Sleep so we can see each step a bit
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = 2 * 1000 * 1000;
  nanosleep(&ts, NULL);
}


void print_costs() {
  for (i32 row = 0; row < GRID_SIZE; row++) {
    for (i32 col = 0; col < GRID_SIZE; col++) {
      printf("%ld", get_cost(row, col));
    }
    printf("\n");
  }
}


int main() {
  FILE *f = fopen(DATA_FILE, "r");

  for (i32 row = 0; row < COSTS_GRID_SIZE; row++) {
    for (i32 col = 0; col < COSTS_GRID_SIZE; col++) {
      fscanf(f, "%1ld", &costs[row][col]);
    }
  }

  /* print_costs(); */

  memset(dist, 1, sizeof(dist));
  dist[0][0] = 0;
  Point dest = {GRID_SIZE - 1, GRID_SIZE - 1};

  while (true) {
    Point curr = get_lowest_dist_unvisited_point();
    printf("%d, %d\n", curr.x, curr.y);
    visited[curr.x][curr.y] = true;
    /* print_visited(); */

    update_tentative(curr, (Point){curr.x - 1, curr.y});
    update_tentative(curr, (Point){curr.x + 1, curr.y});
    update_tentative(curr, (Point){curr.x, curr.y - 1});
    update_tentative(curr, (Point){curr.x, curr.y + 1});

    if (point_eq(curr, dest)) {
      break;
    }
  }

  printf("%ld\n", dist[dest.x][dest.y]);

  return 0;
}
