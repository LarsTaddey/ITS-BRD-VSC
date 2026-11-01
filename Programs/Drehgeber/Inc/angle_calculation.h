/**
 * @file angle_calculation.h
 * @brief
 */

#ifndef ANGLE_CALCULATION_H
#define ANGLE_CALCULATION_H

#include "data_types.h"
#include <stdint.h>

/**
 * @brief
 * @param phase_counter
 * @return AngleData
 */
AngleData calculate_angle(const uint32_t *phase_counter);

/**
 * @brief
 * @param phase_counter
 * @return AngleData
 */
AngleData calculate_angular_speed(const uint32_t *phase_counter);

#endif
