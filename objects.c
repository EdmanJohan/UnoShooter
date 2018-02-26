#include "includes/objects.h"
#include "includes/display_ui.h"
#include "includes/standard.h"

/* === CONSTRUCTORS === */

/* Player Constructor */
void player_new(Object *p) {
    p->posX = 10;
    p->posY = 10;
    p->velX = 1;
    p->velY = 1;
    p->size = 10;
    p->is_alive = 1;
}

/* Rock Constructor */
void rock_new(Object *r) {
    r->posX = randint(120, 125);
    r->posY = randint(5, 25);
    r->velX = -randint(1, 2);
    r->velY = 0;
    r->size = randint(3, 7);
    r->is_alive = 1;
}

/* Shot Constructor */
void new_shot(Object *s, Object *p) {
    s->posX = p->posX;
    s->posY = p->posY;
    s->velX = 0;
    s->velY = -1;
    s->size = 1;
    s->is_alive = 1;
}

/* === MOVE === */

/* Object Move */
void object_move(Object *o) {
    o->posX += o->velX;
    o->posY += o->velY;
}

/* === UPDATE === */
void object_update(Object *o) {
    erase(*o);

    within_border(o);

    if (o->is_alive) {
        object_move(o);
        draw(*o);
    }
}

int get_posX(Object *o) { return o->posX + o->size / 2; }

int get_posY(Object *o) { return o->posY + o->size / 2; }

void within_border(Object *o) {
    if (o->posX < o->size) o->is_alive = 0;
}
