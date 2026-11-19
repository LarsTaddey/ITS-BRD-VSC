/**
 * @file angle_calculation.c
 * @brief
 */

#include "angle_calculation.h"
#include "data_types.h"
#include "timer.h"
#include <stdint.h>

#define SLOT_COUNT 1200.0
#define ANGLE (360.0 / SLOT_COUNT)
#define TIMER_FAKTOR 1000000.0

double calculate_angle(PhaseData phase_now) {
   return (double)phase_now.phase_counter * ANGLE;
}

double calculate_angular_speed(PhaseData phase_last, PhaseData phase_now) {
   int32_t phase_counter_delta =
       phase_now.phase_counter - phase_last.phase_counter;
   uint32_t timestamp_delta = phase_now.timestamp - phase_last.timestamp;

   double delta_seconds = timestamp_delta / (TICKS_PER_US * TIMER_FAKTOR);
   double delta_angle = phase_counter_delta * ANGLE;

   return delta_angle / delta_seconds;
}