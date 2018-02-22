#include "include/display.h"
#include "include/graphics.h"
#include "include/io.h"
#include "include/registers.h"

#define LEFT 0
#define RIGHT 1

int main(void) {
    set_btn(1, SET);
    set_btn(2, SET);
    set_btn(3, SET);
    set_btn(4, SET);

    set_sw(1, SET);
    set_sw(2, SET);

    set_led(1, SET);

    init_display();

    int x = 0;
    int y = 0;

    Object player;
    set_object(player, 5, 10);

    while (1) {
        if (get_sw(1)) clear();
        if (get_sw(2)) print(16, 16, "Hi!");

        if (get_btn(1)) move(player, RIGHT);
        if (get_btn(2)) move(player, LEFT);
        if (get_btn(3)) y++;
        if (get_btn(4)) y--;

        // set_object(player, x, y);

        render();
        delay(45000);
    }

    return 0;
}