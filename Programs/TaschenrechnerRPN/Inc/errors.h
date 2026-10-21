/**
 * @file error.h
 * @author Lennard Peschko (), Lars Taddey (2880138) - HAW HAMBURG
 * @brief Defines error constants and a simple function to handle error-codes.
 * @date October 2026
 * @version GSP Aufgabe 1.1
 */

#include <stdint.h>

// Defines a "unique" number for each error that can occur
#define ERROR_NONE 0
#define ERROR_STACK_UNDERFLOW (-1)
#define ERROR_STACK_OVERFLOW (-2)
#define ERROR_NUM_OVERFLOW (-3)
#define ERROR_DIVISION_BY_ZERO (-4)
#define ERROR_ARITHMETIC_OVERFLOW (-5)
#define ERROR_INPUT_OVERFLOW (-6)
#define ERROR_UNKNOWN_COMMAND (-7)

/**
 * @brief Returns a string constant depending on the given error code
 * @param error a negative number which represents an error
 * @return a pointer to the address of the char sequence
 */
const char *error_message(int32_t error);