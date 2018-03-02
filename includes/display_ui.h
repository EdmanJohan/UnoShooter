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

void draw(Object o, int draw);

void move(Object* o, int dir);

void potentio_move(Object* o);

void draw_borders();

#endif // _DISPLAY_UI_H_
