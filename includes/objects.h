#ifndef _OBJECT_H_
#define _OBJECT_H_
typedef int objectPointer[10][10];

typedef struct Object {
    float posX, posY, size, velX, velY;
    int is_alive, radius;
    objectPointer objForm;
} Object, Player, Rock, Shot;

int difficulty;

/* === CONSTRUCTORS === */

/* Player Constructor */
Player player_new();

/* Rock Constructor */
Rock rock_new();

/* Shot Constructor */
Shot shot_new(Object player);

/* === UPDATE === */
void player_update(Object *player);

void object_update(Object *object);

/* === MOVE === */
void object_move(Object *object);

int check_collision(Object o1, Object o2);

int within_screen(Object *object);

void within_border(Object *object);

#endif  // _OBJECT_H_
