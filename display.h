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
 * Initializes Timer2 with 1:256 pre-scaling, at 1 s.
 */
void init_timer();

/**
 * Starts Timer2.
 */
void start_timer();

/**
 * Stops Timer2.
 */
void stop_timer();

/** 
 * Keeps the counter variable.
 * @return 1 is counter is full, else 0.
 */
int next_frame();

/**
 * Simple delay function. 
 * @param del Number of cycles to "stall".
 */
void delay(int del);

/**
 * Initializes the SPI controller and the OLED display.
 */
void init_display();

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
void print(int x, int y, const char *data);

/**
 * Fills the SPI buffer and the display[] array with 0s.
 */
void clear();

#endif  // _DISPLAY_H_
