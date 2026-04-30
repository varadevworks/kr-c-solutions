#include <stdio.h>
#include <limits.h>

unsigned int setbits(unsigned int x, int p, int n, unsigned int y);

int main(void)
{
    unsigned int x = 140;
    unsigned int y = 155;
    int p = 7;
    int n = 4;
    unsigned int v;

    v = setbits(x, p, n, y);
    printf("x: %u, p: %d, n: %d, y: %u => %u", x, p, n, y, v);

    return 0;
}

unsigned int setbits(unsigned int x, int p, int n, unsigned int y)
{
    unsigned int p1, p2;

    int width = (int)(sizeof(unsigned int) * CHAR_BIT);

    if (n == width)
        return y;

    p1 = (y & (~(~0U << n))) << (p + 1 - n);
    p2 = x & ~((~(~0U << n)) << (p + 1 - n));

    return p1 | p2;
}
