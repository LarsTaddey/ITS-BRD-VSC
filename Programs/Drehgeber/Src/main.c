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
#include "data_types.h"
#include "init.h"
#include "input.h"
#include "output.h"
#include "phase_change.h"
#include "timer.h"
#include <complex.h>
#include <stdint.h>

#define REFRESH_DELAY ((300 * 1000) * TICKS_PER_US)

int main(void) {
   PhaseData phase_last;
   PhaseData phase_now;
   PhaseData phase_buffer;
   AngleData angle_data;
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
         phase_last.phase_counter = 0;
         phase_last.timestamp = 0;
         phase_now = phase_last;
         phase_buffer = phase_last;
         read_pins_and_time(&phase_last);
         state = STATE_NORMAL;
         break;

      // Read pins and check if a phase change occured, then calculate angle and
      // angle speed
      case STATE_NORMAL:

         read_pins_and_time(&phase_now);
         direction = phase_change(&phase_last, &phase_now);

         if ((phase_now.timestamp - phase_buffer.timestamp) > REFRESH_DELAY) {
            angle_data.angle = calculate_angle(phase_now);
            angle_data.angle_speed =
                calculate_angular_speed(phase_buffer, phase_now);

            phase_buffer = phase_now;
         }

         phase_last = phase_now;

         output_counter_leds(phase_now);
         output_display(angle_data);

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
