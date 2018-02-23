#include "objects.h"
#include <stdlib.h>



//Only rectangle size for now
void player_new(Object *player, int posX, int posY, int size) {
        player->posX = posX;
        player->posY = posY;
        player->size = size;
        player->velocity = 1;
}

void rock_new(Object *rock) {
        rock->posX = randint(120, 130);
        rock->posY = randint(5, 25);
        rock->velocity = randint(1, 5);
        rock->size = randint(3, 12);
}


int get_posX(Object *obj) {
        return obj->posX + obj->size/2;
}

int get_posY(Object *obj) {
        return obj->posY + obj->size/2;
}

int within_border(Object *obj) {
        if (get_posX(obj) < 0)
                return 1;
        else
                return 1;
}
