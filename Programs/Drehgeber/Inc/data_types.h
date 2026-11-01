/**
 * @file data_types.h
 * @brief Defines shared data types used throughout the application, including
 * the system state, encoder phase data, angle and angular speed data and
 * rotation direction.
 */

#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <stdint.h>

typedef enum {
   STATE_INIT,
   STATE_NORMAL,
   STATE_ERROR,
} SystemState;

typedef struct {
   uint32_t phase;
   uint32_t timestamp;
} PhaseData;

typedef struct {
   double angle;
   double angle_speed;
} AngleData;

typedef enum {
   DIRECTION_FORWARD = 1,
   DIRECTION_BACKWARD = -1,
   NO_CHANGE = 0,
   INVALID_TRANSITION = -2,
} Direction;

#endif