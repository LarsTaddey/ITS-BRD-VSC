/**
 * @file phase_change.h
 * @brief Declares a function for detecting rotary encoder phase transitions and
 * updating the rotation direction and transition counter.
 */

#ifndef PHASE_CHANGE_H
#define PHASE_CHANGE_H

#include "data_types.h"
#include <stdint.h>

/**
 * @brief Detects rotary encoder phase transitions, updates the rotation
 * direction and transition counter.
 * @param phase_last Pointer to the previous phase data. Updated after a valid
 * transition.
 * @param phase_now Pointer to the current phase data.
 * @param counter Pointer to the transition counter, incremented when a valid
 * phase transition occured.
 * @return uint32_t The rotation direction, or an invalid transition value if
 * the transition is not valid.
 */
int32_t phase_change(PhaseData *phase_last, const PhaseData *phase_now,
                     uint32_t *counter);

#endif
