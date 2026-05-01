#include <stdio.h>
#include <limits.h>

unsigned int invert(unsigned int x, int p, int n);

int main(void)
{
    unsigned int x = 145;
    int p = 7;
    int n = 4;

    unsigned int v;

    v = invert(x, p, n);
    printf("x: %u, p: %d, n: %d => %u\n", x, p, n, v);

    return 0;
}

unsigned int invert(unsigned int x, int p, int n)
{
    int width = (int)(sizeof(unsigned int) * CHAR_BIT);

    if (p >= 0 && p <= (width - 1) && n > 0 && (p + 1) >= n && n < width)
        return x ^ ((~(~0U << n)) << (p + 1 - n));
    // in case of invalid input return same input
    return x;
}