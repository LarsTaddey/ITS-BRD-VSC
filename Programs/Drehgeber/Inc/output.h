/**
 * @file output.h
 * @brief Declares functions for controlling board outputs, including LEDs and
 * the LCD display.
 */

#ifndef OUTPUT_H
#define OUTPUT_H

#include "data_types.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initializes the LCD display and prints default labels
 */
void output_init(void);

/**
 * @brief Sets the default output configuration for the board. Turn off all LEDs
 * and reset the values on the display.
 */
void set_default_output(void);

/**
 * @brief Displays the counter's binary value using LEDs 0-7. An turned on LED
 * represents a binary 1, while a turned off LED represents 0.
 * @param counter The counter value to display
 */
void output_counter_leds(uint32_t counter);

/**
 * @brief Turns the forward direction LED (D23) on or off.
 * @param enabled Set to true to turn the LED on, or false to turn it off.
 */
void output_forward_led(bool enabled);

/**
 * @brief Turns the backward direction LED (D22) on or off.
 * @param enabled Set to true to turn the LED on, or false to turn it off.
 */
void output_backward_led(bool enabled);

/**
 * @brief Turns the error LED (D21) on or off.
 * @param enabled Set to true to turn the LED on, or false to turn it off.
 */
void output_error_led(bool enabled);

/**
 * @brief Display the calculated angle and angular speed. Updates the display
 * only when either value differs from the previously displayed value.
 * @param angle_data Pointer to the angle and angular speed data to display.
 */
void output_display(AngleData *angle_data);

#endif