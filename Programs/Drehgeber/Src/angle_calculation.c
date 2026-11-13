/**
 * @file angle_calculation.c
 * @brief
 */

#include "angle_calculation.h"
#include "data_types.h"
#include <stdint.h>

#define SLOT_COUNT 1200.0
#define CIRCLE_DEGREE 360.0
#define CPU_FREQ 90000000.0

double calculate_angle(PhaseData *phase_now) {
   return (double)phase_now->phase_counter * (CIRCLE_DEGREE / SLOT_COUNT);
}

double calculate_angular_speed(PhaseData *phase_last, PhaseData *phase_now) {
   // static uint32_t phase_counter_last = 0;
   int32_t phase_counter_delta =
       phase_now->phase_counter - phase_last->phase_counter;
   // static uint32_t timestamp_last = 0;
   uint32_t timestamp_delta = phase_now->timestamp - phase_last->timestamp;

   double delta_seconds = timestamp_delta / CPU_FREQ;
   double delta_angle = phase_counter_delta * (CIRCLE_DEGREE / SLOT_COUNT);

   return delta_angle / delta_seconds;
}