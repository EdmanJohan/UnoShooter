#include "include/display.h"
#include "include/graphics.h"
#include "include/io.h"
#include "include/registers.h"

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

    int x = 10;
    int y = 10;

    Player p;
    p.posX = x;
    p.posY = y;
    p.size = 10;

    Rock r;
    r.posX = x;
    r.posY = y;
    r.size = 5;

    draw_borders();

    while (1) {
        if (get_sw(1)) clear();
        // if (get_sw(2)) print(16, 16, "Hi!");

        if (get_btn(1)) move_player(&p, RIGHT); 
        if (get_btn(2)) move_player(&p, DOWN); 
        if (get_btn(3)) move_player(&p, UP); 
        if (get_btn(4)) move_player(&p, LEFT); 
        
        // if (get_btn(1)) move_rock(&r, RIGHT); 
        // if (get_btn(2)) move_rock(&r, LEFT); 
        // if (get_btn(3)) move_rock(&r, UP); 
        // if (get_btn(4)) move_rock(&r, DOWN); 

        draw_player(p);
        // draw_rock(r);

        render();
        delay(50000);
    }

    return 0;
}