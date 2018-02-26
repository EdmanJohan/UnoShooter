#include "includes/display_ui.h"
#include "includes/registers.h"

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

void draw(Object o) {
    int i, j;

    for (i = 0; i <= o.size / 2; i++)
        for (j = 0; j <= o.size / 2; j++) set_pixel(i + o.posX, j + o.posY);
}

void erase(Object o) {
    int i, j;

    for (i = 0; i <= o.size / 2; i++)
        for (j = 0; j <= o.size / 2; j++) unset_pixel(i + o.posX, j + o.posY);
}

void move(Object* o, int dir) {
    erase(*o);

    switch (dir) {
        case LEFT:
            if (o->posY + o->size < WIDTH + 3) o->posY += o->velY;
            break;
        case RIGHT:
            if (o->posY > 1) o->posY -= o->velY;
            break;
        case UP:
            if (o->posX + o->size < HEIGHT + 4) o->posX += o->velX;
            break;
        case DOWN:
            if (o->posX > 10) o->posX -= o->velX;
            break;
    }

    draw(*o);
}

void potentio_move(Object* o) {
    AD1CON1SET = 1 << 1;
    while (!(AD1CON1 & (1 << 1)))
        ;
    while (!(AD1CON1 & 1))
        ;

    erase(*o);
    // slightly less than 1/8 to keep the player inside boundaries
    o->posX = 15 * ADC1BUF0 / 128;
    draw(*o);
}

void draw_borders() {
    int i;
    for (i = 0; i < HEIGHT; i++) {
        set_pixel(i, 0);
        set_pixel(i, WIDTH - 1);
    }
}
