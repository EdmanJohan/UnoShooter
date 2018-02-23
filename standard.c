#include <stdlib.h>


int randint(int min, int max) {
        if ((max - 1) == RAND_MAX)
                return rand();
        else {
                long end = RAND_MAX / max;
                end *= max;


                int r;
                while((r = rand() >= end));

                return min + (r % max);
        }
}
