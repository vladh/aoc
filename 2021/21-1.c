#include <assert.h>
#include <stdio.h>
#include "common.h"


// start = 4
/* #define SPACE_SEQ_ODD_LEN 5 */
/* i32 SPACE_SEQ_ODD[SPACE_SEQ_ODD_LEN] = {10, 4, 6, 6, 4}; */

// start = 2
#define SPACE_SEQ_ODD_LEN 5
i32 SPACE_SEQ_ODD[SPACE_SEQ_ODD_LEN] = {8, 2, 4, 4, 2};

// start = 8
#define SPACE_SEQ_EVEN_LEN 10
i32 SPACE_SEQ_EVEN[SPACE_SEQ_EVEN_LEN] = {3,6,7,6,3,8,1,2,1,8};


i32 calc_seq_sum(i32 n, i32 a_0, i32 a_n) {
  return (f32)0.5 * (n + 1) * (a_0 + a_n);
}


i32 calc_6seq(i32 n) {
  // OEIS A017233
  return (n * 9) + 6;
}


i32 calc_6seq_sum(i32 n) {
  return calc_seq_sum(n, calc_6seq(0), calc_6seq(n));
}


i32 calc_6seq_odd(i32 n) {
  // OEIS A017233, just the odd terms
  return (n * 18) + 6;
}


i32 calc_6seq_odd_sum(i32 n) {
  return calc_seq_sum(n, calc_6seq_odd(0), calc_6seq_odd(n));
}


i32 calc_6seq_even(i32 n) {
  // OEIS A017233, just the even terms
  return (n * 18) + 15;
}


i32 calc_6seq_even_sum(i32 n) {
  return calc_seq_sum(n, calc_6seq_even(0), calc_6seq_even(n));
}


i32 calc_space_sum(i32 n, i32 const *seq, i32 seq_len) {
  i32 sum = 0;
  for (i32 i = 0; i < seq_len; i++) {
    sum += (n / seq_len) * seq[i];
    if (n % seq_len >= i) {
      sum += seq[i];
    }
  }
  return sum;
}


i32 calc_odd_space_sum(i32 n) {
  return calc_space_sum(n, &SPACE_SEQ_ODD[0], SPACE_SEQ_ODD_LEN);
}


i32 calc_even_space_sum(i32 n) {
  return calc_space_sum(n, &SPACE_SEQ_EVEN[0], SPACE_SEQ_EVEN_LEN);
}


i32 calc_odd_space_manual(i32 n, i32 start) {
  /* printf("calc_6seq_odd_sum(%d) = %d\n", n, calc_6seq_odd_sum(n)); */
  i32 res = start + calc_6seq_odd_sum(n);
  // This can probably be done better, whatever
  while (res > 10) {
    res -= 10;
  }
  return res;
}


i32 calc_even_space_manual(i32 n, i32 start) {
  /* printf("calc_6seq_odd_sum(%d) = %d\n", n, calc_6seq_odd_sum(n)); */
  i32 res = start + calc_6seq_even_sum(n);
  // This can probably be done better, whatever
  while (res > 10) {
    res -= 10;
  }
  return res;
}


i32 calc_score(i32 step) {
  if (step == 0) {
    return calc_6seq_sum(0);
  }
  return calc_6seq_sum(0);
}


void solve_space_sequence() {
  for (i32 i = 0; i < 50; i++) {
    printf("%d,", calc_odd_space_manual(i, 2));
  }
  printf("\n");
}


void solve_part1() {
  i32 n_moves = 0;
  i32 losing_score = -1;
  i32 odd_sum = -1;
  i32 even_sum = -1;
  while (true) {
    odd_sum = calc_odd_space_sum(n_moves / 2);
    printf("%d ", odd_sum);
    n_moves++;
    if (odd_sum >= 1000) {
      printf("\n");
      losing_score = even_sum;
      break;
    }

    even_sum = calc_even_space_sum(n_moves / 2);
    printf("%d\n", even_sum);
    n_moves++;
    if (even_sum >= 1000) {
      losing_score = odd_sum;
      break;
    }
  }

  printf("losing score: %d\n", losing_score);
  printf("%d moves (%d rolls)\n", n_moves, n_moves * 3);
  printf("answer: %d\n", losing_score * n_moves * 3);
}


int main() {
  /* solve_space_sequence(); */
  solve_part1();

  return 0;
}
