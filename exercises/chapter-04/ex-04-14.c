#include <stdio.h>
#define swap(t, x, y) \
    {                 \
        t temp##x;    \
        temp##x = x;  \
        x = y;        \
        y = temp##x;  \
    }

int main(void)
{
    int a = 5;
    int b = 6;

    printf("before\tswap\t=> a: %d, b: %d\n", a, b);
    swap(int, a, b);
    printf("after\tswap\t=> a: %d, b: %d\n", a, b);

    return 0;
}