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

/* Updates the object. Toggles the Y-velocity if border hit.  */
void object_update(Object *object);

/* === MOVE === */
void object_move(Object *object);

/* Check if collision has happened by distance formula */
int check_collision(Object o1, Object o2);

/* Check if within screen */
int within_screen(Object *object);

/* Check if within game borders */
void within_border(Object *object);

#endif  // _OBJECT_H_
