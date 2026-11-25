/**
 * @file input.c
 * @brief Implements the functions for reading phase-pin input and button
 * events.
 * @author Len Peschko (2840024), Lars Taddey (2880138) - HAW Hamburg
 */

#include "input.h"
#include "stm32f429xx.h"
#include "timer.h"

#define PHASE_INPUT_MASK 0x03U
#define BUTTON_CLEAR_MASK 0x40U

int32_t read_pins_and_time(PhaseData *phase_data) {

   if (phase_data == NULL) {
      return -1;
   }
   phase_data->phase = (GPIOF->IDR) & PHASE_INPUT_MASK;
   phase_data->timestamp = getTimeStamp();

   return 0;
}

bool clear_button_pressed() { return GPIOF->IDR & BUTTON_CLEAR_MASK ? 0 : 1; }