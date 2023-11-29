
#include "intdarray.h"

int main()
{
    idar_t *T = init(2);

    print(T);

    append(T, 0);
    append(T, 1);
    append(T, 2);
    append(T, 3);
    append(T, 4);
    append(T, 5);
    append(T, 6);
    append(T, 12);
    append(T, 34);

    print(T);

    destroy(T);

    return 0;
}