/**
 ******************************************************************************
 * @file main.c
 * @author Len Peschko (2840024), Lars Taddey (2880138) - HAW Hamburg
 * @brief ITS-Board-Calculator with reverse polish notation.
 * This module handles the functionality of getting tokens from the scanner
 * module and calls a dedicated operation depending on the token. It contains
 * two states. One state is the normal state where the calculator waits for an
 * allowed input and the other state is the error-state where the calculator
 * display an error message and waits for the clear operation to return to the
 * first state.
 * @date October 2026
 * @version GSP Aufgabe 1.1
 ******************************************************************************
 */

#include "main.h"
#include "display.h"
#include "errors.h"
#include "init.h"
#include "operations.h"
#include "scanner.h"
#include "token.h"
#include <stdint.h>
#include <stdio.h>

static Stack stack;
static int32_t error = ERROR_NONE;
static char msg_buffer[MSG_BUFFER];
static const char *error_prefix = "Error:";
static const char *clear_message = "To reset calculator\npress C (clear)";

int main(void) {

   // Initilization of the ITS-Board, display and stack
   initITSboard();
   init_display();
   init_stack(&stack);

   // State 1: Handle incoming tokens and assign them an operation
   while (1) {
      do {
         T_token token = next_token();
         switch (token.tok) {
         case NUMBER:
            error = push(&stack, token.val);
            break;
         case PLUS:
            error = add(&stack);
            break;
         case MINUS:
            error = sub(&stack);
            break;
         case MULT:
            error = mult(&stack);
            break;
         case DIV:
            error = div(&stack);
            break;
         case PRT:
            error = print_n(&stack, 1);
            break;
         case SWAP:
            error = swap(&stack);
            break;
         case PRT_ALL: // only print top three elements from the stack
            error = print_n(&stack, 3);
            break;
         case CLEAR:
            clear(&stack);
            break;
         case DOUBLE:
            error = mult_by_two(&stack);
            break;
         case OVERFLOW:
            error = ERROR_NUM_OVERFLOW;
            break;
         default:
            error = ERROR_UNKNOWN_COMMAND;
            break;
         }
      } while (error == ERROR_NONE);

      // State 2: Shows error message on the display and waits for a clear.
      set_err_mode();

      snprintf(msg_buffer, sizeof(msg_buffer), "%s\n%s\n%s\n", error_prefix,
               error_message(error), clear_message);
      print_stdout(msg_buffer);
      do {
         T_token token = next_token();
         if (token.tok == CLEAR) {
            clear(&stack);
            set_normal_mode();
            error = ERROR_NONE;
         }
      } while (error != ERROR_NONE);
   }
}

// EOF
