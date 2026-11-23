/**
 * @file bmp_display.h
 * @author Len Peschko (2840024), Lars Taddey (2880138) - HAW Hamburg
 * @brief BMP decoding and display
 */

#ifndef BMP_DISPLAY_H
#define BMP_DISPLAY_H

/**
 * @brief read header of 8-bit RLE8 BMP and display
 * its fields. After S5 is pressed, decode palette and pixels onto the
 * LCD. Display potential errors.
 */
void drawBmp(void);

#endif
