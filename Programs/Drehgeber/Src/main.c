/**
******************************************************************************
* @file    main.c
* @author Len Peschko (2840024), Lars Taddey (2880138) - HAW Hamburg
* @brief Main application entry point. Implements the concept of a rotary
* encoder and initializes the hardware and application modules, then manages the
* finite state machine, the input, angle and angular speed calculation,
* and output.
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/

#include "angle_calculation.h"
#include "init.h"
#include "input.h"
#include "output.h"
#include "phase_change.h"
#include "timer.h"
#include <complex.h>
#include <stdint.h>

int main(void) {
   PhaseData phase_last;
   PhaseData phase_now;
   AngleData angle_data;
   uint32_t phase_counter;
   int32_t direction;
   SystemState state;

   // Initialization on start
   initITSboard();
   initTimer();
   output_init();
   state = STATE_INIT;

   while (1) {
      if (clear_button_pressed()) {
         state = STATE_INIT;
      }
      // FSM with three states
      switch (state) {
      // Define variables and set default output
      case STATE_INIT:
         set_default_output();
         phase_counter = 0;
         read_pins_and_time(&phase_last);
         phase_now = phase_last;
         state = STATE_NORMAL;
         break;

      // Read pins and check if a phase change occured, then calculate angle and
      // angle speed
      case STATE_NORMAL:
         read_pins_and_time(&phase_now);
         direction = phase_change(&phase_last, &phase_now, &phase_counter);
         // angle and speed calc with AngleData struct (timer speed is 180Mhz)
         // angle is = counter/1200 * 360 or counter * 0.3
         // angle speed is = (deltaCounter * 360 * 180.000.000) / 1200 *
         // deltaTimerTicks

         output_counter_leds(phase_counter);
         output_display(&angle_data);

         if (direction == DIRECTION_FORWARD) {
            output_backward_led(false);
            output_forward_led(true);
         }
         if (direction == DIRECTION_BACKWARD) {
            output_forward_led(false);
            output_backward_led(true);
         }
         if (direction == INVALID_TRANSITION) {
            output_error_led(true);
            state = STATE_ERROR;
            break;
         }
         break;

      // Waits for clear_button to be pressed and then resets the state
      case STATE_ERROR:
         if (clear_button_pressed()) {
            state = STATE_INIT;
         }
         break;

      default:
         state = STATE_INIT;
         break;
      }
   }
}

// EOF
