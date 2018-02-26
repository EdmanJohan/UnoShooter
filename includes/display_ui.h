#ifndef _DISPLAY_UI_H_
#define _DISPLAY_UI_H_

#include "display.h"
#include "objects.h"

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

void draw(Object o);

void move(Object* o, int dir);

void potentio_move(Object* o);

void drop(Rock* r);

void draw_borders();

void menu_screen();

void game_over();

#endif // _DISPLAY_UI_H_
