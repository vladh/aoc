#include <stdio.h>
#include <math.h>
#include "common.h"


#define N_BITS 12
#define N_ROWS 1000
/* #define N_BITS 5 */
/* #define N_ROWS 12 */


int main() {
  // oxy_bits and co2_bits are initially counts of how many times 1 occurs in
  // that specific position. At the end of the loop, each bit is set to an
  // actual 0 or 1 based on the count.
  u32 oxy_bits[N_BITS] = {0};
  u32 co2_bits[N_BITS] = {0};
  FILE *f = fopen("data/3", "r");
  /* FILE *f = fopen("data/3-test", "r"); */

  for (i32 i = 0; i < N_BITS; i++) {
    rewind(f);
    char current = '\0';
    i32 col = 0;
    i32 n_oxy_filtered_rows = 0;
    i32 n_co2_filtered_rows = 0;
    bool passed_oxy_filter = true;
    bool passed_co2_filter = true;
    while (true) {
      current = (char)fgetc(f);
      if (current == '\n') {
        col = 0;
        passed_oxy_filter = true;
        passed_co2_filter = true;
        continue;
      }
      if (current == EOF) {
        break;
      }

      if (col < i) {
        if ((current - '0') != oxy_bits[col]) {
          passed_oxy_filter = false;
        }
        if ((current - '0') != co2_bits[col]) {
          passed_co2_filter = false;
        }
      }

      if (col == i) {
        if (passed_oxy_filter) {
          n_oxy_filtered_rows += 1;
          if (current == '1') {
            oxy_bits[col] += 1;
          }
        }
        if (passed_co2_filter) {
          n_co2_filtered_rows += 1;
          if (current == '1') {
            co2_bits[col] += 1;
          }
        }
      }

      col += 1;
    };

    if (n_oxy_filtered_rows == 1) {
      // Do nothing, count will be 1 or 0, leave it at that.
    } else {
      if (oxy_bits[i] >= (f32)n_oxy_filtered_rows / 2) {
        oxy_bits[i] = 1;
      } else {
        oxy_bits[i] = 0;
      }
    }

    if (n_co2_filtered_rows == 1) {
      // Do nothing, count will be 1 or 0, leave it at that.
    } else {
      if (co2_bits[i] < (f32)n_co2_filtered_rows / 2) {
        co2_bits[i] = 1;
      } else {
        co2_bits[i] = 0;
      }
    }
  }

  u32 oxy = 0;
  for (i32 i = 0; i < N_BITS; i++) {
    oxy += oxy_bits[N_BITS - 1 - i] * pow(2, i);
  }

  u32 co2 = 0;
  for (i32 i = 0; i < N_BITS; i++) {
    co2 += co2_bits[N_BITS - 1 - i] * pow(2, i);
  }

  printf("%u * %u = %u\n", oxy, co2, oxy * co2);

  return 0;
}
