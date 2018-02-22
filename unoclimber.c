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

    Object obj;
    obj.x = x;
    obj.y = y;

    add(obj);

    while (1) {
        if (get_sw(1)) clear();
        // if (get_sw(2)) print(16, 16, "Hi!");

        if (get_btn(1)) move(&obj, RIGHT); 
        if (get_btn(2)) move(&obj, LEFT); 
        if (get_btn(3)) move(&obj, UP); 
        if (get_btn(4)) move(&obj, DOWN); 

        add(obj);
        render();
        delay(50000);
    }

    return 0;
}