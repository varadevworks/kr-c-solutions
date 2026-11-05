#include <stdio.h>

static int isupperss(int c);
static inline int isupperst(int c);

int main(void)
{
    char c = 'A';

    /* isupperst: isupper save time implementation which uses a inline function */
    if (isupperst(c))
    {
        printf("%c is uppercase (inline)\n", c);
    }

    /* isupperss: isupper save space implementation which uses a function call */
    if (isupperss(c))
    {
        printf("%c is uppercase (function)\n", c);
    }

    return 0;
}

static int isupperss(int c)
{
    return c >= 'A' && c <= 'Z';
}

static inline int isupperst(int c)
{
    return c >= 'A' && c <= 'Z';
}