#include "includes/display.h"
#include "includes/display_ui.h"
#include "includes/graphics.h"
#include "includes/io.h"
#include "includes/objects.h"
#include "includes/registers.h"

#include <stdlib.h>
#include "includes/standard.h"



/* Game Variables */
int current_screen = -1;
int is_initialized = 0;
char char_points[4];
int counter = 36;

int points = 0;


Player p;
Rock rock_array[10];
Shot shot_array[10];
int rock_count;
int shot_count;

/* Initialize Controls */
void init() {
        init_input();
        init_pin();
        srand(seed());

        init_display();
        init_timer(2);
        start_timer(2);
}

void spawn_object(char c) {
        switch (c) {
        case 'r':
                if (rand() % 100 > 65 ) { // TODO: ADD TIMER4 REQ.
                        if (rock_array[rock_count].is_alive == 1)
                                break;
                        rock_count %= 10;
                        rock_array[rock_count++] = rock_new();
                }
                break;
        case 's':
                if (shot_array[shot_count].is_alive != 1) {
                        shot_count %= 10;
                        shot_array[shot_count++] = shot_new(p);
                }
                break;
        }
}


void setup(void) {
        rock_count = 0;
        shot_count = 0;
        p = player_new();

        is_initialized = 1;
}

void player_input(Object* o) {
        potentio_move(o);

        if (get_btn(4))
                move(o, RIGHT);
        if (get_btn(3))
                move(o, LEFT);
}

void menu_screen(void) {
        print(20, 0, "HIGH SCORE:", 11);
        print(20, 1, "1: START", 8);
        print(20, 2, "2: INFO", 7);
        print(20, 3, "3: CREDITS", 10);
        render();

        if (get_btn(1)) {
                current_screen = 3; // start
                clear();
        }

        if (get_btn(2)) {
                current_screen = 1; // instructions
                clear();
        }

        if (get_btn(3)) {
                current_screen = 2; // credits
                clear();
        }
}

void instructions_screen(void) {
        print(6, 0, "AVOID OR SHOOT", 14);
        print(14, 1, "THE ROCKS TO", 12);
        print(20, 2, "STAY ALIVE!", 11);
        print(30, 3, "4: MENU", 7);
        render();

        if (get_btn(4)) {
                current_screen = 0; // menu
                clear();
        }
}

void credits_screen(void) {
        print(25, 0, "A GAME BY", 9);
        print(35, 1, "JEDMEX", 6);
        print(15, 2, "PRODUCTIONS", 11);
        print(30, 3, "4: MENU", 7);
        render();

        if (get_btn(4)) {
                current_screen = 0; // menu
                clear();
        }
}

void point_counter() {
        if (points < 999) {
                counter++;

                if (counter > 35) {
                        counter = 0;
                        points += 1;
                        itoa(points, char_points);
                        print(104, 0, char_points, 3);
                }
        }
}

void game_screen(void) {
        if (!is_initialized) setup();

        if (next_frame()) {

                if (get_btn(2)) // TODO: Add interrupt to prevent too many shots
                        spawn_object('s');

                if (TMR2 % 5 == 0)
                        spawn_object('r');

                int i;
                for (i = 0; i < 10; i++) {
                        object_update(&rock_array[i]);
                        object_update(&shot_array[i]);
                }


                draw_borders();
                point_counter();
                player_input(&p);

                render();
        }
}

void logo_screen(void) {
        int i;
        for(i = 0; i < 512; i++)
                buffer[i] = logo[i];
        //print(28, 0, "UNOROCKET", 9);
        //print(24, 2, "PRESS BTN4", 10);
        //print(20, 3, "TO CONTINUE", 11);
        render();

        if (get_btn(4)) {
                current_screen = 0;
                clear();
        }
}

void finished_screen() {
        print(30, 3, "you ded lol", 11);
}

void draw_display() {
        switch (current_screen) {
        case 0:
                menu_screen();
                break;
        case 1:
                instructions_screen();
                break;
        case 2:
                credits_screen();
                break;
        case 3:
                game_screen();
                break;
        case 4:
                finished_screen();
                break;
        default:
                logo_screen();
                break;
        }
}

int main(void) {
        init();

        while (1) draw_display();

        return 0;
}
