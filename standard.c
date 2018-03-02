#include "includes/standard.h"
#include <math.h>
#include <stdlib.h>
#include "includes/io.h"
#include "includes/registers.h"

unsigned int seed() {
    int n = 8;
    unsigned int seed = n;
    for (; n > 0; n--) {
        /* Start sampling, wait until conversion is done */
        AD1CON1 |= (0x1 << 1);
        while (!(AD1CON1 & (0x1 << 1))) seed ^= ADC1BUF0 + TMR2;
        while (!(AD1CON1 & 0x1)) seed ^= ADC1BUF0 + TMR2;

        /* Get the analog value */
        seed ^= ADC1BUF0 + TMR2;
    }
    return seed;
}

int randint(int min, int max) {
    if (rand() % (max + 1) > min)
        return rand() % (max + 1);
    return rand() % (max + 1) + min;
}

float randfloat(float min, float max) {
    return (((float)rand() / (float)RAND_MAX)) * (max - min) + min;
}

char* to_char(int number, char* number_rep) {
    char const digit[] = "0123456789";
    char* memalloc = number_rep;
    int pos = 100; // Max number 999

    // Allocating memory 
    do {
        ++memalloc;
        pos = pos / 10;
    } while (pos);

    *memalloc = 0;

    // Placing character representations on their positions
    do {
        *--memalloc = digit[number % 10];
        number /= 10;
    } while (number);

    return number_rep;
}

int dist(float x1, float y1, float x2, float y2) {
    int dx = (int)(x2 - x1) * (x2 - x1);
    int dy = (int)(y2 - y1) * (y2 - y1);

    return (int)sqrt(dx + dy);
}
