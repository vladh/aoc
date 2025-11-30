#include <stdio.h>
#include <string.h>
#include "common.h"


#define MAX_LINE_LEN 256


i32 score(char c) {
  if (c == ')') {
    return 3;
  } else if (c == ']') {
    return 57;
  } else if (c == '}') {
    return 1197;
  } else if (c == '>') {
    return 25137;
  }
  printf("WARNING: invalid char to score\n");
  return -1;
}


bool is_opener(char c) {
  return c == '(' ||
    c == '[' ||
    c == '{' ||
    c == '<';
}


bool is_closer(char c) {
  return c == ')' ||
    c == ']' ||
    c == '}' ||
    c == '>';
}


char get_open_version(char c) {
  if (c == ')') {
    return '(';
  } else if (c == ']') {
    return '[';
  } else if (c == '}') {
    return '{';
  } else if (c == '>') {
    return '<';
  }
  printf("WARNING: invalid closed char to get open version for\n");
  return '\0';
}


int main() {
  FILE *f = fopen("data/10-test", "r");

  char stack[MAX_LINE_LEN] = {};
  i32 stack_size = 0;

  i32 syntax_score = 0;

  while (true) {
    char c = (char)fgetc(f);

    if (c == EOF) {
      break;
    } else if (c == '\n') {
      memset(stack, 0, sizeof(stack));
      stack_size = 0;
      continue;
    }

    if (is_opener(c)) {
      stack[stack_size++] = c;
    } else if (is_closer(c)) {
      if (stack[stack_size - 1] == get_open_version(c)) {
        stack[stack_size - 1] = '\0';
        stack_size--;
      } else {
        syntax_score += score(c);
        while (c != '\n') {
          c = (char)fgetc(f);
        }
        memset(stack, 0, sizeof(stack));
        stack_size = 0;
        continue;
      }
    }
  }

  printf("syntax_score: %d\n", syntax_score);

  return 0;
}
