/**
 * @file input.c
 * @brief Implements the functions for reading input and clear button events.
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
   phase_data->phase = ~(GPIOF->IDR) & PHASE_INPUT_MASK;
   phase_data->timestamp = getTimeStamp();

   return 0;
}

bool clear_button_pressed() {
   // For "on release" behavior
   // static bool previous_state = true;
   // bool current_state = GPIOF->IDR & BUTTON_CLEAR_MASK;
   // bool button_released = (previous_state == false && current_state == true);
   // previous_state = current_state;

   // "pressed" behavior
   return GPIOF->IDR & BUTTON_CLEAR_MASK ? 0 : 1;
}