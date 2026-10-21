/**
 * @file test_stack.c
 * @author Lennard Peschko (), Lars Taddey (2880138) - HAW Hamburg
 * @brief Test the implementations of stack.c to see if they behave as expected
 * When in Programs/TaschenrechnerRPN run in terminal:
 * "gcc -Wall -Wextra -std=c11 -I Inc Tests/test_stack.c Src/stack.c
 * Src/errors.c -o"
 * Tests/CompiledTests/test_stack" to compile and
 "./Tests/CompiledTests/test_stack" to run it
 * @date October 2026
 * @version GSP Aufgabe 1.1
 */

#include "errors.h"
#include "stack.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
   Stack stack;
   int32_t error;
   int32_t number;

   // Test 1: init_stack
   init_stack(&stack);
   assert(stack.size == 0);

   // Test 2: push and pop one element
   error = push(&stack, 1);
   assert(error == 0);
   assert(stack.size == 1);

   error = pop(&stack, &number);
   assert(error == 0);
   assert(number == 1);
   assert(stack.size == 0);

   // Test 3: pop from empty stack
   error = pop(&stack, &number);
   assert(error == ERROR_STACK_UNDERFLOW);

   // Test 4: push on full stack
   for (int i = 0; i < MAX_SIZE; ++i) {
      error = push(&stack, (0 + i));
      assert(error == 0);
      // Stack contains MAX_SIZE elements and is full
   }
   assert(stack.size == MAX_SIZE);

   error = push(&stack, 42);
   assert(error == ERROR_STACK_OVERFLOW);

   printf("All stack tests passed.\n");

   return 0;
}