/**
 * @file output.c
 * @brief Implements output to LEDs and the LCD display.
 * @author Len Peschko (2840024), Lars Taddey (2880138) - HAW Hamburg
 */

#include "output.h"
#include "LCD_GUI.h"
#include "data_types.h"
#include "lcd.h"
#include "stm32f429xx.h"
#include <stdint.h>
#include <stdio.h>

#define FORWARD_LED_MASK 0x80U
#define BACKWARD_LED_MASK 0x40U
#define ERROR_LED_MASK 0x20U
#define COUNTER_LED_MASK 0xFFU
#define MONITOR_GPIOE_MASK 0x01U
#define GPIO_CLEAR_MASK 16U
#define TXT_BUFFER 32U
#define TXT_LCD_LENGTH 10U

static char angle_label[] = "Angle (deg):";
static char speed_label[] = "Speed (deg/s):";
static const uint8_t X_COORDINATE = 3;
static const uint8_t X_OFFSET = 12;
static const uint8_t ANGLE_Y = 3;
static const uint8_t SPEED_Y = ANGLE_Y + 2;

/**
 * @brief Updates a single character on the LCD display if its value has
 * changed.
 * Formats the specified value as a right-aligned, ten-character string with
 * one decimal place. Compares the character at the specified index with the
 * previously displayed character and updates the LCD only if a change is
 * detected.

 * @param x_coordinate x-coordinate on the LCD
 * @param y_coordinate y-coordinate of the LCD
 * @param value Numeric value to format and display
 * @param previous_value Buffer containing the previously displayed characters
 * @param index Index of the character to compare and potentially update
 */
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
static void print_values(uint8_t x_coordinate, uint8_t y_coordinate,
                         double value, char *previous_value, int index) {
   char text[TXT_LCD_LENGTH + 1];

   snprintf(text, sizeof(text), "%10.1f", value);
   if (previous_value[index] != text[index]) {
      lcdGotoXY(x_coordinate + index, y_coordinate);
      lcdPrintC(text[index]);
      previous_value[index] = text[index];
   }
}

/**
*
* @brief Displays a label on the LCD at the specified coordinates.
* Positions the LCD cursor at the given coordinates and prints the
* provided label.
*
* @param x_coordinate X-coordinate on the LCD where the label starts.
* @param y_coordinate Y-coordinate of the LCD row.

@param label String containing the text to display.
*/
static void print_label(uint8_t x_coordinate, uint8_t y_coordinate,
                        char label[]) {
   lcdGotoXY(x_coordinate, y_coordinate);
   lcdPrintS(label);
}

void output_init() {
   GUI_init(DEFAULT_BRIGHTNESS);
   print_label(X_COORDINATE, ANGLE_Y, angle_label);
   print_label(X_COORDINATE, SPEED_Y, speed_label);
}

void set_default_output() {

   // Turn off all LEDs
   turn_off_forward_led();
   turn_off_backward_led();
   turn_off_error_led();
   turn_off_counter_leds();
   set_counter_leds(0);

   // Output default data to LCD
   output_data_to_lcd(&(AngleData){0.0, 0.0});
}

void turn_on_forward_led() { GPIOE->BSRR = FORWARD_LED_MASK; }

void turn_off_forward_led() {
   GPIOE->BSRR = FORWARD_LED_MASK << GPIO_CLEAR_MASK;
}

void turn_on_backward_led() { GPIOE->BSRR = BACKWARD_LED_MASK; }

void turn_off_backward_led() {
   GPIOE->BSRR = BACKWARD_LED_MASK << GPIO_CLEAR_MASK;
}

void turn_on_error_led() { GPIOE->BSRR = ERROR_LED_MASK; }

void turn_off_error_led() { GPIOE->BSRR = ERROR_LED_MASK << GPIO_CLEAR_MASK; }

void set_counter_leds(int32_t phase_counter) {
   turn_off_counter_leds();
   GPIOD->BSRR = (uint32_t)phase_counter;
}

void turn_off_counter_leds() {
   GPIOD->BSRR = COUNTER_LED_MASK << GPIO_CLEAR_MASK;
}

void set_monitor_pin_high() { GPIOE->BSRR = MONITOR_GPIOE_MASK; }

void set_monitor_pin_low() {
   GPIOE->BSRR = MONITOR_GPIOE_MASK << GPIO_CLEAR_MASK;
}

void output_data_to_lcd(const AngleData *angle_data) {
   // Needed to be able to compare previous and current values
   static char previous_angle[TXT_LCD_LENGTH];
   static char previous_speed[TXT_LCD_LENGTH];
   static uint8_t index = 0;
   static bool first = true;

   // Only update one of the two values at once. If first one is done then do
   // the second one
   if (first) {
      print_values(X_COORDINATE + X_OFFSET, ANGLE_Y, angle_data->angle,
                   previous_angle, index);
   } else {

      print_values(X_COORDINATE + X_OFFSET, SPEED_Y, angle_data->angle_speed,
                   previous_speed, index);
   }

   ++index;
   if (index >= TXT_LCD_LENGTH) {
      first = !first;
      index = 0;
   }
}
