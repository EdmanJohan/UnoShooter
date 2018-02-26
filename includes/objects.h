#ifndef _OBJECT_H_
#define _OBJECT_H_

typedef struct Object {
        int posX, posY, size, velocity, draw;
} Object, Player, Rock;

void player_new(Object *player, int posX, int posY, int size);

void rock_new(Object *rock);

void rock_update(Object *rock);

void rock_show(Object *rock);

void within_border(Object *obj);

void shoot(Object *l, Object *p);

#endif // _OBJECT_H_