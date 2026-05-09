#include <stdio.h>
#include <string.h>

#define LENGTH 50

void itob(int n, char s[], int b);
char digit_to_char(int n);
void reverse(char s[]);

int main(void)
{
    int n, b;
    char s[LENGTH];
    n = -1768;
    b = 16;

    itob(n, s, b);
    printf("n: %d, b: %d, s: %s\n", n, b, s);

    return 0;
}

void itob(int n, char s[], int b)
{
    int i;
    unsigned u;

    i = 0;
    u = (n < 0) ? -(unsigned int)n : n;

    if (b >= 2 && b <= 16)
    {
        for (; (u / b) > 0; i++, u /= b)
            s[i] = digit_to_char(u % b);

        s[i++] = digit_to_char(u);

        if (n < 0)
            s[i++] = '-';
    }
    s[i] = '\0';

    reverse(s);
}

char digit_to_char(int n)
{

    return (n >= 0 && n <= 9) ? '0' + n : 'A' + (n - 10);
}

void reverse(char s[])
{
    int i, j;
    char ch;

    for (i = 0, j = strlen(s) - 1; j >= 0 && i < j; i++, j--)
    {
        ch = s[i];
        s[i] = s[j];
        s[j] = ch;
    }
}