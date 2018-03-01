#include "includes/display.h"
#include "includes/display_ui.h"
#include "includes/graphics.h"
#include "includes/io.h"
#include "includes/memory.h"
#include "includes/objects.h"
#include "includes/registers.h"

#include <stdlib.h>
#include "includes/standard.h"

#define MAX_AMMO 3
#define MAX_HEALTH 3

/* Game Variables */
int current_screen = -2;
int is_initialized = 0;

char char_points[4];
char char_high_score[4];
int high_score = 30;
int counter = 36;
int points = 0;

int invincible = 0;

int health = 3;
int ammo = 3;

int to_shoot = 0;

int shot_delay = 0;

Player p;
// Rock rock_array[10];
// Shot shot_array[MAX_AMMO];
Rock r;
Shot s;
int rock_count;
int shot_count;

/* Initialize Controls */
void init() {
    init_input();
    init_pin();
    srand(seed());

    i2c_init();
    write_data(0, "533", 3);

    init_display();
    init_timer(2);
    init_timer(3);
}

void spawn_object(char c) {
    switch (c) {
        case 'r':
            if (!r.is_alive) r = rock_new();
            break;
        case 's':
            if (!s.is_alive) {
                s = shot_new(p);
            }
            break;
    }
}

void setup(void) {
    clear();

    TRISE = 0;
    PORTE = 0xE7;

    health = MAX_HEALTH;

    rock_count = 0;
    shot_count = 0;
    p = player_new();
    start_timer(2);
    start_timer(3);
    is_initialized = 1;
}

void player_input(Object* o) {
    potentio_move(o);

    if (get_btn(4)) move(o, RIGHT);
    if (get_btn(3)) move(o, LEFT);
}

void menu_screen(void) {
    clear();
    read_data(0, char_high_score, 4);

    print(0, 0, "HIGH SCORE:", 11);
    print(90, 0, char_high_score, 3);
    print(20, 1, "1: START", 8);
    print(20, 2, "2: INFO", 7);
    print(20, 3, "3: CREDITS", 10);

    if (get_btn(1)) current_screen = 3;  // start

    if (get_btn(2)) current_screen = 1;  // instructions

    if (get_btn(3)) current_screen = 2;  // credits
}

void instructions_screen(void) {
    clear();

    print(6, 0, "AVOID OR SHOOT", 14);
    print(14, 1, "THE ROCKS TO", 12);
    print(20, 2, "STAY ALIVE!", 11);
    print(30, 3, "4: MENU", 7);
    render();

    if (get_btn(4)) current_screen = 0;  // menu
}

void credits_screen(void) {
    clear();

    print(25, 0, "A GAME BY", 9);
    print(35, 1, "JEDMEX", 6);
    print(15, 2, "PRODUCTIONS", 11);
    print(30, 3, "4: MENU", 7);
    render();

    if (get_btn(4)) current_screen = 0;  // menu
}

void point_counter() {
    if (points < 999) {
        counter++;

        if (counter > 35) {
            counter = 0;
            points++;
        }
    }

    to_char(points, char_points);
    print(104, 0, char_points, 3);
}

void finished_screen() {
    clear();

    if (points > high_score) {
        high_score = points;
        to_char(high_score, char_high_score);
        write_data(0, "char_high_score", 4);
    }

    print(20, 0, "Game over!", 10);
    print(20, 1, "Points:", 7);
    print(76, 1, char_points, 3);
    print(20, 3, "Press BTN4", 11);

    if (get_btn(4)) {
        current_screen = 0;
        is_initialized = 0;
    }

    points = 0;
}

void check_health() {
    if (IFS(0) & 0x9000) {
        invincible++;
        IFSCLR(0) = 0x9000;
    }

    if (invincible > 1) {
        invincible = 0;
        health--;
    }

    if (health <= 0) {
        p.is_alive = 0;
        set_led(8, CLR);
    } else {
        if (health == 2) set_led(6, CLR);
        if (health == 1) set_led(7, CLR);
    }
}

void shoot() {
    if (IFS(0) & 0x9000) {
        IFSCLR(0) = 0x9000;
        spawn_object('s');
    }
}

void game_screen(void) {
    if (!is_initialized) setup();

    // TODO: Add interrupt to prevent too many shots 0x10000
    if (next_frame()) {
        // if (get_btn(2)) shoot();

        if (TMR2 % 5 == 0) spawn_object('r');

        if (check_collision(p, r)) check_health();

        if (!p.is_alive) {
            r.is_alive = 0;
            stop_timer(2);
            current_screen = 4;
        }

        if (s.is_alive) {
            if (check_collision(s, r)) {
                s.is_alive = 0;
                r.is_alive = 0;
            }
        }

        object_update(&r);
        object_update(&s);

        draw_borders();
        point_counter();
        player_input(&p);
    }
}

void logo_screen(void) {
    int i;
    for (i = 0; i < 512; i++) buffer[i] = logo[i];

    if (get_btn(4)) {
        current_screen = 0;
        clear();
    }
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
    render();
}

int main(void) {
    init();

    while (1) draw_display();

    return 0;
}
