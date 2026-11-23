/**
 * @file bmp_display.h
 * @author Len Peschko (2840024), Lars Taddey (2880138) - HAW Hamburg
 * @brief BMP decoding and display
 */

#include <stdint.h>
#include <stdio.h>

#include "BMP_types.h"
#include "LCD_GUI.h"
#include "bmp_display.h"
#include "input.h"
#include "lcd.h"
#include "read.h"

// just a random number to lock out insanely large images
#define MAX_BMP_DIMENSION 10240U

static void draw_pixel(uint32_t x, int32_t y, COLOR color) {
   // TODO this check should not be neccessary later
   if (x >= LCD_WIDTH || y < 0 || y >= LCD_HEIGHT)
      return;
   GUI_drawPoint((Coordinate){(POINT)x, (POINT)y}, color, DOT_PIXEL_1X1,
                 DOT_FILL_RIGHTUP);
}

/*
 * parse and check header
 */
static const char *parseHeader(uint32_t *width, uint32_t *height,
                               uint32_t *pixel_data_offset,
                               uint32_t *color_table_length,
                               char *lcdPrint_buffer,
                               size_t lcdPrint_buffer_size) {
   const char *error = "unexpected end of file";
   bool was_eof = false;

   uint16_t signature = read_uint16(&was_eof);
   if (was_eof)
      goto error;
   if (signature != BMP_SIGNATURE) {
      error = "Not a BMP file";
      goto error;
   }

   uint32_t file_size = read_uint32(&was_eof);
   if (was_eof)
      goto error;
   snprintf(lcdPrint_buffer, lcdPrint_buffer_size, "File size: %lu bytes",
            (unsigned long)file_size);
   lcdPrintlnS(lcdPrint_buffer);

   (void)read_uint32(&was_eof); /* reserved */
   *pixel_data_offset = read_uint32(&was_eof);
   if (was_eof)
      goto error;
   snprintf(lcdPrint_buffer, lcdPrint_buffer_size,
            "Pixel data offset: %lu bytes", (unsigned long)*pixel_data_offset);
   lcdPrintlnS(lcdPrint_buffer);

   uint32_t dib_header_size = read_uint32(&was_eof);
   if (was_eof)
      goto error;
   snprintf(lcdPrint_buffer, lcdPrint_buffer_size, "DIB header size: %lu bytes",
            (unsigned long)dib_header_size);
   lcdPrintlnS(lcdPrint_buffer);
   if (dib_header_size != 40) {
      error = "DIB header size must be 40";
      goto error;
   }

   *width = read_uint32(&was_eof);
   if (was_eof)
      goto error;
   snprintf(lcdPrint_buffer, lcdPrint_buffer_size, "Image width: %lu pixels",
            (unsigned long)*width);
   lcdPrintlnS(lcdPrint_buffer);
   if (*width > MAX_BMP_DIMENSION) {
      error = "width too large";
      goto error;
   }

   *height = read_uint32(&was_eof);
   if (was_eof)
      goto error;
   snprintf(lcdPrint_buffer, lcdPrint_buffer_size, "Image height: %lu pixels",
            (unsigned long)*height);
   lcdPrintlnS(lcdPrint_buffer);
   if (*height > MAX_BMP_DIMENSION) {
      error = "height too large";
      goto error;
   }

   uint16_t planes = read_uint16(&was_eof);
   if (was_eof)
      goto error;
   if (planes != 1) {
      error = "planes must be 1";
      goto error;
   }

   uint16_t bits_per_pixel = read_uint16(&was_eof);
   if (was_eof)
      goto error;
   if (bits_per_pixel != 8) {
      error = "bpp must be 8";
      goto error;
   }

   uint32_t compression = read_uint32(&was_eof);
   if (was_eof)
      goto error;
   if (compression != BI_RLE8) {
      error = "compression must be RLE8";
      goto error;
   }

   uint32_t image_size = read_uint32(&was_eof);
   if (was_eof)
      goto error;
   snprintf(lcdPrint_buffer, lcdPrint_buffer_size, "Image size: %lu bytes",
            (unsigned long)image_size);
   lcdPrintlnS(lcdPrint_buffer);

   uint32_t x_pixels_per_meter = read_uint32(&was_eof);
   if (was_eof)
      goto error;
   snprintf(lcdPrint_buffer, lcdPrint_buffer_size, "X pixels per meter: %lu",
            (unsigned long)x_pixels_per_meter);
   lcdPrintlnS(lcdPrint_buffer);

   uint32_t y_pixels_per_meter = read_uint32(&was_eof);
   if (was_eof)
      goto error;
   snprintf(lcdPrint_buffer, lcdPrint_buffer_size, "Y pixels per meter: %lu",
            (unsigned long)y_pixels_per_meter);
   lcdPrintlnS(lcdPrint_buffer);

   *color_table_length = read_uint32(&was_eof);
   if (was_eof)
      goto error;
   if (!*color_table_length)
      *color_table_length = MAX_COLOR_TABLE_SIZE;
   snprintf(lcdPrint_buffer, lcdPrint_buffer_size,
            "Color table length: %lu colors",
            (unsigned long)*color_table_length);
   lcdPrintlnS(lcdPrint_buffer);
   if (*color_table_length > MAX_COLOR_TABLE_SIZE) {
      error = "palette too long for 8bpp";
      goto error;
   }

   uint32_t important_color_count = read_uint32(&was_eof);
   if (was_eof)
      goto error;
   snprintf(lcdPrint_buffer, lcdPrint_buffer_size, "Important color count: %lu",
            (unsigned long)important_color_count);
   lcdPrintlnS(lcdPrint_buffer);

   return NULL;

error:
   return error;
}

