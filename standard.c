#include <stdlib.h>
#include "io.h"
#include "registers.h"

int randint(int min, int max) {
    unsigned int seed = max + min;
    seed ^= ADC1BUF0;
    srand((unsigned) seed);
    int r = rand() % max;

    if (r < min) r += min;

    return r;
}
