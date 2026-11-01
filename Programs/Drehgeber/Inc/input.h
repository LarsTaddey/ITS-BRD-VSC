/**
 * @file input.h
 * @brief Declares functions for reading the board input pins and detecting
 * clear button events.
 */

#ifndef INPUT_H
#define INPUT_H

#include "data_types.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Reads the board's IN0 and IN1 input pins and stores their values and
 * the current timestamp in a PhaseData structure.
 * @param phase_data Pointer to the PhaseData structure in which to store the
 * input pin values and timestamp. timestamp
 * @return int32_t 0 if the inputs are read successfully, or -1 if an error
 * occurs.
 */
int32_t read_pins_and_time(PhaseData *phase_data);

/**
 * @brief Reads the S6 button input and determines whether the button is
 * pressed.
 * @return true if button was pressed, else false
 */
bool clear_button_pressed(void);

#endif