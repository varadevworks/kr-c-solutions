#include <stdio.h>

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
    return x ^ ((~(~0U << n)) << (p + 1 - n));
}