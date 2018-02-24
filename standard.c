#include <stdlib.h>


int randint(int min, int max) {
        int r = rand() % max;

        if(r < min)
                r += min;

        return r;
}
