#include "includes/objects.h"
#include "includes/display_ui.h"
#include "includes/standard.h"

//#include "includes/registers.h"
//#include <stdlib.h>

// /* === CONSTRUCTORS === */
// int rock_count = 0;
// int shot_count = 0;

/* Player Constructor */
Player player_new() {
    Object p;
    p.posX = 10;
    p.posY = 10;
    p.velX = 1;
    p.velY = 1;
    p.size = 10;
    p.is_alive = 1;
    p.radius = 5;

    const int p_shape[10][10] = {
        {1, 1, 0, 0, 1, 1, 0, 0, 1, 1}, {1, 1, 1, 0, 1, 1, 0, 1, 1, 1},
        {1, 0, 1, 0, 1, 1, 0, 1, 0, 1}, {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0}, {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 1, 0, 0}, {0, 0, 1, 0, 1, 1, 0, 1, 0, 0},
        {0, 0, 1, 0, 1, 1, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
    };

    int i, j;
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++) p.objForm[i][j] = p_shape[i][j];

    return p;
}

/* Rock Constructor */
Rock rock_new() {
    Rock r;
    r.size = 10;
    r.posX = randint(120, 130);
    r.posY = randint(5, 30 - r.size);
    r.velX = -randfloat(0.5, 1.2);
    r.velY = 0;
    r.is_alive = 1;
    r.radius = 4;

    const int r_shape[10][10] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    int i, j;
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++) r.objForm[i][j] = r_shape[i][j];

    return r;
}

/* Shot Constructor */
Shot shot_new(Object p) {
    Shot s;
    s.posX = p.posX;
    s.posY = p.posY;
    s.velX = 1;
    s.velY = 0;
    s.size = 10;
    s.is_alive = 1;
    s.radius = 2;

    int s_shape[10][10] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    int i, j;
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++) s.objForm[i][j] = s_shape[i][j];

    return s;
}

/* === MOVE === */

/* Object Move */
void object_move(Object *o) {
    o->posX += o->velX;
    o->posY += o->velY;
}

/* === UPDATE === */
void player_update(Object *p) {
    if (within_screen(p)) object_move(p);
    draw(*p, SET);
}
void object_update(Object *o) {
    draw(*o, 0);
    within_border(o);

    if (o->is_alive) {
        draw(*o, 0);
        object_move(o);

        if (within_screen(o)) draw(*o, 1);
    }
}

/* Check if within game borders */
void within_border(Object *o) {
    if (o->posX < 0 || o->posX > 150 || o->posY < 0 || o->posY > 31)
        o->is_alive = 0;
}

/* Check if within screen */
int within_screen(Object *o) {
    return (o->posX > 0 && o->posX < (128 - o->size) &&
            (o->posY + o->velY) > 1 && (o->posY + o->size + o->velY) < 31);
}

int check_collision(Object dis, Object dat) {
    return dist(dis.posX, dis.posY, dat.posX, dat.posY) < dis.radius;
}
