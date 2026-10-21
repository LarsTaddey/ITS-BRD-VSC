/**
 * @file test_operations.c
 * @author Lennard Peschko (), Lars Taddey (2880138) - HAW Hamburg
 * @brief Test the implementations of operations.c to see if they behave as
 expected
 * When in Programs/TaschenrechnerRPN run in terminal:
 * "gcc -Wall -Wextra -std=c11 -I Inc Tests/test_operations.c Src/errors.c
 Src/stack.c Src/display.c -o" Tests/CompiledTests/test_operations"
 * to compile and
 "./Tests/CompiledTests/test_operations"
 * to run it
 * @date October 2026
 * @version GSP Aufgabe 1.1
 */

#include "errors.h"
#include "operations.h"
#include <assert.h>
#include <stdint.h>

int main(void) {
   Stack stack;
   int32_t number;
   int32_t error;

   // Test 1: Addition
   init_stack(&stack);

   push(&stack, 1);
   push(&stack, 3);
   assert(stack.size == 2);
   error = add(&stack);
   assert(error == 0);
   assert(stack.array[stack.size - 1] == 4);

   // Only one element on stack
   pop(&stack, &number);
   error = add(&stack);
   assert(error == ERROR_STACK_UNDERFLOW);

   // Test arithmetic overflow
   push(&stack, INT32_MAX);
   push(&stack, 1);
   error = add(&stack);
   assert(error == ERROR_ARITHMETIC_OVERFLOW);

   push(&stack, INT32_MIN);
   push(&stack, -1);
   error = add(&stack);
   assert(error == ERROR_ARITHMETIC_OVERFLOW);

   // Test 2: Subtraction
   init_stack(&stack);

   push(&stack, 4);
   push(&stack, 1);
   error = sub(&stack);
   assert(error == 0);
   assert(stack.array[stack.size - 1] == 3);

   // Only one element on stack
   pop(&stack, &number);
   error = sub(&stack);
   assert(error == ERROR_STACK_UNDERFLOW);

   // Test arithmetic overflow
   push(&stack, INT32_MAX);
   push(&stack, -1);
   error = sub(&stack);
   assert(error == ERROR_ARITHMETIC_OVERFLOW);

   push(&stack, INT32_MIN);
   push(&stack, 1);
   error = sub(&stack);
   assert(error == ERROR_ARITHMETIC_OVERFLOW);

   // Test 3: Multiplication
   init_stack(&stack);
   push(&stack, 4);
   push(&stack, 2);
   error = mult(&stack);
   assert(error == 0);
   assert(stack.array[stack.size - 1] == 8);

   // Only one element on stack
   pop(&stack, &number);
   error = mult(&stack);
   assert(error == ERROR_STACK_UNDERFLOW);

   // Test one operand equals zero
   push(&stack, 1);
   push(&stack, 0);
   error = mult(&stack);
   assert(error == 0);
   assert(stack.array[stack.size - 1] == 0);

   // Test arithmetic overflow

   push(&stack, INT32_MAX);
   push(&stack, 1);
   error = mult(&stack);
   assert(error == 0);

   init_stack(&stack);

   push(&stack, 2);
   error = mult(&stack);
   assert(error == ERROR_ARITHMETIC_OVERFLOW);

   push(&stack, INT32_MIN);
   push(&stack, 1);
   error = mult(&stack);
   assert(error == 0);

   push(&stack, -1);
   error = mult(&stack);
   assert(error == ERROR_ARITHMETIC_OVERFLOW);

   push(&stack, 46340);
   push(&stack, 46340);
   error = mult(&stack);
   assert(error == 0);

   push(&stack, 46341);
   push(&stack, 46341);
   error = mult(&stack);
   assert(error == ERROR_ARITHMETIC_OVERFLOW);

   push(&stack, -46341);
   push(&stack, 46341);
   error = mult(&stack);
   assert(error == ERROR_ARITHMETIC_OVERFLOW);

   // Test 4: Division
   init_stack(&stack);

   push(&stack, 25);
   push(&stack, 8);
   error = div(&stack);
   assert(error == 0);
   assert(stack.array[stack.size - 1] == 3);

   // Only one element on the stack
   pop(&stack, &number);
   error = div(&stack);
   assert(error == ERROR_STACK_UNDERFLOW);

   // Division by zero
   push(&stack, 1);
   push(&stack, 0);
   error = div(&stack);
   assert(error == ERROR_DIVISION_BY_ZERO);

   // Test arithmetic overflow
   push(&stack, INT32_MIN);
   push(&stack, -1);
   error = div(&stack);
   assert(error == ERROR_ARITHMETIC_OVERFLOW);

   // Test 5: Swap
   init_stack(&stack);

   int32_t temp1;
   int32_t temp2;
   push(&stack, 1);
   push(&stack, -2);
   temp1 = stack.array[stack.size - 2];
   temp2 = stack.array[stack.size - 1];
   error = swap(&stack);
   assert(error == 0);
   assert(stack.array[stack.size - 1] == temp1);
   assert(stack.array[stack.size - 2] == temp2);

   // Only one element on stack
   pop(&stack, &number);
   error = swap(&stack);
   assert(error == ERROR_STACK_UNDERFLOW);

   // Test 6: print_n

   // Test 7: clear
   init_stack(&stack);
   push(&stack, 1);
   assert(stack.size == 1);
   clear(&stack);
   assert(stack.size == 0);

   // Test 8: mult_by_two
   init_stack(&stack);

   // No element on stack
   error = mult_by_two(&stack);
   assert(error == ERROR_STACK_UNDERFLOW);

   push(&stack, 1);
   error = mult_by_two(&stack);
   assert(error == 0);
   assert(stack.array[stack.size - 1] == 2);

   push(&stack, -1);
   error = mult_by_two(&stack);
   assert(error == 0);
   assert(stack.array[stack.size - 1] == -2);

   return 0;
}