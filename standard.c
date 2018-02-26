#include <stdlib.h>
#include "includes/io.h"
#include "includes/registers.h"

unsigned int seed() {
        int n = 8;
        unsigned int seed = n;
        for (; n > 0; n--) {
                /* Start sampling, wait until conversion is done */
                AD1CON1 |= (0x1 << 1);
                while (!(AD1CON1 & (0x1 << 1)))
                        seed ^= ADC1BUF0 + TMR2;
                while (!(AD1CON1 & 0x1))
                        seed ^= ADC1BUF0 + TMR2;

                /* Get the analog value */
                seed ^= ADC1BUF0 + TMR2;
        }
        return seed;
}

int randint(int min, int max) {
        srand(seed());
        int r = rand() % (max + 1);

        if (r < min) r += min;
        return r;
}
