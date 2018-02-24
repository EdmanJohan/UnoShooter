#include "display.h"
#include "display_ui.h"
#include "graphics.h"
#include "io.h"
#include "objects.h"
#include "registers.h"

#include <stdlib.h>
#include "standard.h"

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

Player p;
Rock r;

// Rock rocks[5];
// Rock pebbles[10];

void game_preload() {
    init_analog();
    set_btn(1, SET);
    set_btn(2, SET);
    set_btn(3, SET);
    set_btn(4, SET);

    set_sw(1, SET);
    set_sw(2, SET);

    set_led(1, SET);

    init_display();
    init_timer();

}

void game_setup(void) {
    player_new(&p, 10, 10, 10);
    rock_new(&r);
    start_timer();
}

void game_movement(Object* o) {
    AD1CON1SET |= 1 << 1;
    while (!(AD1CON1 & (1 << 1)))
        ;
    while (!(AD1CON1 & 1))
        ;

    if (ADC1BUF0 > 582) 
        move(o, UP);
    if (ADC1BUF0 < 472) 
        move(o, DOWN);

    if (get_btn(4)) move(o, RIGHT);
    if (get_btn(3)) move(o, LET);
}

void game_draw(void) {
    if (get_sw(1)) clear();

    if (next_frame()) {
        draw_borders();
        // if (TMR2 % 10) drop(&r);

        // drop(&r);

        rock_update(&r);
        rock_show(&r);

        game_movement(&p);
        draw(p);

        // if (p.posX == r.posX && p.posY == r.posY) game_over();
        render();
    }
}

int main(void) {
    game_preload();
    game_setup();

    while (1) game_draw();

    return 0;
}
