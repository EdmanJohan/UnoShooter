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

int current_screen = -1;
int is_initialized = 0;
int points = 0;
char char_points[4];
int counter = 36;
const int ROCKS = 5;

Player p;

Rock rock_array[5];
Shot shot_array[10];

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
    start_timer();
}

// modified from bhuwansahni's original code from
// https://stackoverflow.com/questions/9655202/how-to-convert-integer-to-string-in-c
char* itoa(int i, char b[]) {
    char const digit[] = "0123456789";
    char* p = b;
    int shifter = 100;

    // Move to where representation ends
    do {
        ++p;
        shifter = shifter / 10;
    } while (shifter);

    // Move back, inserting digits as u go
    *p = '\0';
    do {
        *--p = digit[i % 10];
        i = i / 10;
    } while (i);

    return b;
}

void spawn_rocks() {
    int i;
    for (i = 0; i < ROCKS; i++) rock_new(&rock_array[i]);
}

void setup(void) {
    is_initialized = 1;
    player_new(&p);
    spawn_rocks();
}

void game_movement(Object* o) {
    potentio_move(o);

    if (get_btn(4)) move(o, RIGHT);
    if (get_btn(3)) move(o, LEFT);
}

void menu_screen(void) {
    print(20, 0, "HIGH SCORE:", 11);
    print(20, 1, "1: START", 8);
    print(20, 2, "2: INFO", 7);
    print(20, 3, "3: CREDITS", 10);
    render();

    if (get_btn(1)) {
        current_screen = 3;  // start
        clear();
    }

    if (get_btn(2)) {
        current_screen = 1;  // instructions
        clear();
    }

    if (get_btn(3)) {
        current_screen = 2;  // credits
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
        current_screen = 0;  // menu
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
        current_screen = 0;  // menu
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
        // if (TMR2 % 20 == 0) spawn_rocks();

        // int i;
        // for (i = 0; i < ROCKS; i++) object_update(&rock_array[i]);

        // shot_new(&shot, &p);

        draw_borders();
        point_counter();
        game_movement(&p);

        render();
    }
}

void logo_screen(void) {
    print(28, 0, "UNOROCKET", 9);
    print(24, 2, "PRESS BTN4", 10);
    print(20, 3, "TO CONTINUE", 11);
    render();

    if (get_btn(4)) {
        current_screen = 0;
        clear();
    }
}

void finished_screen() { print(30, 3, "you ded lol", 11); }

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
