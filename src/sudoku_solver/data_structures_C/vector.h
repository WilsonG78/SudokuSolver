#include <stdio.h>
#include <stdlib.h>


typedef struct Vector
{
    int size;
    int capacity;
    int *data;
} Vector;