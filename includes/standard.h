#ifndef _STANDARD_H_
#define _STANDARD_H_

void *stdin, *stdout, *stderr, *errno;

int randint(int min, int max);

float randfloat(float min, float max);

unsigned int seed();

void to_char(int number, char* number_rep);

int dist(float x1, float y1, float x2, float y2);

#endif // _STANDARD_H_
