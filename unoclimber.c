#include <stdlib.h>
#include "includes/display_ui.h"
#include "includes/graphics.h"
#include "includes/io.h"
#include "includes/memory.h"
#include "includes/registers.h"
#include "includes/standard.h"

#define MAX_AMMO 3
#define MAX_ROCK 5
#define MAX_HEALTH 3
#define POINTS 5

#define LOGO -1
#define MENU 0
#define GAME 1
#define CREDITS 2
#define HIGH_SCORE 3
#define FINISH 4

#define HS_MAX 999
#define HS_LEN 3

#define FI_ADDR 0x0001000
#define SE_ADDR 0x0002000
#define TH_ADDR 0x0003000

/* Game settings */
int current_screen;
int is_initialized;
int hs_is_updated;

/* Game variables */
int first_place, second_place, third_place;
char char_player_score[HS_LEN];
char char_first_place[HS_LEN];
char char_second_place[HS_LEN];
char char_third_place[HS_LEN];

int player_score, old_player_score;
int shot_count, rock_count;
int invincible, cooldown;
int player_health;
int ammo;

/* Objects */
Player p;
Shot shot_array[MAX_AMMO];
Rock rock_array[MAX_ROCK];

/* Initialize controls */
void init() {
    init_input();
    init_pin();
    srand(seed());

    i2c_init();

    if (get_sw(1)) {
        write_int(FI_ADDR, 0);
        write_int(SE_ADDR, 0);
        write_int(TH_ADDR, 0);
    }

    first_place = read_int(FI_ADDR);
    second_place = read_int(SE_ADDR);
    third_place = read_int(TH_ADDR);

    init_display();
    init_timer(2);
    init_timer(3);

    start_timer(2);
    start_timer(3);

    current_screen = LOGO;
}

void setup(void) {
    clear();

    player_health = MAX_HEALTH;
    ammo = MAX_AMMO;

    invincible = 1;
    cooldown = 1;

    player_score = 0;
    old_player_score = 0;

    difficulty = 0;
    shot_count = 0;
    rock_count = 0;

    hs_is_updated = 0;

    int i;
    for (i = 0; i < HS_LEN; i++)
        char_player_score[i] = 0;

    p = player_new();

    start_timer(2);
    start_timer(3);

    is_initialized = 1;
}

void player_input(Object* o) {
    potentio_move(o);

    if (get_btn(4))
        move(o, RIGHT);
    if (get_btn(3))
        move(o, LEFT);
}

void spawn_object(char c) {
    switch (c) {
        case 'r':
            rock_count %= (difficulty + 1);
            if (!rock_array[rock_count].is_alive)
                rock_array[rock_count] = rock_new();
            rock_count++;
            break;
        case 's':
            shot_count %= MAX_AMMO;
            if (!shot_array[shot_count].is_alive)
                shot_array[shot_count] = shot_new(p);
            shot_count++;
            break;
    }
}

void score_update() {
    to_char(player_score, char_player_score);
    print(104, 0, char_player_score, 3);
}

void increase_score() {
    if (player_score + POINTS > HS_MAX)
        player_score = HS_MAX;
    else
        player_score += POINTS;
}

void increase_difficulty() {
    if (player_score > old_player_score)
        if (player_score % 10 == 0 && difficulty < 5) {
            difficulty++;
            old_player_score = player_score;
        }
}

void check_health(int collision) {
    if (collision) {
        if (IFS(0) & 0x9000) {
            invincible++;
            IFSCLR(0) = 0x9000;
        }

        if (invincible > 1) {
            invincible = 0;
            player_health--;
        }
    }

    if (player_health == 3) {
        set_led(8, SET);
        set_led(7, SET);
        set_led(6, SET);
    } else if (player_health == 2) {
        set_led(6, CLR);
    } else if (player_health == 1) {
        set_led(7, CLR);
    } else if (player_health <= 0) {
        PORTE = 0;
        p.is_alive = 0;
    }
}

int check_ammo() {
    int count = 0;

    int i;
    for (i = 0; i < MAX_AMMO; i++)
        if (!shot_array[i].is_alive)
            count++;

    if (count == 3) {
        set_led(3, SET);
        set_led(2, SET);
        set_led(1, SET);
    } else if (count == 2) {
        set_led(3, CLR);
        set_led(2, SET);
        set_led(1, SET);
    } else if (count == 1) {
        set_led(3, CLR);
        set_led(2, CLR);
        set_led(1, SET);
    } else if (count == 0) {
        set_led(3, CLR);
        set_led(2, CLR);
        set_led(1, CLR);
    }

    return count;
}

