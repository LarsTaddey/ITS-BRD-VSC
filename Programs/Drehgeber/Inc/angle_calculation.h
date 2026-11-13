/**
 * @file angle_calculation.h
 * @brief
 */

#ifndef ANGLE_CALCULATION_H
#define ANGLE_CALCULATION_H

#include "data_types.h"

/**
 * @brief
 * @param phase_counter
 * @return double
 */
double calculate_angle(PhaseData *phase_now);

/**
 * @brief
 * @return double
 */
double calculate_angular_speed(PhaseData *phase_last, PhaseData *phase_now);

#endif
