#ifndef _OBJECT_H_
#define _OBJECT_H_

typedef struct Object {
        int posX, posY, size, velocity;
} Object, Player, Rock;

void player_new(Object *player, int posX, int posY, int size);

void rock_new(Object *rock);

int within_border(Object *obj);

#endif
