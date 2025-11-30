#include <assert.h>
#include <stdlib.h>
#include <string.h>
#define CGLM_DEFINE_PRINTS
#include <cglm/cglm.h>
#include "common.h"

typedef vec2 v2;
typedef vec3 v3;
typedef vec4 v4;
typedef mat2 m2;
typedef mat3 m3;
typedef mat4 m4;

#define MAX_N_SCANNERS 32
#define MAX_N_SCANNER_POINTS 32
#define MAX_N_TOTAL_POINTS 5000


v3 points[MAX_N_SCANNERS][MAX_N_SCANNER_POINTS] = {};
i32 n_scanners = 0;
i32 n_points[MAX_N_SCANNERS] = {};
m3 cob_matrices[24] = {}; // cob = change of basis
v3 global_points[MAX_N_TOTAL_POINTS] = {};
i32 n_global_points = 0;
bool matched_scanners[MAX_N_SCANNERS] = {};
i32 n_matched_scanners = 1;
v3 scanner_pos[MAX_N_SCANNERS] = {};


void make_cob_matrices(m3 cob_matrices[24]) {
  /* [ newForward.x  newForward.y  newForward.z ] */
  /* [ newUp.x       newUp.y       newUp.z      ] */
  /* [ newRight.x    newRight.y    newRight.z   ] */
  i32 idx_matrix = 0;
  for (i32 axis = 0; axis <= 2; axis++) {
    for (i32 axis_sign = -1; axis_sign <= 1; axis_sign += 2) {
      for (i32 up_axis = 1; up_axis <= 2; up_axis += 1) {
        for (i32 up_axis_sign = -1; up_axis_sign <= 1; up_axis_sign += 2) {
          // forward
          cob_matrices[idx_matrix][axis][0] = axis_sign;

          // up
          cob_matrices[idx_matrix][(axis + up_axis) % 3][1] = up_axis_sign;

          // right
          v3 forward = {};
          forward[0] = cob_matrices[idx_matrix][0][0];
          forward[1] = cob_matrices[idx_matrix][1][0];
          forward[2] = cob_matrices[idx_matrix][2][0];
          v3 up = {};
          up[0] = cob_matrices[idx_matrix][0][1];
          up[1] = cob_matrices[idx_matrix][1][1];
          up[2] = cob_matrices[idx_matrix][2][1];
          v3 right = {};
          glm_vec3_cross(forward, up, right);
          cob_matrices[idx_matrix][0][2] = right[0];
          cob_matrices[idx_matrix][1][2] = right[1];
          cob_matrices[idx_matrix][2][2] = right[2];

          idx_matrix++;
        }
      }
    }
  }
}


bool get_transform(i32 idx_scanner_b, i32 *idx_best_matrix, v3 best_delta) {
  printf("Finding transformation for scanner %d\n", idx_scanner_b);

  for (i32 idx_matrix = 0; idx_matrix < 24; idx_matrix++) {
    m3 *rotation_matrix = &cob_matrices[idx_matrix];

    // Rotate b points
    v3 rotated_b_points[MAX_N_SCANNER_POINTS] = {};
    for (i32 idx_b_point = 0; idx_b_point < n_points[idx_scanner_b]; idx_b_point++) {
      glm_mat3_mulv(*rotation_matrix, points[idx_scanner_b][idx_b_point], rotated_b_points[idx_b_point]);
    }

    // Calculate deltas
    v3 deltas[MAX_N_TOTAL_POINTS * MAX_N_SCANNER_POINTS] = {};
    i32 delta_counts[MAX_N_TOTAL_POINTS * MAX_N_SCANNER_POINTS] = {};
    i32 n_deltas = 0;

    for (i32 idx_a_point = 0; idx_a_point < n_global_points; idx_a_point++) {
      for (i32 idx_b_point = 0; idx_b_point < n_points[idx_scanner_b]; idx_b_point++) {
        v3 diff = {};
        glm_vec3_sub(rotated_b_points[idx_b_point], global_points[idx_a_point], diff);

        // Check if we already have a count for this delta
        for (i32 idx_delta = 0; idx_delta < n_deltas; idx_delta++) {
          if (glm_vec3_eqv_eps(deltas[idx_delta], diff)) {
            delta_counts[idx_delta]++;
            goto next_point;
          }
        }

        // Add a count for this delta
        glm_vec3_copy(diff, deltas[n_deltas]);
        delta_counts[n_deltas] = 1;
        n_deltas++;

next_point:
        ;
      }
    }

    // Find most common delta
    i32 idx_most_common_delta = -1;
    i32 biggest_delta_count = 0;
    for (i32 idx_unique_delta = 0; idx_unique_delta < n_deltas; idx_unique_delta++) {
      if (delta_counts[idx_unique_delta] > biggest_delta_count) {
        idx_most_common_delta = idx_unique_delta;
        biggest_delta_count = delta_counts[idx_unique_delta];
      }
    }

    // If there are 12 or more common points, use this as our transform
    if (biggest_delta_count >= 12) {
      printf("biggest_delta_count: %d\n", biggest_delta_count);
      *idx_best_matrix = idx_matrix;
      glm_vec3_copy(deltas[idx_most_common_delta], best_delta);
      return true;
    }
  }

  printf("could not match\n");
  return false;
}


