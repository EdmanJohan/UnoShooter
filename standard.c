#include <stdlib.h>
#include "io.h"
#include "registers.h"

int randint(int min, int max) {
        int r = rand() % (max + 1);

        if (r < min) r += min;
        return r;
}

unsigned int seed() {
        int n = 8;
        unsigned int seed = n;
        for (; n > 0; n--) {
                /* Start sampling, wait until conversion is done */
                AD1CON1 |= (0x1 << 1);
                while (!(AD1CON1 & (0x1 << 1)));
                while (!(AD1CON1 & 0x1));

                /* Get the analog value */
                seed ^= ADC1BUF0;

        }
        return seed;

}
