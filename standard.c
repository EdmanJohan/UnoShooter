#include <stdlib.h>
#include "io.h"
#include "registers.h"

int randint(int min, int max) {
    unsigned int seed = max + min;
    seed ^= ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3 + ADC1BUF4 + ADC1BUF5 +
            ADC1BUF6 + ADC1BUF7 + ADC1BUF8 + ADC1BUF9 + ADC1BUFA + ADC1BUFB +
            ADC1BUFC + ADC1BUFD + ADC1BUFE + ADC1BUFF;
    srand((unsigned) seed);
    int r = rand() % max;

    if (r < min) r += min;

    return r;
}
