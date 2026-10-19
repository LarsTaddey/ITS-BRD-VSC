/**
 ******************************************************************************
 * @file    main.c
 * @author  Franz Korf, Len Peschko
 * @brief   ITS-Board-Rechner mit umgekehrter polnischer Notation
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
static const char *error_prefix = "Error:\n";
static const char *clear_message = "\nReset calculator (press C)";

int main(void) {

   // Initialisierung des ITS-Boards
   initITSboard();
   init_display();
   init_stack(&stack);

   push(&stack, INT32_MAX);

   while (1) {
      // Zustand: Operation eingeben und ggf. Ergebnis ausgeben
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
         case PRT_ALL:
            error = print_n(&stack, 3);
            break; // "all" is a lie but whatever
         case CLEAR:
            clear(&stack);
            break;
         case DOUBLE:
            error = mul_2(&stack);
            break;
         case OVERFLOW:
            error = ERROR_NUM_OVERFLOW;
            break;
         default:
            error = ERROR_UNKNOWN_COMMAND;
            break;
         }
      } while (error == ERROR_NONE);

      // Zustand: Fehler anzeigen und quittieren
      set_err_mode();
      snprintf(msg_buffer, sizeof(msg_buffer), "%s%s%s\n", error_prefix,
               error_message(error), clear_message);
      print_stdout(msg_buffer);
      // TODO: Fehlertyp ausgeben
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
