#ifndef _OBJECT_H_
#define _OBJECT_H_

typedef struct Object {
        int posX, posY, size, vX, vY, isAlive;
} Object, Player, Rock, Shot;


/* === CONSTRUCTOS === */

/* Player Constructor */
void player_new(Object *player);

/* Rock Constructor */
void rock_new(Object *rock);

/* Shot Constructor */
void shot_new(Object *shot, Object *player);

/* === UPDATE === */
void object_update(Object *object);

/* === MOVE === */
void object_move(Object *object);

void rock_update(Object *rock);

void rock_show(Object *rock);

void within_border(Object *obj);

void shoot(Object *l, Object *p);

#endif // _OBJECT_H_
