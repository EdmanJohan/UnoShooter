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
    srand(seed());
    int r = rand() % (max + 1);

    if (r < min) r += min;
    return r;
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

    *memalloc = '\0';

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

float randfloat(float min, float max) {
        return (((float) rand() / (float) RAND_MAX)) * (max - min) + min;
}

// modified from bhuwansahni's original code from
// https://stackoverflow.com/questions/9655202/how-to-convert-integer-to-string-in-c
char* itoa(int i, char b[]) {
        char const digit[] = "0123456789";
        char* p = b;
        int shifter = 100;

        // Move to where representation ends
        do {
                ++p;
                shifter = shifter / 10;
        } while (shifter);

        // Move back, inserting digits as u go
        *p = '\0';
        do {
                *--p = digit[i % 10];
                i = i / 10;
        } while (i);

        return b;
}
