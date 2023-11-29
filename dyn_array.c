#include "dyn_array.h"

#include <stdio.h>
#include <stdlib.h>

dyn_array *append(dyn_array *array, int appValue)
{
    array->length++;        

    array->arr = (type_arr *)realloc(array->arr, array->length * sizeof(type_arr));

    if(array->arr == NULL)
    {
        perror("Error reallocating memory...");
        exit (1);
    }    

    array->arr[array->length-1] = appValue;
    
    return array;
}

void printArray(dyn_array *array)
{
    printf("The array is:\n\n");

    for(int i = 0; i<array->length; i++)
    {
        printf("a[%d]: %d ", i, array->arr[i]);
    }

    printf("\n\n");
}

void cleanup(dyn_array *array)
{
    free(array->arr);
    free(array);
    array->arr = NULL;
    array = NULL;
}