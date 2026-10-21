/**
 * @file test_errors.c
 * @author Lennard Peschko (), Lars Taddey (2880138) - HAW Hamburg
 * @brief Test the implementations of errors.c to see if they behave as expected
 * When in Programs/TaschenrechnerRPN run in terminal:
 * "gcc -Wall -Wextra -std=c11 -I Inc Tests/test_errors.c Src/errors.c -o"
 Tests/CompiledTests/test_errors" to compile and
 "./Tests/CompiledTests/test_errors" to run it
 * @date October 2026
 * @version GSP Aufgabe 1.1
 */

#include "errors.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {

   // Test 1: Error-Code ERROR_STACK_UNDERFLOW
   assert(strcmp(error_message(ERROR_STACK_UNDERFLOW), "Stack underflow!") ==
          0);
   // Test 2: Error-Code ERROR_STACK_OVERFLOW
   assert(strcmp(error_message(ERROR_STACK_OVERFLOW), "Stack overflow!") == 0);
   // Test 3: Error-Code ERROR_NUM_OVERFLOW
   assert(strcmp(error_message(ERROR_NUM_OVERFLOW), "Number is too big!") == 0);
   // Test 4: Error-Code ERROR_INPUT_OVERFLOW
   assert(strcmp(error_message(ERROR_INPUT_OVERFLOW), "Input is too big!") ==
          0);
   // Test 5: Error-Code ERROR_ARITHMETIC_OVERFLOW
   assert(strcmp(error_message(ERROR_ARITHMETIC_OVERFLOW),
                 "Arithmetic overflow!") == 0);
   // Test 6: Error-Code ERROR_DIVISION_BY_ZERO
   assert(strcmp(error_message(ERROR_DIVISION_BY_ZERO), "Division by zero!") ==
          0);
   // Test 7: Error-Code ERROR_UNKNOWN_COMMAND
   assert(strcmp(error_message(ERROR_UNKNOWN_COMMAND), "Unknown command!") ==
          0);

   printf("All error tests have passed.\n");

   return 0;
}
