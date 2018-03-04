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

/**
 * Draws an object on the display
 * @param o    Object to draw
 * @param draw Draws if SET, erases if CLR
 */
void draw(Object o, int draw);

/**
 * Moves an object
 * @param o   The object to move
 * @param dir UP, DOWN, LEFT or RIGHT
 */
void move(Object* o, int dir);

/**
 * Enables the use of the potentiometer to adjust x-position.
 * @param The object to move.
 */
void potentio_move(Object* o);

/** Draws borders on the display */
void draw_borders();

#endif // _DISPLAY_UI_H_
