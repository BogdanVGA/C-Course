#ifndef DYN_ARRAY
#define DYN_ARRAY

#include <stdlib.h>

typedef int type_arr;

typedef struct
{
    type_arr *arr;
    type_arr length;
} dyn_array;

dyn_array *append(dyn_array *array, int appValue);

void printArray(dyn_array *array);

void cleanup(dyn_array *array);

#endif