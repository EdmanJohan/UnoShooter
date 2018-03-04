#include "includes/display_ui.h"
#include "includes/registers.h"

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

#define SET 1
#define CLR 0

/**
 * Draws an object on the display
 * @param o    Object to draw
 * @param draw Draws if SET, erases if CLR
 */
void draw(Object o, int draw) {
    int i, j;

    for (i = 0; i < o.size; i++)
        for (j = 0; j < o.size; j++)
            if (o.objForm[i][j]) {
                if (draw)
                    set_pixel(i + o.posX, j + o.posY, SET);
                else
                    set_pixel(i + o.posX, j + o.posY, CLR);
            }
}

/**
 * Moves an object
 * @param o   The object to move
 * @param dir UP, DOWN, LEFT or RIGHT
 */
void move(Object* o, int dir) {
    draw(*o, CLR);

    switch (dir) {
        case LEFT:  // UP
            o->velX = 0;
            o->velY = 1;
            break;
        case RIGHT:  // DOWN
            o->velX = 0;
            o->velY = -1;
            break;
    }
    player_update(o);
}

/**
 * Enables the use of the potentiometer to adjust x-position.
 * @param The object to move.
 */
void potentio_move(Object* o) {
    AD1CON1SET = 1 << 1;
    while (!(AD1CON1 & (1 << 1)))
        ;
    while (!(AD1CON1 & 1))
        ;

    draw(*o, CLR);
    // less than 1/8 to keep the player inside boundaries
    o->posX = 11 * ADC1BUF0 / 128 + 1;
    draw(*o, SET);
}

/** Draws borders on the display */
void draw_borders() {
    int i;
    for (i = 0; i < HEIGHT; i++) set_pixel(i, WIDTH - 1, SET);
    for (i = 0; i < HEIGHT - 26; i++) set_pixel(i, 0, SET);
    for (i = 0; i < 9; i++) set_pixel(HEIGHT - 26, i, SET);
    for (i = HEIGHT - 26; i < HEIGHT; i++) set_pixel(i, 9, SET);
}
