/**
 * @brief This module implements basic functions of a stack. It is possible to
 * push from and pop to the stack. The stack has a fixed size (see stack.h).
 */

#include "stack.h"
#include "errors.h"

void init_stack(Stack *stack) { stack->size = 0; }

int32_t pop(Stack *stack, int *number) {

   if (stack->size == 0) {
      return ERROR_STACK_UNDERFLOW;
   }
   stack->size--;
   *number = stack->array[stack->size];
   return ERROR_NONE;
}

int32_t push(Stack *stack, int number) {

   if (stack->size >= MAX_SIZE) {
      return ERROR_STACK_OVERFLOW;
   }
   stack->array[stack->size] = number;
   stack->size++;
   return ERROR_NONE;
}