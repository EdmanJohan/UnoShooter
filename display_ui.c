#include "includes/display_ui.h"
#include "includes/registers.h"

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

void draw(Object o) {
        int i, j;

        for (i = 0; i <= o.size / 2; i++)
                for (j = 0; j <= o.size / 2; j++) set_pixel(i + o.posX, j + o.posY);
}

void erase(Object o) {
        int i, j;

        for (i = 0; i <= o.size / 2; i++)
                for (j = 0; j <= o.size / 2; j++) unset_pixel(i + o.posX, j + o.posY);
}

void move(Object* o, int dir) {
        erase(*o);

        switch (dir) {
        case LEFT:
                if (o->posY + o->size < WIDTH + 3) o->posY += o->velocity;
                break;
        case RIGHT:
                if (o->posY > 1) o->posY -= o->velocity;
                break;
        case UP:
                if (o->posX + o->size < HEIGHT + 4) o->posX += o->velocity;
                break;
        case DOWN:
                if (o->posX > 10) o->posX -= o->velocity;
                break;
        }

        draw(*o);
}

void potentio_move(Object* o) {
        AD1CON1SET |= 1 << 1;
        while (!(AD1CON1 & (1 << 1)))
                ;
        while (!(AD1CON1 & 1))
                ;

        erase(*o);
        if (o->posX < HEIGHT - o->size)
                o->posX = ADC1BUF0 / 8;
        else
                o->posX = HEIGHT - o->size - 1;
        draw(*o);
}

void draw_borders() {
        int i;
        for (i = 0; i < HEIGHT; i++) {
                set_pixel(i, 0);
                set_pixel(i, WIDTH - 1);
        }
}

void game_over() {
        print(0, 1, "You");
        print(28, 1, "lost");
        print(60, 1, "!");
        stop_timer();
}
