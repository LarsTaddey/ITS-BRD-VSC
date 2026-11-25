/**
 * @file output.h
 * @brief Declares functions for controlling LEDs and LCD output.
 * @author Len Peschko (2840024), Lars Taddey (2880138) - HAW Hamburg
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
 * and reset the values on the display
 */
void set_default_output(void);

/**
 * @brief Displays the counter's binary value using LEDs 0-7. An turned on LED
 * represents a binary 1, while a turned off LED represents 0

 * @param phase_counter Current counter value to display in binary
 */
void set_counter_leds(int32_t phase_counter);

/**
 * @brief Turns on the LED indicating the forward direction
 */
void turn_on_forward_led();

/**
 * @brief Turns off the LED indicating the forward direction
 */
void turn_off_forward_led();

/**
 * @brief Turns on the LED indicating the backward direction
 */
void turn_on_backward_led();

/**
 * @brief Turns off the LED indicating the backward direction
 */
void turn_off_backward_led();

/**
 * @brief Turns on the LED indicating the error status
 */
void turn_on_error_led();

/**
 * @brief Turns off the LED indicating the error status
 */
void turn_off_error_led();

/**
 * @brief Turns off all counter LEDs
 */
void turn_off_counter_leds();

/**
 * @brief Display the calculated angle and angular speed. Updates the display
 * only when either value differs from the previously displayed value.
 * @param angle_data Pointer to the angle and angular speed data to display.
 */
void output_data_to_lcd(const AngleData *angle_data);

#endif