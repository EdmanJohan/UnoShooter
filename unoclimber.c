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

const int ROCKS = 5;

Player p;
//ROCKS r;

Rock rock_array[5];
// ROCKS pebbles[10];

void game_preload() {
        set_btn(1, SET);
        set_btn(2, SET);
        set_btn(3, SET);
        set_btn(4, SET);

        set_sw(1, SET);
        set_sw(2, SET);

        init_pin();
        srand(seed());

        init_display();
        init_timer();
}

void game_setup(void) {
        player_new(&p, 10, 10, 10);

        int i;
        for (i = 0; i < ROCKS; i++)
                rock_new(&rock_array[i]);

        start_timer();
}

void game_movement(Object* o) {
        //AD1CON1SET = 0x2;
        //potentio_move(o);

        if (get_btn(4)) move(o, RIGHT);
        if (get_btn(3)) move(o, LEFT);
}

void game_draw(void) {
        int k = 0;
        int i;
        if (get_sw(1)) clear();

        if (next_frame()) {
                draw_borders();
                if (TMR2 % 20) k++;

                for (i = 0; i < ROCKS; i++) {
                        rock_update(&rock_array[i]);
                        rock_show(&rock_array[i]);
                }

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
