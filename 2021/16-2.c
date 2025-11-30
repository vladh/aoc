#include <assert.h>
#include <stdio.h>
#include "common.h"


FILE *f;

// For get_next_bit()
i8 last_digit = -1;
i8 idx_last_bit = 3;

typedef struct PacketData {
  i64 len;
  i64 value;
} PacketData;


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


char* get_operator_type(u64 type_id) {
  if (type_id == 0) {
    return "sum";
  } else if (type_id == 1) {
    return "product";
  } else if (type_id == 2) {
    return "minimum";
  } else if (type_id == 3) {
    return "maximum";
  } else if (type_id == 4) {
    return "literal";
  } else if (type_id == 5) {
    return "greater than";
  } else if (type_id == 6) {
    return "less than";
  } else if (type_id == 7) {
    return "equal to";
  }
  return "???";
}


PacketData read_packet();


PacketData read_literal_packet(i32 nesting_level) {
  i64 packet_len = 0;

  u64 literal = 0;
  u64 block_header;
  do {
    block_header = get_n_bits(1);
    literal = (literal << 4) + get_n_bits(4);
    packet_len += 5;
  } while (block_header == 1);

  print_nesting(nesting_level);
  printf("literal: %lu\n", literal);

  return (PacketData){packet_len, literal};
}


i64 reduce_packet_value(i64 accumulator, i64 value, u64 type_id) {
  if (accumulator == ~0) {
    return value;
  }

  if (type_id == 0) {
    // Sum
    return accumulator + value;

  } else if (type_id == 1) {
    // Product
    return accumulator * value;

  } else if (type_id == 2) {
    // Minimum
    if (value < accumulator) {
      return value;
    } else {
      return accumulator;
    }

  } else if (type_id == 3) {
    // Maximum
    if (value > accumulator) {
      return value;
    } else {
      return accumulator;
    }

  } else if (type_id == 5) {
    // Greater than
    return (accumulator > value) ? 1 : 0;

  } else if (type_id == 6) {
    // Less than
    return (accumulator < value) ? 1 : 0;

  } else if (type_id == 7) {
    // Equal to
    return (accumulator == value) ? 1 : 0;
  }
  return accumulator;
}


PacketData read_operator_packet(i32 nesting_level, u64 type_id) {
  i64 packet_len = 0;
  i64 packet_value = ~0;

  u64 length_type_id = get_n_bits(1);
  packet_len += 1;

  if (length_type_id == 0) {
    // Length-based subpacket
    u64 total_subpacket_len = get_n_bits(15);
    packet_len += 15;

    print_nesting(nesting_level);
    printf("total_subpacket_len: %lu\n", total_subpacket_len);

    u64 seen_subpacket_len = 0;
    while (seen_subpacket_len < total_subpacket_len) {
      print_nesting(nesting_level);
      printf("subpacket {\n");
      PacketData subpacket_data = read_packet(nesting_level + 1);
      packet_value = reduce_packet_value(packet_value, subpacket_data.value, type_id);
      seen_subpacket_len += subpacket_data.len;
      print_nesting(nesting_level);
      printf("}\n");
    }

    packet_len += total_subpacket_len;

  } else {
    // Count-based subpacket
    u64 n_subpackets = get_n_bits(11);
    packet_len += 11;

    print_nesting(nesting_level);
    printf("n_subpackets: %lu\n", n_subpackets);

    for (i32 idx_subpacket = 0; idx_subpacket < n_subpackets; idx_subpacket++) {
      print_nesting(nesting_level);
      printf("subpacket {\n");
      PacketData subpacket_data = read_packet(nesting_level + 1);
      packet_value = reduce_packet_value(packet_value, subpacket_data.value, type_id);
      packet_len += subpacket_data.len;
      print_nesting(nesting_level);
      printf("}\n");
    }
  }

  return (PacketData){packet_len, packet_value};
}


PacketData read_packet(i32 nesting_level) {
  i64 packet_len = 0;

  u64 version = get_n_bits(3);
  if (version == ~0) {
    return (PacketData){0, 0};
  }
  packet_len += 3;

  u64 type_id = get_n_bits(3);
  packet_len += 3;

  print_nesting(nesting_level);
  printf("type: %s (%lu)\n", get_operator_type(type_id), type_id);

  PacketData packet_data;
  if (type_id == 4) {
    packet_data = read_literal_packet(nesting_level);
    packet_len += packet_data.len;
  } else {
    packet_data = read_operator_packet(nesting_level, type_id);
    packet_len += packet_data.len;
  }

  return (PacketData){packet_len, packet_data.value};
}


int main() {
  /* f = fopen("data/16-test", "r"); */
  f = fopen("data/16", "r");

  while (true) {
    printf("-> Reading root packet\n");
    PacketData packet_data = read_packet(0);
    if (packet_data.len == 0) {
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

    printf("packet_data {\n");
    printf("  len: %ld\n", packet_data.len);
    printf("  value: %ld\n", packet_data.value);
    printf("}\n");

    printf("\n");
  }
end:

  return 0;
}
