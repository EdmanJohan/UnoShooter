#include "display.h"
#include "display_ui.h"
#include "graphics.h"
#include "io.h"
#include "registers.h"

#include <stdlib.h>
#include "standard.h"

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3



void game_preload() {
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
        Rock r_1;
        r_1.posX = 115;
        r_1.posY = 10;
        r_1.size = rand() % 10;
        r_1.velocity = 1;

        Rock r_2;
        r_2.posX = 110;
        r_2.posY = 10;
        r_2.size = rand() % 20;
        r_2.velocity = 1;

        Player p;
        p.posX = 10;
        p.posY = 10;
        p.size = 10;
        p.velocity = 1;

        start_timer();

}

void game_draw(void) {

        while (1) {
                if (get_sw(1)) clear();

                if (next_frame()) {
                        if (TMR2 % 10) drop(&r_1);
                        if (TMR2 % 20) drop(&r_2);

                        if (get_btn(1)) move(&p, UP);
                        if (get_btn(2)) move(&p, LEFT);
                        if (get_btn(3)) move(&p, RIGHT);
                        if (get_btn(4)) move(&p, DOWN);

                        draw(p);
                        draw_borders();

                        if (p.posX == r_1.posX && p.posY == r_1.posY) game_over();

                        render();
                }
        }
}

int main(void) {
        game_preload();
        game_setup();

        while(1)
                game_draw();

        return 0;
}
