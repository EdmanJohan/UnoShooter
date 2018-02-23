#ifndef _DISPLAY_UI_H_
#define _DISPLAY_UI_H_

#include "display.h"

#define SET 1
#define CLR 0

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

#define PIXEL_UNIT 8
#define HEIGHT 128
#define WIDTH 32
#define PAGES 4
#define TOTAL_PIXELS (HEIGHT * WIDTH / PIXEL_UNIT)

typedef struct Object { int posX, posY, size, velocity; } Object, Player, Rock;

/**
 * Enables a single pixel on the buffer at a specific location within the
 * 128 x 32 resolution buffer.
 * @param x The x-value of the pixel.
 * @param y The y-value of the pixel.
 */
void set_pixel(int x, int y);

void unset_pixel(int x, int y);

void draw(Object o);

void move(Object* o, int dir);

void drop(Rock* r);

void draw_borders();

void game_over();

#endif // _DISPLAY_UI_H_