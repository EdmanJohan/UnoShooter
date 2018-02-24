#include <stdlib.h>
#include "registers.h"
#include "io.h"


int randint(int min, int max) {
        srand((unsigned) get_an());
        int r = rand() % max;

        if(r < min)
                r += min;

        return r;
}
