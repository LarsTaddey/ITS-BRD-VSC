/**
 * @file angle_calculation.h
 * @brief Declares functions for angle and angular speed calculation based on
 * phase data
 * @author Len Peschko (2840024), Lars Taddey (2880138) - HAW Hamburg
 */

#ifndef ANGLE_CALCULATION_H
#define ANGLE_CALCULATION_H

#include "data_types.h"

/**
 * @brief Calculates the angle corresponding to the current phase
 *
 * @param current_phase Pointer to the current phase data
 * @return double Angle in degrees
 */
double calculate_angle(PhaseData *current_phase);

/**
 * @brief Calculates the angular speed based on the change in phase and time
 *
 * @param reference_phase Pointer to the phase data from the reference_phase for
 * calculation
 * @param current_phase Pointer to the phase data from the current measurement
 * @return double Angular speed in degrees per second
 */
double calculate_angular_speed(PhaseData *reference_phase,
                               PhaseData *current_phase);

#endif
