#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#define PIXEL_UNIT 8
#define HEIGHT 128
#define WIDTH 32
#define PAGES 4
#define TOTAL_PIXELS (HEIGHT * WIDTH / PIXEL_UNIT)

typedef unsigned char byte;
byte buffer[TOTAL_PIXELS];

/**
 * Enables a single pixel on the buffer at a specific location within the
 * 128 x 32 resolution buffer.
 * @param x The x-value of the pixel.
 * @param y The y-value of the pixel.
 * @author  Alex Diaz
 */
void set_pixel(int x, int y, int state);


/**
 * Initializes Timer2 with 1:256 pre-scaling, at 1 s.
 * Author: Alex Diaz, modified by Johan Edman
 */
void init_timer(int timer);

/**
 * Starts a Timer
 * Author: Alex Diaz, modified by Johan Edman
 */
void start_timer(int timer);

/**
 * Stops a Timer.
 * Author: Alex Diaz, modified by Johan Edman
 */
void stop_timer(int timer);

/**
 * Keeps the counter variable.
 * @return 1 is counter is full, else 0.
 * @author Alex Diaz
 */
int next_frame();

/**
 * Simple delay function.
 * @param del Number of cycles to "stall".
 * @author Alex Diaz
 */
void delay(int del);

/**
 * Initializes the SPI controller and the OLED display.
 * @author Fredrik Lundevall
 */
void init_display();

/**
 * Pushes the display[] array to the SPI buffer to be rendered on the display.
 * @author Fredrik Lundevall, modifed by Alex Diaz
 */
void render();

/**
 * Prints characters on the display.
 * @param x    Initial x-value position
 * @param line Row position, 0 ... 3
 * @param data The string to print
 * @author Fredrik Lundevall, modified by Alex Diaz
 */
void print(int x, int line, const char *data, const int len);

/**
 * Fills the display array with 0s.
 * Author: Alex Diaz
 */
void clear();

#endif  // _DISPLAY_H_
