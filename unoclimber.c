#include "includes/display.h"
#include "includes/display_ui.h"
#include "includes/graphics.h"
#include "includes/io.h"
#include "includes/objects.h"
#include "includes/registers.h"

#include <stdlib.h>
#include "includes/standard.h"

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

int currentScreen = 1;
int isInitialized = 0;
const int ROCKS = 5;

Player p;

Rock rock_array[5];
// ROCKS pebbles[10];

void init() {
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

void setup(void) {
        player_new(&p, 10, 10, 10);

        int i;
        for (i = 0; i < ROCKS; i++) rock_new(&rock_array[i]);

        start_timer();
        isInitialized = 1;
}

void game_movement(Object* o) {
        potentio_move(o);

        if (get_btn(4)) move(o, RIGHT);
        if (get_btn(3)) move(o, LEFT);
}


void menu_screen(void) {

}
void game_screen(void) {
        if (get_sw(1)) clear();

        draw_borders();

        int i;
        for (i = 0; i < ROCKS; i++) {
                rock_update(&rock_array[i]);
                rock_show(&rock_array[i]);
        }

        game_movement(&p);
        draw(p);

        //if (p.posX == r.posX && p.posY == r.posY)
        //        currentScreen = 2;
        render();
}

void finished_screen() {

}


void draw_display() {
        switch(currentScreen) {
        case 0:
                menu_screen();
                break;
        case 1:
                if (!isInitialized)
                        setup();
                if (next_frame())
                        game_screen();
                break;
        case 2:
                finished_screen();
                break;
        }
}


int main(void) {
        init();

        while (1)
                draw_display();

        return 0;
}
