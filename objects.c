#include "includes/objects.h"
#include "includes/display_ui.h"
#include "includes/standard.h"

/*
   TODO: Add various design to objects instead of rectangles.
 */

/* === CONSTRUCTORS === */
int rock_count = 0;
int shot_count = 0;

/* Player Constructor */
Player player_new() {
        Object p;
        p.posX = 10;
        p.posY = 10;
        p.velX = 1;
        p.velY = 1;
        p.size = 10;
        p.is_alive = 1;

        const int p_shape[10][10] = {
                {1, 1, 0, 0, 1, 1, 0, 0, 1, 1},
                {1, 1, 1, 0, 1, 1, 0, 1, 1, 1},
                {1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
                {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
                {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
                {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
                {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
                {0, 0, 1, 0, 1, 1, 0, 1, 0, 0},
                {0, 0, 1, 0, 1, 1, 0, 1, 0, 0},
                {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        };

        int i, j;
        for (i = 0; i < 10; i++)
                for (j = 0; j < 10; j++)
                        p.objForm[i][j] = p_shape[i][j];

        return p;
}

/* Rock Constructor */
Rock rock_new() {
        Rock r;
        r.size = 10;
        r.posX = randint(120, 130);
        r.posY = randint(r.size, 32-r.size);
        r.velX = -randfloat(0.1, 1.2);
        r.velY = 0;
        r.is_alive = 1;

        const int r_shape[10][10] = {
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
                {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
                {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
                {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        };

        int i, j;
        for (i = 0; i < 10; i++)
                for (j = 0; j < 10; j++)
                        r.objForm[i][j] = r_shape[i][j];

        return r;
}

/* Shot Constructor */
Shot shot_new(Object p) {
        Shot s;
        s.posX = p.posX;

        if (rand() % 2 == 1)
                s.posY = p.posY / 2;
        else
                s.posY = p.posY + p.size / 2;

        s.velX = 1;
        s.velY = 0;
        s.size = 10;
        s.is_alive = 1;

        int s_shape[10][10] = {
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
                {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
                {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
                {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        };

        int i, j;
        for (i = 0; i < 10; i++)
                for (j = 0; j < 10; j++)
                        s.objForm[i][j] = s_shape[i][j];

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
        draw(*p, 0);

        if (within_screen(p))
                object_move(p);
        draw(*p, 1);
}


void object_update(Object *o) {
        draw(*o, 0);
        within_border(o);

        if (o->is_alive) {
                object_move(o);

                if (within_screen(o))
                        draw(*o, 1);
        }
}

/* === PROPERTIES === */

/* Set X-Velocity */
void set_velX(Object *o, float velX) {
        o->velX = velX;
}

/* Set Y-Velocity */
void set_velY(Object *o, float velY) {
        o->velY = velY;
}

/* Get X-Position */
float get_posX(Object *o) {
        return o->posX;
}

/* Get Y-Position */
float get_posY(Object *o) {
        return o->posY;
}

/* Set X-Pos */
void set_posX(Object *o, float posX) {
        o->posX = posX;
}

/* Set Y-Pos */
void set_posY(Object *o, float posY) {
        o->posY = posY;
}

/* Check if within game borders */
void within_border(Object *o) {
        if (o->posX < 0 || o->posX > 150 || o->posY < 0 || o->posY > 31)
                o->is_alive = 0;
}

/* Check if within screen */
int within_screen(Object *o) { // TODO: Is this working correctly?
        if (o->posX > 0 && o->posX < (127 - o->size) && (o->posY + o->velY) > 0 && (o->posY + o->size + o->velY) < 32)
                return 1;
        return 0;
        //if (o->posX < 0 || o->posX > (127 - o->size) || (o->posY - o->velY) <= 1 || o->posY >= 31)
        //        return 0;
        //return 1;
}
