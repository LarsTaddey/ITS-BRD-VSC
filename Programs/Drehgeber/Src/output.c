/**
 * @file output.c
 * @brief Implements output to LEDs and the LCD display.
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
#define BIT_MASK 16U
#define TXT_BUFFER 32U
#define TXT_LCD_LENGTH 10U

static char angle_label[] = "Angle (deg):";
static char speed_label[] = "Speed (deg/s):";
static const uint8_t X_COORDINATE = 3;
static const uint8_t X_OFFSET = 15;
static const uint8_t ANGLE_Y = 3;
static const uint8_t SPEED_Y = ANGLE_Y + 2;
/**
 * @brief Writes a floating-point value to the display to the given position
 * (x,y).
 * @note The value must not exceed 31 Bytes, exluding the null
 * terminator.
 * @param x_coordinate x-position
 * @param y_coordinate y-position
 * @param value flotating-point value
 */
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
static void print_values(uint8_t x_coordinate, uint8_t y_coordinate,
                         double value, char *previous_value, int index) {
   static char text[TXT_BUFFER];

   snprintf(text, sizeof(text), "%10.1f", value);
   if (previous_value[index] != text[index]) {
      lcdGotoXY(x_coordinate + index, y_coordinate);
      lcdPrintC(text[index]);
      previous_value[index] = text[index];
   }
}

/**
 * @brief Writes a formatted string to the display to the given position (x,y).
 * @note The formatted string must not exceed 31 Bytes, excluding the null
 * terminator
 * @param x_coordinate x-position
 * @param y_coordinate y-position
 * @param label adress of the char array
 */
static void print_labels(uint8_t x_coordinate, uint8_t y_coordinate,
                         char label[]) {
   lcdGotoXY(x_coordinate, y_coordinate);
   lcdPrintS(label);
}

/**
 * @brief Helper function to set or clear only specified GPIOE pins
 * @param mask bitmask for one or more LEDs
 * @param enabled true to enable, false to disable
 */
static void set_gpioe_leds(uint32_t mask, bool enabled) {
   if (enabled) {
      GPIOE->BSRR |= mask;
   } else {
      GPIOE->BSRR = mask << BIT_MASK;
   }
}

void output_init() {
   GUI_init(DEFAULT_BRIGHTNESS);
   print_labels(X_COORDINATE, ANGLE_Y, angle_label);
   print_labels(X_COORDINATE, SPEED_Y, speed_label);
}

void set_default_output() {

   output_counter_leds((PhaseData){0, 0, 0});
   output_forward_led(false);
   output_backward_led(false);
   output_error_led(false);
   output_display((AngleData){0.0, 0.0});
}

void output_counter_leds(PhaseData phase_now) {
   GPIOD->BSRR =
       (COUNTER_LED_MASK << BIT_MASK) | ((uint32_t)phase_now.phase_counter);
}

void output_forward_led(bool enabled) {
   set_gpioe_leds(FORWARD_LED_MASK, enabled);
}

void output_backward_led(bool enabled) {
   set_gpioe_leds(BACKWARD_LED_MASK, enabled);
}

void output_error_led(bool enabled) { set_gpioe_leds(ERROR_LED_MASK, enabled); }

void output_display(AngleData angle_data) {
   // needed to be able to compare previous and current values
   static char previous_angle[TXT_BUFFER];
   static char previous_speed[TXT_BUFFER];
   static uint8_t index = 0;
   static bool first = true;

   if (first) {
      print_values(X_COORDINATE + X_OFFSET, ANGLE_Y, angle_data.angle,
                   previous_angle, index);
   } else {

      print_values(X_COORDINATE + X_OFFSET, SPEED_Y, angle_data.angle_speed,
                   previous_speed, index);
   }

   ++index;
   if (index >= TXT_LCD_LENGTH) {
      first = !first;
      index = 0;
   }
}
