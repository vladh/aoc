#include <assert.h>
#include <stdio.h>
#include "common.h"


FILE *f;

// For get_next_bit()
i8 last_digit = -1;
i8 idx_last_bit = 3;

u64 version_sum = 0;


void reset_bit_getter() {
  last_digit = -1;
}


i8 get_next_bit() {
  if (last_digit < 0 || idx_last_bit < 0) {
    idx_last_bit = 3;
    if (fscanf(f, "%1hhx", &last_digit) < 0) {
      return -1;
    }
  }
  i8 res = (last_digit & (1 << idx_last_bit)) >> idx_last_bit;
  idx_last_bit--;
  return res;
}


u64 get_n_bits(i32 n) {
  u64 res = 0;
  for (i32 i = n - 1; i >= 0; i--) {
    i8 next_bit = get_next_bit();
    if (next_bit < 0) {
      return ~0;
    }
    res += next_bit << i;
  }
  return res;
}


void print_nesting(i32 nesting_level) {
  for (u32 i = 0; i < nesting_level; i++) {
    printf(" ");
  }
}


i32 read_packet();


i32 read_literal_packet(i32 nesting_level) {
  i32 packet_len = 0;

  u64 literal = 0;
  u64 block_header;
  do {
    block_header = get_n_bits(1);
    literal = (literal << 4) + get_n_bits(4);
    packet_len += 5;
  } while (block_header == 1);

  print_nesting(nesting_level);
  printf("literal: %lu\n", literal);

  return packet_len;
}


i32 read_operator_packet(i32 nesting_level) {
  i32 packet_len = 0;

  u64 length_type_id = get_n_bits(1);
  packet_len += 1;

  if (length_type_id == 0) {
    u64 total_subpacket_len = get_n_bits(15);
    packet_len += 15;

    print_nesting(nesting_level);
    printf("total_subpacket_len: %lu\n", total_subpacket_len);

    u64 seen_subpacket_len = 0;

    while (seen_subpacket_len < total_subpacket_len) {
      print_nesting(nesting_level);
      printf("subpacket {\n");
      seen_subpacket_len += read_packet(nesting_level + 1);
      print_nesting(nesting_level);
      printf("}\n");
    }

    packet_len += total_subpacket_len;
  } else {
    u64 n_subpackets = get_n_bits(11);
    packet_len += 11;

    print_nesting(nesting_level);
    printf("n_subpackets: %lu\n", n_subpackets);

    for (i32 idx_subpacket = 0; idx_subpacket < n_subpackets; idx_subpacket++) {
      print_nesting(nesting_level);
      printf("subpacket {\n");
      packet_len += read_packet(nesting_level + 1);
      print_nesting(nesting_level);
      printf("}\n");
    }
  }

  return packet_len;
}


i32 read_packet(i32 nesting_level) {
  i32 packet_len = 0;

  u64 version = get_n_bits(3);
  if (version == ~0) { return 0; }
  version_sum += version;
  packet_len += 3;

  u64 type_id = get_n_bits(3);
  packet_len += 3;

  if (type_id == 4) {
    print_nesting(nesting_level);
    printf("type: literal\n");
    packet_len += read_literal_packet(nesting_level);
  } else {
    print_nesting(nesting_level);
    printf("type: operator\n");
    packet_len += read_operator_packet(nesting_level);
  }

  return packet_len;
}


int main() {
  /* f = fopen("data/16-test", "r"); */
  f = fopen("data/16", "r");

  while (true) {
    version_sum = 0;
    printf("-> Reading root packet\n");
    i32 packet_len = read_packet(0);
    if (packet_len == 0) {
      printf("...but there was none\n");
      break;
    }
    reset_bit_getter();

    // Consume unread characters up to newline (or EOF)
    char c;
    while (true) {
      c = fgetc(f);
      if (c == '\n') { break; }
      if (c == EOF) { goto end; }
    }

    printf("version sum: %lu\n", version_sum);
    printf("\n");
  }
end:

  return 0;
}
