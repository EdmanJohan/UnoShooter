#include "includes/objects.h"
#include "includes/display_ui.h"
#include "includes/standard.h"

/* === CONSTRUCTORS === */

/* Player Constructor */
void player_new(Object *p) {
        p->posX = 10;
        p->posY = 10;
        p->vX = 1;
        p->vY = 1;
        p->size = 10;
        p->isAlive = 1;
}

/* Rock Constructor */
void rock_new(Object *r) {
        r->posX = randint(115, 120);
        r->posY = randint(5, 25);
        r->vX = -randint(1, 2);
        r->vY = 0;
        r->size = randint(3, 7);
        r->isAlive = 1;
}

/* Shot Constructor */
void new_shot(Object *s, Object *p) {
      s->posX = p->posX;
      s->posY = p->posY;
      s->vX = 0;
      s->vY = -1;
      s->size = 1;
      s->isAlive = 1;
}

/* === MOVE === */

/* Object Move */
void object_move(Object *o) {
      o->posX += o->vX;
      o->posY += o->vY;
}

/* === UPDATE === */
void object_update(Object *o){
  erase(o);

  within_border(o);

  if (o->isAlive) {
      object_move(o);
      draw(*o);
  }
}




/*void rock_update(Object *r) {
        int i, j;
        for (i = 0; i <= r->size / 2; i++)
                for (j = 0; j <= r->size / 2; j++)
                        unset_pixel(i + r->posX, j + r->posY);

        within_border(r);
        if (r->isAlive) object_move(r);
}


void rock_show(Object *r) {
        if (r->isAlive) {
                int i, j;
                for (i = 0; i <= r->size / 2; i++)
                        for (j = 0; j <= r->size / 2; j++)
                                set_pixel(i + r->posX, j + r->posY);

                draw(*r);
        }
}
*/

int get_posX(Object *o) {
        return o->posX + o->size / 2;
}

int get_posY(Object *o) {
        return o->posY + o->size / 2;
}

void within_border(Object *o) {
        if (o->posX < o->size)
               o->isAlive = 0;
}
