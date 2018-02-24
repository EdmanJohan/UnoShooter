#include "display_ui.h"
#include "registers.h"

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

/**
 * Enables a single pixel on the buffer at a specific location within the
 * 128 x 32 resolution buffer.
 * @param x The x-value of the pixel.
 * @param y The y-value of the pixel.
 */
void set_pixel(int x, int y) {
    int i = y / PIXEL_UNIT;
    buffer[i * HEIGHT + x] |= 1 << (y - i * PIXEL_UNIT);
}

void unset_pixel(int x, int y) {
    int i = y / PIXEL_UNIT;
    buffer[i * HEIGHT + x] &= ~(1 << (y - i * PIXEL_UNIT));
}

void draw(Object o) {
    int i, j;

    for (i = 0; i <= o.size / 2; i++)
        for (j = 0; j <= o.size / 2; j++) set_pixel(i + o.posX, j + o.posY);
}

void potentio_move(Object* o) {
    int i, j;

    for (i = 0; i <= o->size / 2; i++)
        for (j = 0; j <= o->size / 2; j++)
            unset_pixel(i + o->posX, j + o->posY);

    o->posX += o->posX - ADC1BUF0 % HEIGHT;

    for (i = o->posX; i < o->size; i++)
        for (j = o->posY; j < o->size; j++) set_pixel(i, j);
}

void move(Object* o, int dir) {
    int i, j;

    for (i = 0; i <= o->size / 2; i++)
        for (j = 0; j <= o->size / 2; j++)
            unset_pixel(i + o->posX, j + o->posY);

    switch (dir) {
        case LEFT:
            if (o->posY + o->size < WIDTH + 3) o->posY += o->velocity;
            break;
        case RIGHT:
            if (o->posY > 1) o->posY -= o->velocity;
            break;
        case UP:
            if (o->posX + o->size < HEIGHT + 4) o->posX += o->velocity;
            break;
        case DOWN:
            if (o->posX > 10) o->posX -= o->velocity;
            break;
    }

    for (i = o->posX; i < o->size; i++)
        for (j = o->posY; j < o->size; j++) set_pixel(i, j);
}

void draw_borders() {
    int i;
    for (i = 0; i < HEIGHT; i++) {
        set_pixel(i, 0);
        set_pixel(i, WIDTH - 1);
    }
}

void game_over() {
    print(0, 1, "You");
    print(28, 1, "lost");
    print(60, 1, "!");
    stop_timer();
}
