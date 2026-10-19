/**
 * @brief All operations take their operands from the stack and push their
 * result to the stack. THey return one of ERROR_* from main.h
 *
 */

#include "stack.h"

/**
 * @brief Adds two numbers
 */
int32_t add(Stack *stack);

/**
 * @brief Subtracts two numbers
 */
int32_t sub(Stack *stack);

/**
 * @brief Multiplies two numbers
 */
int32_t mult(Stack *stack);

/**
 * @brief Divides a number with another number
 * Results are rounded to a decimal
 */
int32_t div(Stack *stack);

/**
 * @brief Swaps the first to elements of the stack
 *
 */
int32_t swap(Stack *stack);

/**
 * @brief Prints an amount of elements from the top of the stack
 *
 * @param count how many elements to print from the stack
 */
int32_t print_n(Stack *stack, int count);

/**
 * @brief Clears the display and stack
 */
void clear(Stack *stack);

/**
 * @brief Multiplies the top element on the stack by two
 */
int32_t mul_2(Stack *stack);
