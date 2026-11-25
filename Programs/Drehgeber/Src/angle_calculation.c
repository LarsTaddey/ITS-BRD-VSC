/**
 * @file angle_calculation.c
 * @brief Implements angle and angular speed calculations based on phase data
 * @author Len Peschko (2840024), Lars Taddey (2880138) - HAW Hamburg
 */

#include "angle_calculation.h"
#include "data_types.h"
#include "timer.h"
#include <stdint.h>

#define SLOT_COUNT 1200.0
#define DEGREE_PER_PHASE (360.0 / SLOT_COUNT)
// Time runs with 90MHz
#define TIMER_FREQUENCY (TICKS_PER_US * 1000000.0)

double calculate_angle(PhaseData *current_phase) {
   return (double)current_phase->phase_counter * DEGREE_PER_PHASE;
}

double calculate_angular_speed(PhaseData *reference_phase,
                               PhaseData *current_phase) {
   int32_t phase_counter_delta =
       current_phase->phase_counter - reference_phase->phase_counter;
   uint32_t timestamp_delta =
       current_phase->timestamp - reference_phase->timestamp;

   double delta_seconds = timestamp_delta / TIMER_FREQUENCY;
   double delta_angle = phase_counter_delta * DEGREE_PER_PHASE;

   return delta_angle / delta_seconds;
}