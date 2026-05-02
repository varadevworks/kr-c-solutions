#include <stdio.h>

int bitcount(unsigned int x);

int main(void)
{
    unsigned int x = 199;
    int b;
    b = bitcount(x);
    printf("x: %u => bitcount : %d\n", x, b);
}

int bitcount(unsigned int x)
{
    int b = 0;

    if (x > 0)
        b = 1;

    while ((x &= (x - 1)) > 0)
        b++;

    return b;
}

/*
* Question: why x &= (x - 1) deletes rightmost 1 bit?
* Explanation:
*   In two’s complement, x - 1 is x + (-1).
    When adding -1 (all 1s), the rightmost 1 bit in x becomes 0; bits to its right become 1s,
    and carry generated there propagates to higher bits which remain bits before rightmost 1 bit unchanged.
    Then x & (x - 1) clears that rightmost 1 bit.
*   Example:
    x  = 199 (binary: 11000111)
    -1 = 255 (binary: 11111111)
    x + (-1) = 198 (binary: 11000110)
    x & (x - 1) = 11000111 & 11000110
                = 11000110 (198)
    The rightmost 1 bit in x is cleared in the result.
*/