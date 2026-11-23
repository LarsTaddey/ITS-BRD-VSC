/**
 ******************************************************************************
 * @file    main.c
 * @author  Len Peschko (2840024), Lars Taddey (2880138) - HAW Hamburg
 * @brief   Initializes the ITS board and displays BMP files received over
 *          USB-CDC or UART-via-USB.
 *
 *          Ablauf:
 *            1. Transport waehlen (S7/PF7 = USB-CDC, S6/PF6 = UART-via-USB)
 *               und initialisieren: ask_transport() + init_input(); danach
 *               Hilfetext (show_s5_hint()) und Warten auf S5 (PF5), bevor das
 *               erste Bild uebertragen wird
 *            2. Naechste Datei anfordern und ihren BITMAP-Header in drawBmp()
 *               einlesen, pruefen und auf dem LCD-Display ausgeben.
 *            3. S5 zeigt das Bild, ein weiterer Druck fordert die naechste
 *               Datei an. Ein gehaltener Tastendruck zaehlt nur einmal.
 *
 ******************************************************************************
 */

#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "bmp_display.h"
#include "error.h"
#include "fontsFLASH.h"
#include "init.h"
#include "input.h" /* ask_transport, init_input */
#include "lcd.h"   /* lcdGotoXY */
#include "usb_clock_config.h"
#include <timer.h>

/**
 * @brief  Initializes the board, display and selected transport, then requests
 *         and displays BMP files. S5 advances from the header to the image
 *         and then to the next file.
 * @return Does not return.
 */
int main(void) {
   initITSboard();      // Initialisierung des ITS Boards
   switch_to_168_MHz(); // Zur Unterstützung von USB wird die Taktrate auf ein
                        // vielfaches von 42 MHz gesetzt
   initTimer();
   GUI_init(DEFAULT_BRIGHTNESS); // Initialisierung des LCD Boards mit Touch
   TP_Init(false);               // Initialisierung des LCD Boards mit Touch
   if (!checkVersionFlashFonts()) {
      Error_Handler();
   }

   transport_t transport =
       ask_transport(); // Anwender waehlt am LCD: S7 (PF7) = USB-CDC, S6 (PF6)
                        // = UART-via-USB
   init_input(transport); // Verbindung aufgebaut (Handshake H/R/G)

   /* Hilfetext einmalig anzeigen und auf S5 warten, sobald die Verbindung
      steht -- erst dann das erste Bild einlesen. */
   show_s5_hint();
   wait_for_s_i(
       BUTTON_S5); // erst S5 (PF5) startet die Uebertragung des ersten Bilds

   while (1) {
      GUI_clear(BLACK);
      lcdGotoXY(1, 1);
      open_nxt_file();
      drawBmp();
      wait_for_s_i(BUTTON_S5);
   }
}
// EOF
