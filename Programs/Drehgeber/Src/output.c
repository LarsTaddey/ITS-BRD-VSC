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

static const char ANGLE_LABEL[] = "Angle (deg):";
static const char SPEED_LABEL[] = "Speed (deg/s):";
static const uint8_t X_COORDINATE = 3;
static const uint8_t X_OFFSET = 15;
static const uint8_t ANGLE_Y = 3;
static const uint8_t SPEED_Y = ANGLE_Y + 2;
static char text[TXT_BUFFER];
static AngleData dummy_data = {.angle = 0.0, .angle_speed = 0.0};
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
                         double value) {
   lcdGotoXY(x_coordinate, y_coordinate);
   snprintf(text, sizeof(text), "%.2f", value);
   lcdPrintS(text);
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
                         const char *label) {
   lcdGotoXY(x_coordinate, y_coordinate);
   snprintf(text, sizeof(text), "%s", label);
   lcdPrintS(text);
}

/**
 * @brief Helper function to set or clear only specified GPIOE pins
 * @param mask bitmask for one or more LEDs
 * @param enabled true to enable, false to disable
 */
static void set_gpioe_leds(uint32_t mask, bool enabled) {
   if (enabled) {
      GPIOE->ODR |= mask;
   } else {
      GPIOE->ODR &= ~mask;
   }
}

void output_init() {
   GUI_init(DEFAULT_BRIGHTNESS);
   print_labels(X_COORDINATE, ANGLE_Y, ANGLE_LABEL);
   print_labels(X_COORDINATE, SPEED_Y, SPEED_LABEL);
}

void set_default_output() {

   output_counter_leds(0U);
   output_forward_led(false);
   output_backward_led(false);
   output_error_led(false);
   output_display(&dummy_data);
}

void output_counter_leds(uint32_t counter) {
   GPIOD->ODR = (GPIOD->ODR & ~COUNTER_LED_MASK) | (counter & COUNTER_LED_MASK);
}

void output_forward_led(bool enabled) {
   set_gpioe_leds(FORWARD_LED_MASK, enabled);
}

void output_backward_led(bool enabled) {
   set_gpioe_leds(BACKWARD_LED_MASK, enabled);
}

void output_error_led(bool enabled) { set_gpioe_leds(ERROR_LED_MASK, enabled); }

void output_display(AngleData *angle_data) {
   // needed to be able to compare previous and current values
   static bool initialized = false;
   static double previous_angle = 0.0;
   static double previous_speed = 0.0;

   if (angle_data == NULL) {
      return;
   }
   if (!initialized || angle_data->angle != previous_angle) {
      print_values(X_COORDINATE + X_OFFSET, ANGLE_Y, angle_data->angle);
      previous_angle = angle_data->angle;
   }

   if (!initialized || angle_data->angle_speed != previous_speed) {
      print_values(X_COORDINATE + X_OFFSET, SPEED_Y, angle_data->angle_speed);
      previous_speed = angle_data->angle_speed;
   }

   initialized = true;
}
