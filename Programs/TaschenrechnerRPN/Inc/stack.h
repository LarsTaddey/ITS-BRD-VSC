/**
 * @file stack.h
 * @brief This module defines the stack and basic functions for the stack.
 */

#include <stdint.h>

// Maximum size of the stack-array
#define MAX_SIZE 10

/// The stack is implemented as a static array of integers
typedef struct Stack {
   int32_t array[MAX_SIZE];
   int32_t size;
} Stack;

/**
 * @brief Initializes the stack with default values
 * @param stack the stack to initialize
 */
void init_stack(Stack *stack);

/**
 * @brief Pushes a number to the top of a stack
 * @param number the number to push
 * @return int identifier if an error occured
 */
int32_t push(Stack *stack, int32_t number);

/**
 * @brief Pops a number from the top of the stack
 * @param number the variable to pop the value to
 * @return int indentifier if an error occured
 */
int32_t pop(Stack *stack, int32_t *number);