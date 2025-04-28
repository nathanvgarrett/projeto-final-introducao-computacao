#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "util.h"


char *array_string(const int arr[], const int size) {
  if (size <= 0) {
    return "[]";
  }

  // the integers are only single char: 1 or 0
  int length = size * 2 + 2;
  char *result = malloc(length * sizeof(char));

  snprintf(result, length, "[%d", arr[0]);
  for (int i = 1; i < size; i++)
    snprintf(result + strlen(result), length - strlen(result), ",%d", arr[i]);
  snprintf(result + strlen(result), length - strlen(result), "]");
  return result;
}


uint8_t features2bits(const int feats[4]) {
  uint8_t result = 0;

  for (int i = 0; i < 4; i++)
    if (feats[i] == 1)
      result |= (1 << (3 - i));
  return result;
}


void bits2features(const uint8_t value, int feats[4]) {
  for (int i = 0; i < 4; i++) {
    feats[i] = (value >> (3 - i)) & 1 ? 1 : 0;
  }
}

