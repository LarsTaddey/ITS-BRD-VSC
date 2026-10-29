/**
 * @file errors.c
 * @author Lars Taddey (2880138), Len Peschko (2840024) - HAW Hamburg
 * @brief Implements a simple function which translates an error-code to a
 * readable string
 * @date October 2026
 * @version GSP Aufgabe 1.1
 */

#include "errors.h"

const char* error_message(int32_t error) {
   switch (error) {
   case ERROR_STACK_UNDERFLOW:
      return "Stack underflow!";
   case ERROR_STACK_OVERFLOW:
      return "Stack overflow!";
   case ERROR_NUM_OVERFLOW:
      return "Number is too high!";
   case ERROR_INPUT_OVERFLOW:
      return "Input is too long!";
   case ERROR_ARITHMETIC_OVERFLOW:
      return "Arithmetic overflow!";
   case ERROR_DIVISION_BY_ZERO:
      return "Division by zero!";
   default:
      return "Unknown command!";
   }
}