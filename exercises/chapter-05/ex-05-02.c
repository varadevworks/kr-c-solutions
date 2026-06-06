#include <ctype.h>
#include <stdio.h>

int getch(void);
void ungetch(int);
int getfloat(double *);

int main(void)
{
    double n;
    double *pn = &n;
    int r = getfloat(pn);
    if (r == 0)
        printf("input is not a number");
    else
        printf("float read: %g", n);

    return 0;
}

int getfloat(double *pn)
{
    int c, temp, exp = 0;
    double sign = 1.0, power = 1.0, esign = 1.0;

    while (isspace(c = getch())) /* skip whitespace */
        ;

    if (!isdigit(c) && c != EOF && c != '+' && c != '-' && c != '.')
    {
        ungetch(c); /* not a number*/
        return 0;
    }

    sign = (c == '-') ? -1.0 : 1.0;

    temp = c;
    if (c == '+' || c == '-')
        c = getch();
    
    if (c != '.' && !isdigit(c))
    {
        if (c != EOF)
            ungetch(c); /* not a number */
        ungetch(temp);  /* push back the sign */
        return 0;
    }

    for (*pn = 0; isdigit(c); c = getch()) /* handle integer part */
        *pn = 10 * *pn + (c - '0');

    if (c == '.')
        c = getch();

    for (; isdigit(c); c = getch()) /* handle fractional part */
    {
        *pn = 10 * *pn + (c - '0');
        power = power * 10.0;
    }

    if (c == 'e' || c == 'E') /* determine exponent sign */
    {
        c = getch();
        if (c == '-')
        {
            esign = -1.0;
            c = getch();
        }
        else if (c == '+')
            c = getch();
    }

    for (; isdigit(c); c = getch()) /* handle exponent part*/
        exp = 10 * exp + (c - '0');

    for (; exp > 0; exp--) /* calculate power */
        power = (esign > 0) ? power / 10.0 : power * 10.0;

    *pn *= sign / power;

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