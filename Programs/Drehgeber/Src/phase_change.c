/**
 * @file phase_change.c
 * @brief Implements rotation phase transition detection and tracking.
 * @author Len Peschko (2840024), Lars Taddey (2880138) - HAW Hamburg
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

int32_t phase_change(PhaseData *previous_phase, PhaseData *current_phase) {
   int32_t direction;

   switch ((uint8_t)(previous_phase->phase << 2U) | current_phase->phase) {
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
      current_phase->phase_counter++;
      direction = DIRECTION_FORWARD;
      break;

   // Backwards
   case ((uint32_t)PHASE_A << 2U) | (uint32_t)PHASE_D:
   case ((uint32_t)PHASE_D << 2U) | (uint32_t)PHASE_C:
   case ((uint32_t)PHASE_C << 2U) | (uint32_t)PHASE_B:
   case ((uint32_t)PHASE_B << 2U) | (uint32_t)PHASE_A:

      current_phase->phase_counter--;
      direction = DIRECTION_BACKWARD;
      break;

   default:
      return INVALID_TRANSITION;
   }

   return direction;
}