void shoot() {
    if (IFS(0) & 0x9000) {
        IFSCLR(0) = 0x9000;
        if (!cooldown) {
            spawn_object('s');
            cooldown = 1;
        } else
            cooldown--;
    }
}

void logo_screen(void) {
    int i;
    for (i = 0; i < TOTAL_PIXELS; i++)
        buffer[i] = logo[i];

    if (get_btn(4)) {
        current_screen = MENU;
        clear();
    }
}

void menu_screen(void) {
    clear();

    print(10, 0, "1: START", 8);
    print(10, 1, "2: CREDITS", 10);
    print(10, 2, "3: HIGH SCORE", 13);

    if (get_btn(1))
        current_screen = GAME;

    if (get_btn(2))
        current_screen = CREDITS;

    if (get_btn(3))
        current_screen = HIGH_SCORE;
}

void game_screen(void) {
    if (!is_initialized)
        setup();

    if (next_frame()) {
        if (check_ammo() > 0 && get_btn(2))
            shoot();

        increase_difficulty();
        check_health(0);

        if (TMR2 % 5 == 0)
            spawn_object('r');

        int i;
        if (!p.is_alive) {
            for (i = 0; i < (difficulty + 1) % MAX_ROCK; i++)
                rock_array[i].is_alive = 0;
            stop_timer(2);
            current_screen = FINISH;
        }

        int j;
        for (i = 0; i < (difficulty + 1) % MAX_ROCK; i++) {
            if (rock_array[i].is_alive) {
                if (check_collision(p, rock_array[i]))
                    check_health(1);

                for (j = 0; j < MAX_AMMO; j++)
                    if (shot_array[j].is_alive)
                        if (check_collision(shot_array[j], rock_array[i])) {
                            increase_score();
                            rock_array[i].is_alive = 0;
                            shot_array[j].is_alive = 0;
                        }
            }
        }

        for (i = 0; i < MAX_AMMO; i++)
            if (shot_array[i].is_alive)
                object_update(&shot_array[i]);
        for (i = 0; i < (difficulty + 1) % MAX_ROCK; i++)
            if (rock_array[i].is_alive)
                object_update(&rock_array[i]);

        draw_borders();
        score_update();
        player_input(&p);
    }
}

void credits_screen(void) {
    clear();

    print(25, 0, "A GAME BY", 9);
    print(35, 1, "JEDMEX", 6);
    print(15, 2, "PRODUCTIONS", 11);
    print(30, 3, "4: MENU", 7);

    if (get_btn(4))
        current_screen = MENU;
}

void high_score_screen(void) {
    clear();

    to_char(first_place, char_first_place);
    to_char(second_place, char_second_place);
    to_char(third_place, char_third_place);

    print(0, 0, "1. ", 3);
    print(16, 0, char_first_place, 3);

    print(0, 1, "2. ", 3);
    print(16, 1, char_second_place, 3);

    print(0, 2, "3. ", 3);
    print(16, 2, char_third_place, 3);

    print(0, 3, "4: MENU", 7);

    if (get_btn(4))
        current_screen = MENU;
}

void finished_screen() {
    clear();

    if (!hs_is_updated) {
        if (player_score >= first_place) {
            third_place = second_place;
            second_place = first_place;
            first_place = player_score;

            write_int(TH_ADDR, third_place);
            write_int(SE_ADDR, second_place);
            write_int(FI_ADDR, first_place);

        }

        else if (player_score >= second_place) {
            third_place = second_place;
            second_place = player_score;

            write_int(TH_ADDR, third_place);
            write_int(SE_ADDR, second_place);

        }

        else if (player_score >= third_place) {
            third_place = player_score;

            write_int(TH_ADDR, third_place);
        }

        hs_is_updated = 1;
    }

    print(20, 0, "Game over!", 10);
    print(20, 1, "Score: ", 7);
    print(76, 1, char_player_score, 3);
    print(20, 3, "Press BTN4", 11);

    if (get_btn(4)) {
        current_screen = MENU;
        is_initialized = 0;
    }
}

void draw_display() {
    switch (current_screen) {
        case MENU:
            menu_screen();
            break;
        case GAME:
            game_screen();
            break;
        case CREDITS:
            credits_screen();
            break;
        case HIGH_SCORE:
            high_score_screen();
            break;
        case FINISH:
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

    while (1)
        draw_display();

    return 0;
}