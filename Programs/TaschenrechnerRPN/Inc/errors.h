/**
 * @brief Defines error constants and a simple function to handle error-codes.
 */

#include <stdint.h>

// Defines a "unique" number for each error that can occur
#define ERROR_NONE 0
#define ERROR_STACK_UNDERFLOW (-1)
#define ERROR_STACK_OVERFLOW (-2)
#define ERROR_NUM_OVERFLOW (-3)
#define ERROR_DIVISION_BY_ZERO (-4)
#define ERROR_UNKNOWN_COMMAND (-5)
#define ERROR_ARITHMETIC_OVERFLOW (-6)
#define ERROR_INPUT_OVERFLOW (-7)

/**
 * @brief Returns a string constant depending on the given error code
 * @param error a negative number which represents an error
 * @return a pointer to the address of the char sequence
 */
const char *error_message(int32_t error);