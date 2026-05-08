#include <stdio.h>
#include <string.h>
#include <limits.h>

void itoa(int n, char s[]);
void reverse(char s[]);

/*
Existing itoa() implementation does not handle the large negative number -2147483648 correctly.
Because the absolute value of -2147483648 cannot be represented in 32-bit signed integer.
32-bit integer range from -2^31 to 2^31 - 1,
so the absolute value of -2147483648 is 2147483648 exceeds the maximum positive value of 2147483647.
below is the modified itoa() function that fixes this issue
*/

int main(void)
{
    char s[100];
    int n = INT_MIN;

    itoa(n, s);
    printf("%s\n", s);

    return 0;
}

void itoa(int n, char s[])
{
    int i;
    i = 0;
    unsigned int u = (n < 0) ? -(unsigned int)n : n;

    do
    {
        s[i++] = (u % 10) + '0';
    } while (u /= 10);

    if (n < 0)
        s[i++] = '-';

    s[i] = '\0';

    reverse(s);
}

void reverse(char s[])
{
    int i;
    int j;
    char ch;

    for (i = 0, j = strlen(s) - 1; j >= 0 && i < j; i++, j--)
    {
        ch = s[i];
        s[i] = s[j];
        s[j] = ch;
    }
}
