#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXOP 100
#define NUMBER '0' /* signal that a number was found */

int getop(char[]);
void push(double);
double pop(void);

int main(void)
{
    int type;
    char s[MAXOP];
    double op2;
    char fmt[20];

    snprintf(fmt, sizeof(fmt), "%%%ds", MAXOP - 1);

    while (scanf(fmt, s) == 1)
    {
        type = getop(s);
        switch (type)
        {
        case NUMBER:
            push(atof(s));
            break;
        case '+':
            push(pop() + pop());
            break;
        case '*':
            push(pop() * pop());
            break;
        case '-':
            op2 = pop();
            push(pop() - op2);
            break;
        case '/':
            op2 = pop();
            if (op2 != 0.0)
                push(pop() / op2);
            else
                printf("error: zero divisor\n");
            break;
        case 'p':
            printf("\t%.8g\n", pop());
            break;
        default:
            printf("error: unknown command %s\n", s);
            break;
        }
    }

    return 0;
}

int getop(char s[])
{
    int i = 0;
    int c;

    while (*s == ' ' || *s == '\t') /* skip whitespaces */
        s++;

    if (!isdigit(*s) && *s != '.') /* not a number */
        return *s;

    if (isdigit(*s)) /* get integer part */
        while (isdigit(*s))
            s++;

    if (*s == '.') /* get fractional part */
    {
        s++;
        while (isdigit(*s))
            s++;
    }
    *s = '\0';
    return NUMBER;
}

#define MAXVAL 100 /* maximum depth of val stack */
int sp = 0;
double val[MAXVAL]; /* value stack */

void push(double f)
{
    if (sp < MAXVAL)
        val[sp++] = f;
    else
        printf("error: stack full, can't push %g\n", f);
}

double pop(void)
{
    if (sp > 0)
        return val[--sp];
    else
    {
        printf("error: stack empty\n");
        return 0.0;
    }
}