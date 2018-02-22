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

    // int x = 0;
    // int y = 0;

    Object player;
    add(player, 2, 2);

    while (1) {
        if (get_sw(1)) clear();
        // if (get_sw(2)) print(16, 16, "Hi!");

        if (get_btn(1)) move(player, RIGHT);
        if (get_btn(2)) move(player, LEFT);
        if (get_btn(3)) move(player, UP);
        if (get_btn(4)) move(player, DOWN);

        // add(player, x, y);

        render();
        delay(45000);
    }

    return 0;
}