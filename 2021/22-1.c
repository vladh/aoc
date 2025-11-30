#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "common.h"


typedef struct Volume {
  i8 sign;
  i64 x0;
  i64 x1;
  i64 y0;
  i64 y1;
  i64 z0;
  i64 z1;
} Volume;

#define MAX_N_VOLUMES 100000


bool isincore(i64 p) {
  return p >= -50 && p <= 50;
}


int main() {
  Volume volumes[MAX_N_VOLUMES] = {};
  i32 n_volumes = 0;
  FILE *f = fopen("data/22", "r");

  // Cheated this one a little bit, the idea was inspired by /u/Boojum on Reddit
  // It does mean I didn't waste a lot of time splitting cubes, so I'll take it
  char on_or_off[4];
  i64 x0, x1, y0, y1, z0, z1;
  while (fscanf(f, "%s x=%ld..%ld,y=%ld..%ld,z=%ld..%ld", on_or_off, &x0, &x1, &y0, &y1, &z0, &z1) > 0) {
    i8 sign = 1;
    if (strcmp(on_or_off, "off") == 0) {
      sign = -1;
    }
    Volume newvol = {sign, x0, x1, y0, y1, z0, z1};

    i32 orig_n_volumes = n_volumes;
    for (i32 idx_volume = 0; idx_volume < orig_n_volumes; idx_volume++) {
      Volume *oldvol = &volumes[idx_volume];
      i64 intersect_x0 = max(newvol.x0, oldvol->x0);
      i64 intersect_x1 = min(newvol.x1, oldvol->x1);
      i64 intersect_y0 = max(newvol.y0, oldvol->y0);
      i64 intersect_y1 = min(newvol.y1, oldvol->y1);
      i64 intersect_z0 = max(newvol.z0, oldvol->z0);
      i64 intersect_z1 = min(newvol.z1, oldvol->z1);
      if (intersect_x0 <= intersect_x1 && intersect_y0 <= intersect_y1 && intersect_z0 <= intersect_z1) {
        Volume intersect_vol = {
          -oldvol->sign, intersect_x0, intersect_x1, intersect_y0, intersect_y1, intersect_z0, intersect_z1
        };
        memcpy(&volumes[n_volumes++], &intersect_vol, sizeof(Volume));
        assert(n_volumes < MAX_N_VOLUMES);
      }
    }
    if (newvol.sign == 1) {
      memcpy(&volumes[n_volumes++], &newvol, sizeof(Volume));
      assert(n_volumes < MAX_N_VOLUMES);
    }
  }

  u64 full_sum = 0;
  u64 partial_sum = 0;
  for (i32 idx_volume = 0; idx_volume < n_volumes; idx_volume++) {
    Volume *vol = &volumes[idx_volume];
    full_sum += (vol->x1 - vol->x0 + 1) * (vol->y1 - vol->y0 + 1) * (vol->z1 - vol->z0 + 1) * vol->sign;

    // What's the better way of doing this?
    if (
      (isincore(vol->x0) || isincore(vol->x1)) &&
      (isincore(vol->y0) || isincore(vol->y1)) &&
      (isincore(vol->z0) || isincore(vol->z1))
    ) {
      i64 clamped_x1 = min(vol->x1, 50);
      i64 clamped_x0 = max(vol->x0, -50);
      assert(clamped_x1 >= clamped_x0);
      i64 clamped_y1 = min(vol->y1, 50);
      i64 clamped_y0 = max(vol->y0, -50);
      assert(clamped_y1 >= clamped_y0);
      i64 clamped_z1 = min(vol->z1, 50);
      i64 clamped_z0 = max(vol->z0, -50);
      assert(clamped_z1 >= clamped_z0);
      partial_sum += (clamped_x1 - clamped_x0 + 1) *
        (clamped_y1 - clamped_y0 + 1) *
        (clamped_z1 - clamped_z0 + 1) * vol->sign;
    }
  }

  printf("partial sum: %19lu\n", partial_sum);
  printf("full sum:    %19lu\n", full_sum);

  return 0;
}
