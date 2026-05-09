#include <stdio.h>
#include <string.h>
#include <limits.h>

void itoa(int n, char s[], int w);
void reverse(char s[]);
int padblanks(char s[], int p, int w);

int main(void)
{
    char s[100];
    int n, w;

    n = INT_MIN;
    w = 20;

    itoa(n, s, w);
    printf("n: %d, w: %d, s: %s\n", n, w, s);

    return 0;
}

void itoa(int n, char s[], int w)
{
    int i;
    i = 0;
    unsigned int u;

    u = (n < 0) ? -(unsigned int)n : n;

    do
    {
        s[i++] = (u % 10) + '0';
    } while (u /= 10);

    if (n < 0)
        s[i++] = '-';

    i = padblanks(s, i, w);
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

int padblanks(char s[], int p, int w)
{
    int i;

    i = p;
    for (; i < w; i++)
        s[i] = ' ';
    return i;
}