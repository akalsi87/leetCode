#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>

struct timeval start;
struct timeval end;

static
void markstart()
{
    gettimeofday(&start, NULL);
}

static
void markend()
{
    gettimeofday(&end, NULL);
    long secs = (end.tv_sec - start.tv_sec); //avoid overflow by subtracting first
    long micros = ((secs*1000000) + end.tv_usec) - (start.tv_usec);
    printf("microseconds taken: %d\n", (int)micros);
}



