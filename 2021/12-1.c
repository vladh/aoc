#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"


#define MAX_N_NODES 100
#define MAX_NAME_LEN 32
b8 adjmat[MAX_N_NODES][MAX_N_NODES] = {};
char node_names[MAX_N_NODES][MAX_NAME_LEN] = {};
b8 is_node_small[MAX_N_NODES] = {};
i32 n_nodes = 0;
i32 start_id = -1;
i32 end_id = -1;

// NOTE: It's not necessary to store all the walks, it would be more efficient
// to remember the visited nodes in a bitset passed using the walk() function.
#define MAX_N_WALKS 50000
i32 walks[MAX_N_WALKS][MAX_N_NODES] = {};
i32 walk_lengths[MAX_N_WALKS] = {};
i32 n_walks = 0;
i32 n_finished_walks = 0;


i32 get_node_name_idx_or_add(char *name) {
  for (i32 i = 0; i < n_nodes; i++) {
    if (strcmp(node_names[i], name) == 0) {
      return i;
    }
  }
  strcpy(node_names[n_nodes], name);
  n_nodes++;
  return n_nodes - 1;
}


void print_walk(i32 walk_id) {
  for (i32 i = 0; i < walk_lengths[walk_id]; i++) {
    printf("%s (%d)", node_names[walks[walk_id][i]], walks[walk_id][i]);
    if (i < walk_lengths[walk_id] - 1) {
      printf(" -> ");
    }
  }
  printf(".\n");
}


bool does_walk_have_node(i32 walk_id, i32 node_id) {
  for (i32 i = 0; i < walk_lengths[walk_id]; i++) {
    if (walks[walk_id][i] == node_id) {
      return true;
    }
  }
  return false;
}


void walk(i32 walk_id, i32 node_id) {
  if (walk_id >= MAX_N_WALKS - 1) {
    printf("WARNING: walk_id >= MAX_N_WALKS - 1\n");
    printf("walk %d of length %d\n", walk_id, walk_lengths[walk_id]);
    print_walk(walk_id);
    printf("\n");
    exit(-1);
  }

  if (is_node_small[node_id] && does_walk_have_node(walk_id, node_id)) {
    return;
  }

  walks[walk_id][walk_lengths[walk_id]++] = node_id;

  if (node_id == end_id) {
    print_walk(walk_id);
    n_finished_walks++;
    return;
  }

  for (i32 i = 0; i < n_nodes; i++) {
    if (adjmat[node_id][i]) {
      i32 new_walk_id = n_walks++;
      memcpy(walks[new_walk_id], walks[walk_id], sizeof(i32) * MAX_N_NODES);
      walk_lengths[new_walk_id] = walk_lengths[walk_id];
      walk(new_walk_id, i);
    }
  }
}


int main() {
  FILE *f = fopen("data/12", "r");

  char curr_src[MAX_NAME_LEN] = {};
  i32 curr_src_len = 0;
  char curr_dst[MAX_NAME_LEN] = {};
  i32 curr_dst_len = 0;
  char c;
  bool reading_src = true;

  while (c != EOF) {
    c = (char)fgetc(f);
    if (c == '-') {
      reading_src = false;
      continue;
    } else if (c == '\n') {
      reading_src = true;

      i32 src_id = get_node_name_idx_or_add(curr_src);
      i32 dst_id = get_node_name_idx_or_add(curr_dst);
      if (strcmp(curr_src, "start") == 0) {
        start_id = src_id;
      }
      if (strcmp(curr_dst, "end") == 0) {
        end_id = dst_id;
      }
      if (islower(curr_src[0])) {
        is_node_small[src_id] = true;
      }
      if (islower(curr_dst[0])) {
        is_node_small[dst_id] = true;
      }
      adjmat[src_id][dst_id] = true;
      adjmat[dst_id][src_id] = true;
      printf("%s (%d) -> %s (%d)\n", curr_src, src_id, curr_dst, dst_id);

      memset(curr_src, 0, sizeof(curr_src));
      curr_src_len = 0;
      memset(curr_dst, 0, sizeof(curr_dst));
      curr_dst_len = 0;

      continue;
    }
    if (reading_src) {
      curr_src[curr_src_len++] = c;
    } else {
      curr_dst[curr_dst_len++] = c;
    }
  }

  printf("\n");
  printf("start_id: %d\n", start_id);
  printf("end_id: %d\n", end_id);
  printf("small nodes: ");
  for (i32 i = 0; i < n_nodes; i++) {
    printf("%d ", is_node_small[i]);
  }
  printf("\n");
  printf("adjmat:\n");
  for (i32 i = 0; i < n_nodes; i++) {
    printf("  ");
    for (i32 j = 0; j < n_nodes; j++) {
      printf("%d ", adjmat[i][j]);
    }
    printf("\n");
  }

  printf("\n");

  walk(n_walks++, start_id);

  printf("n_walks: %d\n", n_walks);
  printf("n_finished_walks: %d\n", n_finished_walks);

  return 0;
}
