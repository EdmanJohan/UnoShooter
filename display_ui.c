#include "display_ui.h"

#define SET 1
#define CLR 0

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

void draw_player(Player p) {
    int i, j;

    for (i = 0; i <= p.size / 2; i++)
        for (j = 0; j <= p.size / 2; j++) set_pixel(i + p.posX, j + p.posY);
}

void move_player(Player* p, int dir) {
    int i, j;

    for (i = 0; i <= (*p).size / 2; i++)
        for (j = 0; j <= (*p).size / 2; j++)
            unset_pixel(i + (*p).posX, j + (*p).posY);

    switch (dir) {
        case RIGHT:
            if ((*p).posX + (*p).size < HEIGHT + 4) (*p).posX++;
            break;
        case LEFT:
            if ((*p).posX > 10) (*p).posX--;
            break;
        case DOWN:
            if ((*p).posY + (*p).size < WIDTH + 3) (*p).posY++;
            break;
        case UP:
            if ((*p).posY > 1) (*p).posY--;
            break;
    }

    for (i = (*p).posX; i < (*p).size; i++)
        for (j = (*p).posY; j < (*p).size; j++) set_pixel(i, j);
}

void draw_rock(Rock r) { set_pixel(r.posX, r.posY); }

void move_rock(Rock* r, int dir) {
    unset_pixel((*r).posX, (*r).posY);
    switch (dir) {
        case RIGHT:
            (*r).posX++;
            break;
        case LEFT:
            (*r).posX--;
            break;
        case UP:
            (*r).posY++;
            break;
        case DOWN:
            (*r).posY--;
            break;
    }
}

void draw_borders() {
    int i;
    for (i = 0; i < HEIGHT; i++) {
        set_pixel(i, 0);
        set_pixel(i, WIDTH - 1);
    }
}