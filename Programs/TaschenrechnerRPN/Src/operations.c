/**
 * @file operations.c
 * @author Lennard Peschko (), Lars Taddey (2880138) - HAW Hamburg
 * @brief This module implements basic functionality for the defined cases in
 * token.h. It is able to do arithmetic operations, swap top elements on the
 * stack, print n-elements from the stack, clear the stack and display and
 * double the top element of the stack.
 * @date October 2026
 * @version GSP Aufgabe 1.1
 */

#include "operations.h"
#include "display.h"
#include "errors.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>

/**
 * @brief Helper function which pops two elements from the stack. It also checks
 * if the pop operation causes any errors.
 * @param stack the stack to pop from
 * @param number2 where to store the value of the top element
 * @param number1 where to store the value of the top - 1 element
 */

static int32_t pop_operands(Stack *stack, int32_t *number1, int32_t *number2) {
   int32_t error;

   error = pop(stack, number2);
   if (error < 0) {
      return error;
   }
   error = pop(stack, number1);
   if (error < 0) {
      return error;
   }

   return 0;
}

int32_t add(Stack *stack) {
   int32_t number1;
   int32_t number2;
   int32_t error;

   error = pop_operands(stack, &number1, &number2);
   if (error != 0) {
      return error;
   }
   if ((number2 > 0 && number1 > INT32_MAX - number2) ||
       (number2 < 0 && number1 < INT32_MIN - number2)) {
      return ERROR_ARITHMETIC_OVERFLOW;
   }

   return push(stack, number1 + number2);
}

int32_t sub(Stack *stack) {
   int32_t minuend;
   int32_t subtrahend;
   int32_t error;

   error = pop_operands(stack, &minuend, &subtrahend);
   if (error != 0) {
      return error;
   }
   if ((subtrahend < 0 && minuend > INT32_MAX + subtrahend) ||
       (subtrahend > 0 && minuend < INT32_MIN + subtrahend)) {
      return ERROR_ARITHMETIC_OVERFLOW;
   }

   return push(stack, minuend - subtrahend);
}

int32_t mult(Stack *stack) {
   int32_t multiplicand;
   int32_t multiplier;
   int32_t error;

   error = pop_operands(stack, &multiplicand, &multiplier);
   if (error != 0) {
      return error;
   }
   if ((multiplicand == 0) | (multiplier == 0)) {
      return push(stack, 0);
   }

   if ((multiplicand == -1 & multiplier == INT32_MIN) ||
       (multiplier == -1 & multiplicand == INT32_MIN)) {
      return ERROR_ARITHMETIC_OVERFLOW;
   }

   if (((multiplicand > 0 & multiplier > 0) &&
        multiplicand > INT32_MAX / multiplier) ||
       ((multiplicand > 0 & multiplier < 0) &&
        multiplier < INT32_MIN / multiplicand) ||
       ((multiplicand < 0 & multiplier > 0) &&
        multiplicand < INT32_MIN / multiplier) ||
       ((multiplicand < 0 & multiplier < 0) &&
        multiplicand < INT32_MAX / multiplier)) {
      return ERROR_ARITHMETIC_OVERFLOW;
   }

   return push(stack, multiplicand * multiplier);
}

int32_t div(Stack *stack) {
   int32_t dividend;
   int32_t divisor;
   int32_t error;

   error = pop_operands(stack, &dividend, &divisor);
   if (error != 0) {
      return error;
   }
   if (divisor == 0) {
      return ERROR_DIVISION_BY_ZERO;
   }
   if (dividend == INT32_MIN && divisor == -1) {
      return ERROR_ARITHMETIC_OVERFLOW;
   }

   return push(stack, dividend / divisor);
}

int32_t Swap(Stack *stack) {
   int32_t temporary;

   temporary = stack->array[stack->size - 1];
   stack->array[stack->size - 1] = stack->array[stack->size - 2];
   stack->array[stack->size - 2] = temporary;

   return 0;
}

int32_t print_n(Stack *stack, int32_t count) {

   clear_stdout();
   if (stack->size == 0) {
      print_stdout("Empty Stack!");
   }
   if (count < 0) {
      count = 0;
   } else if (count > stack->size) {
      count = stack->size;
   }
   for (int i = 1; i <= count; i++) {
      char msg[MSG_BUFFER];
      snprintf(msg, sizeof(msg), "%d\n", stack->array[stack->size - i]);
      print_stdout(msg);
   }

   return 0;
}

void clear(Stack *stack) {
   stack->size = 0;
   clear_stdout();
   clear_echo_term();
}

int32_t mult_by_two(Stack *stack) {
   int32_t temporary;

   if (stack->size == 0) {
      return ERROR_STACK_UNDERFLOW;
   }
   temporary = stack->array[stack->size - 1];
   if (temporary > INT32_MAX / 2 || temporary < INT32_MIN / 2) {
      return ERROR_STACK_OVERFLOW;
   }
   stack->array[stack->size - 1] = temporary * 2;

   return 0;
}