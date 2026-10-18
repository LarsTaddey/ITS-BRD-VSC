/**
 ******************************************************************************
 * @file    main.c
 * @author  Franz Korf, Len Peschko
 * @brief   ITS-Board-Rechner mit umgekehrter polnischer Notation
 ******************************************************************************
 */

#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "init.h"
#include "lcd.h"
#include "stm32f4xx_hal.h"

#include "main.h"
#include "token.h"
#include "scanner.h"
#include "stack.h"
#include "operations.h"


int main(void) {
    // Initialisierung des ITS-Boards
    initITSboard();

    // Initialisierung des LCD-Boards mit Touch
    GUI_init(DEFAULT_BRIGHTNESS); 
    TP_Init(false);

    // Begruessungstext
    lcdPrintlnS("Hallo liebes TI-Labor (c-project)");

	int error = ERROR_NONE;
    while (1) {
		// Zustand: Operation eingeben und ggf. Ergebnis ausgeben
		do {
			T_token token = next_token();
			switch (token->tok) {
				case NUMBER: error = push(token->val); break;
				case PLUS: error = add(); break;
				case MINUS: error = sub(); break;
				case MULT: error = mult(); break;
				case DIV: error = div(); break;
				case PRT: error = print_n(1); break;
				case SWAP: error = swap(); break;
				case PRT_ALL: error = print_n(3); break; // "all" is a lie but whatever
				case CLEAR: clear(); break;
				case DOUBLE: error = push(2) || mult(); break;
				case OVERFLOW: error = ERROR_NUM_OVERFLOW; break;
				default: error = ERROR_UNKNOWN_COMMAND; break;
			}
		}
		while (error == ERROR_NONE);

		// Zustand: Fehler anzeigen und quittieren
		lcdPrintlnS("Fehler!!!");
		// TODO: Fehlertyp ausgeben
		do {
			T_token token = next_token();
			if (token->tok == ENTER) {
				error = ERROR_NONE;
			}
			else {
				lcdPrintlnS("Nein! Bestaetigen mit Enter!");
			}
		}
		while (error != ERROR_NONE);
    }
}

// EOF