void read_points(FILE *f) {
  while (true) {
    i32 idx_scanner;
    if (fscanf(f, "--- scanner %d ---\n", &idx_scanner) <= 0) {
      break;
    }
    n_scanners++;
    while (true) {
      i32 x;
      i32 y;
      if (
        fscanf(
          f, "%f,%f,%f",
          &points[n_scanners - 1][n_points[n_scanners - 1]][0],
          &points[n_scanners - 1][n_points[n_scanners - 1]][1],
          &points[n_scanners - 1][n_points[n_scanners - 1]][2]
        ) <= 0
      ) {
        break;
      }
      n_points[n_scanners - 1]++;

      char c;
      c = fgetc(f);
      assert(c == '\n');
      c = fgetc(f);
      if (c == '\n') {
        break;
      } else {
        ungetc(c, f);
      }
    }
  }
}


int main() {
  make_cob_matrices(cob_matrices);

  FILE *f = fopen("data/19", "r");

  read_points(f);

  for (i32 i = 0; i < n_points[0]; i++) {
    glm_vec3_copy(points[0][i], global_points[n_global_points++]);
  }

  while (true) {
    for (i32 idx_scanner_b = 1; idx_scanner_b < n_scanners; idx_scanner_b++) {
      if (matched_scanners[idx_scanner_b]) {
        continue;
      }

      i32 idx_best_matrix = -1;
      v3 best_delta = {};
      if (!get_transform(idx_scanner_b, &idx_best_matrix, best_delta)) {
        continue;
      }

      glm_vec3_copy(best_delta, scanner_pos[idx_scanner_b]);

      matched_scanners[idx_scanner_b] = true;
      n_matched_scanners++;
      printf("Matched %d/%d scanners\n", n_matched_scanners, n_scanners);

      i32 n_added = 0;
      for (i32 idx_point = 0; idx_point < n_points[idx_scanner_b]; idx_point++) {
        // Transform the point
        v3 transformed_point = {};
        glm_mat3_mulv(cob_matrices[idx_best_matrix], points[idx_scanner_b][idx_point], transformed_point);
        glm_vec3_sub(transformed_point, best_delta, transformed_point);

        // Check if we already have this point
        for (i32 idx_global = 0; idx_global < n_global_points; idx_global++) {
          if (glm_vec3_eqv_eps(transformed_point, global_points[idx_global])) {
            goto next_point;
          }
        }

        // If we don't already have it, add it
        glm_vec3_copy(transformed_point, global_points[n_global_points++]);
        n_added++;

    next_point:
        ;
      }

      printf("Added %d points\n", n_added);

      if (n_matched_scanners >= n_scanners) {
        goto end_search;
      }
    }
  }
end_search:
  ;

  printf("n_global_points: %d\n", n_global_points);

  i32 biggest_manhattan_dist = 0;
  for (i32 idx_a = 0; idx_a < n_scanners; idx_a++) {
    for (i32 idx_b = 0; idx_b < n_scanners; idx_b++) {
      i32 manhattan_dist = fabs(scanner_pos[idx_a][0] - scanner_pos[idx_b][0]) +
        fabs(scanner_pos[idx_a][1] - scanner_pos[idx_b][1]) +
        fabs(scanner_pos[idx_a][2] - scanner_pos[idx_b][2]);
      if (manhattan_dist > biggest_manhattan_dist) {
        biggest_manhattan_dist = manhattan_dist;
      }
    }
  }
  printf("biggest_manhattan_dist: %d\n", biggest_manhattan_dist);

  return 0;
}
