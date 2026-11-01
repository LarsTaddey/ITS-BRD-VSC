/**
 * @file phase_change.c
 * @brief Implements rotary encoder phase-transition detection and tracking.
 */

#include "phase_change.h"
#include "data_types.h"
#include <stdint.h>

typedef enum {
   PHASE_A = 0, // 00
   PHASE_B = 2, // 10
   PHASE_C = 3, // 11
   PHASE_D = 1, // 01
} Phase;

int32_t phase_change(PhaseData *phase_last, const PhaseData *phase_now,
                     uint32_t *counter) {
   int32_t direction;

   switch ((phase_last->phase << 2U) | phase_now->phase) {
   // No phase change
   case ((uint32_t)PHASE_A << 2U) | (uint32_t)PHASE_A:
   case ((uint32_t)PHASE_B << 2U) | (uint32_t)PHASE_B:
   case ((uint32_t)PHASE_C << 2U) | (uint32_t)PHASE_C:
   case ((uint32_t)PHASE_D << 2U) | (uint32_t)PHASE_D:
      direction = NO_CHANGE;
      break;

   // Forward
   case ((uint32_t)PHASE_A << 2U) | (uint32_t)PHASE_B:
   case ((uint32_t)PHASE_B << 2U) | (uint32_t)PHASE_C:
   case ((uint32_t)PHASE_C << 2U) | (uint32_t)PHASE_D:
   case ((uint32_t)PHASE_D << 2U) | (uint32_t)PHASE_A:
      (*counter)++;
      direction = DIRECTION_FORWARD;
      break;

   // Backwards
   case ((uint32_t)PHASE_A << 2U) | (uint32_t)PHASE_D:
   case ((uint32_t)PHASE_D << 2U) | (uint32_t)PHASE_C:
   case ((uint32_t)PHASE_C << 2U) | (uint32_t)PHASE_B:
   case ((uint32_t)PHASE_B << 2U) | (uint32_t)PHASE_A:

      (*counter)++;
      direction = DIRECTION_BACKWARD;
      break;

   default:
      return INVALID_TRANSITION;
   }
   *phase_last = *phase_now;

   return direction;
}
