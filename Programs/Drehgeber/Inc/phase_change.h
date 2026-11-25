/**
 * @file phase_change.h
 * @brief Declares a function for detecting rotation phase transitions and
 * updating the rotation direction and transition counter.
 * @author Len Peschko (2840024), Lars Taddey (2880138) - HAW Hamburg
 */

#ifndef PHASE_CHANGE_H
#define PHASE_CHANGE_H

#include "data_types.h"
#include <stdint.h>

/**
 * @brief Determines the direction of phase progression and updates the phase
 * counter and detected direction accordingly
 * Compares the previous and current phases to identify forward movement,
 * backward movement, or no change. Returns INVALID_TRANSITION for invalid
 * phase transitions.

 * @param previous_phase Pointer to the previous phase data. Updated after a
 valid
 * transition.
 * @param current_phase Pointer to the current phase data.

 * @return uint32_t The rotation direction, or an invalid transition value if
 * the transition is not valid.
 */
int32_t phase_change(PhaseData *previous_phase, PhaseData *current_phase);

#endif
