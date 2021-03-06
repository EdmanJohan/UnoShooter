#include "includes/standard.h"
#include <math.h>
#include <stdlib.h>
#include "includes/io.h"
#include "includes/registers.h"

/* Sets a seed for the rand() function by using analogue inputs on the device */
/*  @author Johan Edman - Tweaked by Alex Diaz */
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

/* Returns a random integer between parameter min and max, inclusive. */
/*  @author Johan Edman */
int randint(int min, int max) {
        int r = rand() % (max + 1);

        if (r > min)
                return r;
        return r + min;
}

/* Returns a random float between parameter min and max. */
/*  @author Johan Edman */
float randfloat(float min, float max) {
        return (((float)rand() / (float)RAND_MAX)) * (max - min) + min;
}

/* Converts an integer and stores it in a character array */
/*  @author Alex Diaz */
void to_char(int number, char* number_rep) {
        char const digit[] = "0123456789";
        int pos = 100; // Max number 999

        // Allocating memory
        do {
                ++number_rep;
                pos = pos / 10;
        } while (pos);

        *number_rep = 0;

        // Placing character representations on their positions
        do {
                *--number_rep = digit[number % 10];
                number /= 10;
        } while (number);
}

/* Calculates the distance between to xy-positions. */
/*  @author Johan Edman */
int dist(float x1, float y1, float x2, float y2) {
        int dx = (int)(x2 - x1) * (x2 - x1);
        int dy = (int)(y2 - y1) * (y2 - y1);

        return (int)sqrt(dx + dy);
}
