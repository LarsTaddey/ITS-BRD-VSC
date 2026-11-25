/**
******************************************************************************
* @file    main.c
* @brief Main application entry point. Implements the concept of a rotary
* encoder and initializes the hardware and application modules, then manages the
* finite state machine, the input, angle and angular speed calculation,
* and output.
* @author Len Peschko (2840024), Lars Taddey (2880138) - HAW Hamburg
* @version Aufgabe_2.2_GSP
* @date 25.11.2026
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
   PhaseData previous_phase;
   PhaseData current_phase;
   PhaseData reference_phase;
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
         previous_phase.phase_counter = 0;
         previous_phase.timestamp = 0;
         current_phase = previous_phase;
         reference_phase = previous_phase;
         read_pins_and_time(&previous_phase);
         state = STATE_RUN;
         break;

      // Read pins and check if a phase change occured, then calculate angle and
      // angle speed
      case STATE_RUN:

         read_pins_and_time(&current_phase);
         direction = phase_change(&previous_phase, &current_phase);

         if ((current_phase.timestamp - reference_phase.timestamp) >
             REFRESH_DELAY) {
            angle_data.angle = calculate_angle(&current_phase);
            angle_data.angle_speed =
                calculate_angular_speed(&reference_phase, &current_phase);

            reference_phase = current_phase;
         }

         previous_phase = current_phase;

         set_counter_leds(current_phase.phase_counter);
         output_data_to_lcd(&angle_data);

         if (direction == DIRECTION_FORWARD) {
            turn_off_backward_led();
            turn_on_forward_led();
         }
         if (direction == DIRECTION_BACKWARD) {
            turn_off_forward_led();
            turn_on_backward_led();
         }
         if (direction == INVALID_TRANSITION) {
            turn_on_error_led();
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
