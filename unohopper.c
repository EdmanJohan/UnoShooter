#include "include/pic32mx.h"
#include "include/pic32mx_disp.h"
#include "include/pic32mx_io.h"

static const byte smiley[] = {8, 152, 128, 152, 88};
static const byte bg[] = {};

byte disp[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int main(void) {
    init_display();
    oled_update();

    print_image(0, bg);
    print_image(32, bg);
    print_image(64, bg);
    print_image(96, bg);

    set_btn(1, SET);
    set_btn(2, SET);

    int line = 0;

    while (1) {
        if (get_btn(1)) {
            remove_image(line, smiley);
            line += 1;
        }
        if (get_btn(2)) {
            remove_image(line, smiley);
            line -= 1;
        }

        print_image(line, smiley);
        sleep(25000);
    }

    return 0;
}