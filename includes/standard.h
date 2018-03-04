#ifndef _STANDARD_H_
#define _STANDARD_H_

void *stdin, *stdout, *stderr, *errno;

/* Sets a seed for the rand() function by using analogue inputs on the device */
unsigned int seed();

/* Returns a random integer between parameter min and max, inclusive. */
int randint(int min, int max);

/* Returns a random integer between parameter min and max, inclusive. */
float randfloat(float min, float max);

/* Converts an integer and stores it in a character array */
void to_char(int number, char* number_rep);

/* Calculates the distance between to xy-positions. */
int dist(float x1, float y1, float x2, float y2);

#endif // _STANDARD_H_
