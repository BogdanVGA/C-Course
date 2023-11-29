
#ifndef DARRAY
#define DARRAY

#include <stdlib.h>

typedef int type_t;

typedef struct
{
    type_t *li;
    size_t len;
    size_t alloc;
} idar_t;

idar_t *init(size_t initial_size);

int append(idar_t *t, type_t val);

int grow(idar_t *t);

void print(idar_t *t);

void destroy(idar_t *t);

#endif