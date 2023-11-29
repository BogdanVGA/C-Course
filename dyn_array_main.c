#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"

int main()
{
    dyn_array *array = NULL;            
    
    array = (dyn_array *)calloc(1, sizeof(dyn_array));    
    
    if(array == NULL)
    {
        perror("Error allocating memory...\n");
        exit (1);
    }

    /* init parameters for the array */
    array->length = 2;

    array->arr = (type_arr *)calloc(array->length, sizeof(type_arr));

    if(array->arr == NULL)
    {
        perror("Error allocating memory...\n");
        exit (1);
    }

    /* populate the array with whatever values */
    for(int i = 0; i<array->length; i++)
    {
        array->arr[i] = i + 3;
    }

    /* call print array function */    
    printArray(array);

    /* call append function */
    array = append(array, 23);        

    array = append(array, 56);    

    array = append(array, 67);    

    array = append(array, -33);

    array = append(array, 135);    
    
    printArray(array);   

    /* call cleanup function to free memory */
    cleanup(array);

    return 0;
}
