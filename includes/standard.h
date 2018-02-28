#ifndef _STANDARD_H_
#define _STANDARD_H_

void *stdin, *stdout, *stderr;

int randint(int min, int max);

float randfloat(float min, float max);

unsigned int seed();

char* itoa(int i, char b[]);

#endif // _STANDARD_H_