void drawBmp(void) {
   const char *error = "unexpected end of file";
   bool was_eof = false;
   char lcdPrint_buffer[80];
   uint32_t width, height, pixel_data_offset, color_table_length;
   COLOR color_table[MAX_COLOR_TABLE_SIZE] = {0};

   const char *header_error =
       parseHeader(&width, &height, &pixel_data_offset, &color_table_length,
                   lcdPrint_buffer, sizeof(lcdPrint_buffer));
   if (header_error != NULL) {
      error = header_error;
      goto error;
   }

   // present header data and wait for s5
   wait_for_s_i(BUTTON_S5);
   GUI_clear(BLACK);

   // parse color table
   for (uint32_t i = 0; i < color_table_length; i++) {
      uint8_t b = read_uint8(&was_eof);
      uint8_t g = read_uint8(&was_eof);
      uint8_t r = read_uint8(&was_eof);
      (void)read_uint8(&was_eof);
      if (was_eof)
         goto error;

      color_table[i] = (COLOR)(((uint16_t)(r >> 3) << 11) |
                               ((uint16_t)(g >> 2) << 5) | (b >> 3));
   }

   {
      uint32_t position = 14 + 40 + color_table_length * 4;
      if (pixel_data_offset < position) {
         error = "invalid pixel data offset";
         goto error;
      }
      while (position < pixel_data_offset) {
         (void)read_uint8(&was_eof);
         if (was_eof)
            goto error;
         position++;
      }
   }

   // draw pixel data
   uint32_t x = 0;
   int32_t y = (int32_t)height - 1;
   while (true) {
      uint8_t count = read_uint8(&was_eof);
      uint8_t value = read_uint8(&was_eof);
      if (was_eof)
         goto error;

      // encoded mode?
      if (count) {
         for (uint32_t i = 0; i < count; i++) {
            draw_pixel(x++, y, color_table[value]);
         }
      }
      // end of line?
      else if (value == 0) {
         x = 0;
         y--;
      }
      // end of image?
      else if (value == 1) {
         break;
      }
      // move position?
      else if (value == 2) {
         uint8_t dx = read_uint8(&was_eof);
         uint8_t dy = read_uint8(&was_eof);
         if (was_eof)
            goto error;
         x += dx;
         y -= dy;
      }
      // absolute mode?
      else {
         for (uint32_t i = 0; i < value; i++) {
            uint8_t index = read_uint8(&was_eof);
            if (was_eof)
               goto error;
            draw_pixel(x++, y, color_table[index]);
         }
         // padding needed?
         if (value & 1) {
            (void)read_uint8(&was_eof);
            if (was_eof)
               goto error;
         }
      }
   }
   return;

error:
   snprintf(lcdPrint_buffer, sizeof(lcdPrint_buffer), "ERROR: %s", error);
   lcdPrintlnS(lcdPrint_buffer);
}
