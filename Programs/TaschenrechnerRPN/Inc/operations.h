/**
 * @brief Defines the functions for the operation cases which are defined in
 * token.h. Arithmetic functions all pop two elements from the stack and write
 * the result back to the top of the stack. Other operations either manipulate
 * one or two elements on the stack or print them on the display.
 *
 * @return all function return a "unique" negative 32-Bit Integer for each error
 * case or a zero for no error.
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
 * @brief Prints an n-amount of elements from the top of the stack
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
int32_t mult_by_two(Stack *stack);
