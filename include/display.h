#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/**
 * Simple delay function. TO BE REPLACED!
 * @param del Number of cycles to "stall".
 */
void delay(int del);

/**
 * Initializes the SPI controller and the OLED display.
 */
void init_display();

/**
 * Enables a single pixel on the display at a specific location within the 
 * 128 x 32 resolution display.
 * @param x The x-value of the pixel.
 * @param y The y-value of the pixel.
 */
void set_pixel(int x, int y);

/**
 * Pushes the display[] array to the SPI buffer to be rendered on the display.
 */
void render();

/**
 * Prints characters on the display. NON-FUNCTIONAL!
 * @param x    [description]
 * @param y    [description]
 * @param data [description]
 */
void print(int x, int y, const char* data);

/**
 * Fills the SPI buffer and the display[] array with 0s.
 */
void clear();

#endif // _DISPLAY_H_
