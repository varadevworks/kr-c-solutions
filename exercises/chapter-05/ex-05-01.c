#include <ctype.h>
#include <stdio.h>

int getch(void);
void ungetch(int);
int getint(int *);

int main(void)
{
    int n;
    int *pn = &n;
    int r = getint(pn);
    if (r == 0)
        printf("input is not a number");
    else
        printf("integer read: %d", n);

    return 0;
}

int getint(int *pn)
{
    int c, sign, temp;

    while (isspace(c = getch())) /* skip whitespace */
        ;

    if (!isdigit(c) && c != EOF && c != '+' && c != '-')
    {
        ungetch(c); /* not a number*/
        return 0;
    }

    sign = (c == '-') ? -1 : 1;

    temp = c;
    if ((c == '+' || c == '-') && (!isdigit(c = getch())))
    {
        if (c != EOF)
            ungetch(c); /* not a number */
        ungetch(temp);  /* push back the sign */

        return 0;
    }

    for (*pn = 0; isdigit(c); c = getch())
        *pn = 10 * *pn + (c - '0');

    *pn *= sign;

    if (c != EOF)
        ungetch(c);

    return c;
}

#define MAXBUFSIZE 100

char buffer[MAXBUFSIZE];
int bufferp = 0;

int getch(void)
{
    return (bufferp > 0) ? buffer[--bufferp] : getchar();
}

void ungetch(int c)
{
    if (bufferp >= MAXBUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buffer[bufferp++] = c;
}