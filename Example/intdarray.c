#include <assert.h>
#include "intdarray.h"
#include <string.h>
#include <stdio.h>

idar_t *init(size_t initial_size)
{
    idar_t *t = (idar_t *)malloc(sizeof(idar_t));
    t->li = (type_t *)malloc(initial_size*sizeof(type_t));
    t->alloc = initial_size;
    t->len = 0;

    return t;
}

int append(idar_t *t, type_t val)
{
    if(t->len > t->alloc-1)
    {
        /* check if grow function was successful */
        assert(grow(t) != 1);
    }
    
    t->li[t->len++] = val;
    
    return val;    
}

int grow(idar_t *t)
{
    assert(t != NULL);
    assert(t->li != NULL);
    type_t *newli = (type_t *)malloc(2*t->alloc);

    /* check memory allocation */
    if(newli == NULL)
    {
        perror("Failed to allocate memory...\n");
        return 1;
    }

    memcpy(newli, t->li, sizeof(type_t)*t->len);
    free(t->li);
    t->li = newli;
    t->alloc *= 2;

    /* function successful */
    return 0;
}

void print(idar_t *t)
{
    assert(t != NULL);
    assert(t->li != NULL);
    printf("len is %zd, alloc is %zd\n", t->len, t->alloc);
    for(int i=0; i<t->len; i++)
    {
        printf("%d", t->li[i]);
        int c = (i == t->len-1)?('\n'):(' ');
        printf("%c", c);
    }

    puts("");
}

void destroy(idar_t *t)
{
    free(t->li);
    free(t);
}