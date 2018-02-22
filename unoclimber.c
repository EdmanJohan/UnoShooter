#include "display.h"
#include "display_ui.h"
#include "graphics.h"
#include "io.h"
#include "registers.h"

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

int main(void) {
    set_btn(1, SET);
    set_btn(2, SET);
    set_btn(3, SET);
    set_btn(4, SET);

    set_sw(1, SET);
    set_sw(2, SET);

    set_led(1, SET);

    init_display();

    Player p;
    p.posX = 10;
    p.posY = 10;
    p.size = 10;

    Rock r;
    r.posX = 120;
    r.posY = 10;
    r.size = 4;

    while (1) {
        if (get_sw(1)) clear();

        move_rock(&r, LEFT);

        // print(16, 16, "Hi!");

        if (get_btn(1)) move_player(&p, RIGHT);
        if (get_btn(2)) move_player(&p, DOWN);
        if (get_btn(3)) move_player(&p, UP);
        if (get_btn(4)) move_player(&p, LEFT);

        draw_player(p);
        draw_rock(r);
        draw_borders();

        render();
        delay(50000);
    }

    return 0;
}