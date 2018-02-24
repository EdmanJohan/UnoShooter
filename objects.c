#include "objects.h"
#include "standard.h"
#include "display_ui.h"



//Only rectangle size for now
void player_new(Object *p, int posX, int posY, int size) {
        p->posX = posX;
        p->posY = posY;
        p->size = size;
        p->velocity = 1;
        p->draw = 1;
}

void rock_new(Object *r) {
        r->posX = 100; //randint(120, 125);
        r->posY = randint(5, 25);
        r->velocity = randint(1, 3);
        r->size = randint(3,7);
        r->draw = 1;
}

void rock_move(Object *r) {
        r->posX -= r->velocity;
}

void rock_update(Object *r) {
        int i, j;
        for (i = 0; i <= r->size / 2; i++)
                for (j = 0; j <= r->size / 2; j++)
                        unset_pixel(i + r->posX, j + r->posY);

        within_border(r);
        if (r->draw)
                rock_move(r);

}

void rock_show(Object *r) {
        if (r->draw) {
                int i, j;

                for (i = r->posX; i < r->size; i++)
                        for (j = r->posY; j < r->size; j++)
                                set_pixel(i, j);

                draw(*r);
        }
}


int get_posX(Object *o) {
        return o->posX + o->size/2;
}

int get_posY(Object *o) {
        return o->posY + o->size/2;
}

void within_border(Object *o) {
        if (get_posX(o) < 0)
                o->draw = 0;
}
