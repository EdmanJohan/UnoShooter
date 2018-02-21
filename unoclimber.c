#include "include/pic32mx.h"
#include "include/pic32mx_disp.h"
#include "include/pic32mx_io.h"

#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3
#define FORW 4
#define BACKW 5

#define DISPLAY_COL 32
#define SMILEY_SIZE 5

static byte disp[512];
static byte smiley[] = {0x58, 0x98, 0x80, 0x98, 0x58};

int x = 0;
int line = 0;

void move_page(int dir) {
    int i;
    int k = 0;

    switch (dir) {
        case FORW:
            remove_image(line, disp);
            line += 32;
            x = 0;

            for (i = DISPLAY_COL - SMILEY_SIZE; i < DISPLAY_COL; i++) {
                disp[k] = disp[i];
                disp[i] = 0;
                k++;
            }
            break;

        case BACKW:
            remove_image(line, disp);
            line -= 32;
            x = 32 - (SMILEY_SIZE + 1);

            for (i = DISPLAY_COL - (SMILEY_SIZE + 1); i < DISPLAY_COL; i++) {
                disp[i] = disp[k];
                disp[k] = 0;
                k++;
            }
            break;
    }
}

void move(int dir) {
    int i;
    switch (dir) {
        case RIGHT:
            if (x > 32 - (SMILEY_SIZE + 1)) move_page(FORW);

            for (i = DISPLAY_COL - 1; i > 0; i--)
                if (disp[i - 1] != 0) {
                    disp[i] = disp[i - 1];
                    disp[i - 1] = 0;
                }
            x++;
            break;

        case LEFT:
            x--;
            if (x < SMILEY_SIZE) move_page(BACKW);

            for (i = 0; i < DISPLAY_COL - 1; i++)
                if (disp[i + 1] != 0) {
                    disp[i] = disp[i + 1];
                    disp[i + 1] = 0;
                }
            break;
    }
}

int main(void) {
    init_display();
    update_display();
    clear_display();

    set_btn(1, SET);
    set_btn(2, SET);

    int i;
    for (i = 0; i < SMILEY_SIZE; i++) disp[i] = smiley[i];

    while (1) {
        if (get_btn(1)) move(RIGHT);
        if (get_btn(2)) move(LEFT);

        sleep(65000);
        print_image(line, disp);
    }

    return 0;
}