#include <stdio.h>
#define MAXLENGTH 100

void itoa(int n, char s[]);

int main(void)
{
    char s[MAXLENGTH];
    int n = -123456;

    itoa(n, s);
    printf("integer: %d, string: %s\n", n, s);

    return 0;
}

static int store_digits(unsigned int n, char s[], int st);

void itoa(int n, char s[])
{
    int i = 0;
    unsigned int u;

    if (n < 0)
    {
        s[i++] = '-';
        u = -(unsigned int)n;
    }
    else
        u = n;

    i = store_digits(u, s, i);
    s[i] = '\0';
}

static int store_digits(unsigned int n, char s[], int st)
{
    int i;

    i = (n / 10) ? store_digits(n / 10, s, st) : st;
    s[i] = n % 10 + '0';

    return i + 1;
}