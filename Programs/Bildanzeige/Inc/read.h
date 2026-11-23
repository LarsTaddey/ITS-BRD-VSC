/**
 * @file read.h
 * @author Len Peschko (2840024), Lars Taddey (2880138) - HAW Hamburg
 * @brief Read integers from the input file.
 */

#ifndef READ_H
#define READ_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief reads 1 byte
 * @param was_eof
 * @return value
 */
uint8_t read_uint8(bool *was_eof);

/**
 * @brief read 2 bytes in le
 * @param was_eof
 * @return value
 */
uint16_t read_uint16(bool *was_eof);

/**
 * @brief read 4 bytes in le
 * @param was_eof
 * @return value
 */
uint32_t read_uint32(bool *was_eof);

#endif