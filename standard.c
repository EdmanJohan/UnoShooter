#include <stdlib.h>
#include "registers.h"


int randint(int min, int max) {
        srand((unsigned) TMR2);
        int r = rand() % max;

        if(r < min)
                r += min;

        return r;
}
