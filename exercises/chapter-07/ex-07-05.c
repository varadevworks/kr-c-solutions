#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINELEN 101
#define CHECKOPCNT(sp, cnt)                     \
    if (sp < cnt)                               \
    {                                           \
        printf("error: not enough operands\n"); \
        break;                                  \
    }

void push(double);
double pop(void);
int get_line(char[], int);

int main(void)
{
    char line[MAXLINELEN]; /* buffer to hold the current line */
    char *p;               /* pointer to the current position in the line */
    int rs = 0, n = 0;     /* rs: result of sscanf, n: number of characters read */
    char c;                /* character read */
    double op1, op2;       /* variables to hold the number read */

    while (get_line(line, MAXLINELEN) > 0)
    {
        p = line; /* reset pointer to the start of the line */
        rs = 0;   /* reset sscanf result for the new line */
        n = 0;    /* reset number of characters read for the new line */
        while (*p != '\0')
        {
            /* 1. try read double */
            rs = sscanf(p, "%lf%n", &op2, &n);
            if (rs > 0) /* successfully read a number */
            {
                push(op2);
                p = p + n;
                continue;
            }

            /* 2. try read double failed, read character */
            rs = sscanf(p, "%c%n", &c, &n);
            if (rs > 0)
            {
                p = p + n;
                if (c == ' ' || c == '\t') /* skip whitespace */
                    continue;

                switch (c) /* perform operation */
                {
                case '+':
                    CHECKOPCNT(sp, 2);
                    op2 = pop();
                    op1 = pop();
                    push(op1 + op2);
                    break;
                case '*':
                    CHECKOPCNT(sp, 2);
                    op2 = pop();
                    op1 = pop();
                    push(op1 * op2);
                    break;
                case '-':
                    CHECKOPCNT(sp, 2);
                    op2 = pop();
                    op1 = pop();
                    push(op1 - op2);
                    break;
                case '/':
                    CHECKOPCNT(sp, 2);
                    op2 = pop();
                    op1 = pop();
                    if (op2 != 0.0)
                        push(op1 / op2);
                    else
                        printf("error: zero divisor\n");
                    break;
                case '\n':
                    CHECKOPCNT(sp, 1);
                    printf("%.8g\n", pop());
                    break;
                default:
                    printf("error: unknown command %c\n", c);
                    break;
                }
            }
        }
    }

    return 0;
}

int get_line(char s[], int lim)
{
    int c, i = 0;

    while (i < lim - 1 && (c = getchar()) != EOF)
    {
        s[i++] = c;
        if (c == '\n')
            break;
    }

    s[i] = '\0';
    return i;
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