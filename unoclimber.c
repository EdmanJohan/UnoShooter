#include "include/graphics.h"
#include "include/registers.h"
#include "include/display.h"
#include "include/io.h"

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

    while (1) {
        if (get_sw(1)) clear();
        if (get_sw(2)) print(16, 16, "Hi!");

        if (get_btn(1)) x++;
        if (get_btn(2)) x--;
        if (get_btn(3)) y++;
        if (get_btn(4)) y--;

        set_pixel(x, y);
        render();
        delay(45000);
    }

    return 0;
}