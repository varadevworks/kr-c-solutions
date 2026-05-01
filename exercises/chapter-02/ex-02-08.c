#include <stdio.h>
#include <limits.h>

unsigned int rightrot(unsigned int x, int n);

int main(void)
{
    unsigned int x = 145;
    int n = 4;
    unsigned int v;

    v = rightrot(x, n);
    printf("x: %u, n: %d => %u\n", x, n, v);

    return 0;
}

unsigned int rightrot(unsigned int x, int n)
{
    int width = (int)(sizeof(unsigned int) * CHAR_BIT);
    unsigned int p1, p2;

    if (n > 0 && n < width)
    {
        // extract rightmost n bits and push to leftmost
        p1 = (x & (~(~0U << n))) << (width - n);

        // shift to n bits right
        p2 = x >> n;
        return p1 | p2;
    }
    // return same input back in case of invalid input
    return x;
}
