#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#define PIXEL_UNIT 8
#define HEIGHT 128
#define WIDTH 32
#define PAGES 4
#define TOTAL_PIXELS (HEIGHT * WIDTH / PIXEL_UNIT)

typedef unsigned char byte;
typedef struct Object { int posX, posY, size; } Object, Player, Rock;
byte buffer[TOTAL_PIXELS];

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
