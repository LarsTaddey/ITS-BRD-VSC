/**
 * @file read.h
 * @author Len Peschko (2840024), Lars Taddey (2880138) - HAW Hamburg
 * @brief Read integers from the input file.
 */

#include <stdio.h>

#include "input.h"
#include "read.h"

uint8_t read_uint8(bool *was_eof) {
   int value = nxt_chr();
   if ((*was_eof = value == EOF))
      return 0;
   return (uint8_t)value;
}

uint16_t read_uint16(bool *was_eof) {
   int value_0 = nxt_chr();
   int value_1 = nxt_chr();
   if ((*was_eof = value_1 == EOF))
      return 0;
   return (uint16_t)((uint16_t)value_0 | ((uint16_t)value_1 << 8));
}

uint32_t read_uint32(bool *was_eof) {
   int value_0 = nxt_chr();
   int value_1 = nxt_chr();
   int value_2 = nxt_chr();
   int value_3 = nxt_chr();
   if ((*was_eof = value_3 == EOF))
      return 0;
   return ((uint32_t)value_0 | ((uint32_t)value_1 << 8) |
           ((uint32_t)value_2 << 16) | ((uint32_t)value_3 << 24));
}
