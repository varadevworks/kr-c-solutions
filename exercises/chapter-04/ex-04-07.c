#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAXOP 100
#define NUMBER '0'
#define NAME '1'
#define VARIABLE '2'

int getop(char s[]);
void push(double f);
double pop(void);
double top(void);
void duplicate(void);
void swap(void);
void clear(void);
void putvar(char ch, double s);
double getvar(char ch);

int main(void)
{
    int type;
    double op2;
    char s[MAXOP];
    double last = 0.0;

    while ((type = getop(s)) != EOF)
    {
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
        case '%':
            op2 = pop();
            if (op2 != 0.0)
                push((int)pop() % (int)op2);
            else
                printf("error: zero divisor\n");
            break;
        case '=':
            putvar(s[0], pop());
            break;
        case '_':
            push(last);
            break;
        case VARIABLE:
            push(getvar(s[0]));
            break;
        case NAME:
            if (strcmp(s, "pow") == 0)
            {
                op2 = pop();
                push(pow(pop(), op2));
            }
            else if (strcmp(s, "sin") == 0)
                push(sin(pop()));
            else if (strcmp(s, "exp") == 0)
                push(exp(pop()));
            else if (strcmp(s, "print") == 0)
                printf("stack top:\t%.8g\n", top());
            else if (strcmp(s, "dup") == 0)
                duplicate();
            else if (strcmp(s, "swap") == 0)
                swap();
            else if (strcmp(s, "clear") == 0)
                clear();
            else
                printf("error: unknown function %s\n", s);
            break;
        case '\n':
            last = pop();
            printf("\t%.8g\n", last);
            break;
        default:
            printf("error: unknown command %s\n", s);
            break;
        }
    }
    return 0;
}

#define MAXVAL 100

int sp = 0;
double val[MAXVAL];

void push(double f)
{
    if (sp < MAXVAL)
        val[sp++] = f;
    else
        printf("error: stack full, can`t push %g\n", f);
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

double top(void)
{
    if (sp > 0)
        return val[sp - 1];
    else
    {
        printf("error: stack empty\n");
        return 0.0;
    }
}

void duplicate(void)
{
    if (sp > 0)
        push(top());
    else
        printf("error: no element to duplicate\n");
}

void swap(void)
{
    double op1, op2;
    if (sp > 1)
    {
        op1 = pop();
        op2 = pop();
    }
    else
    {
        printf("error: not sufficient elements to swap\n");
        return;
    }
    push(op1);
    push(op2);
}

void clear(void)
{
    while (sp > 0)
        pop();
}

#include <ctype.h>

int getch(void);
void ungetch(int);
void ungets(char s[]);

int getop(char s[])
{
    int i, c;

    while ((s[0] = c = getch()) == ' ' || c == '\t')
        ;
    s[1] = '\0';

    i = 0;

    if (!isdigit(c) && c != '.' && c != '-')
    {
        while (isalpha(c))
            s[++i] = c = getch();

        if (i == 1) /* single character variable */
        {
            while (c == ' ' || c == '\t')
                c = getch();

            if (c == '=') /* assignment operator for variable */
                return c;

            ungetch(c);
            return VARIABLE;
        }
        else if (i > 1 && c != EOF) /* function name */
        {
            ungetch(c);
            s[i] = '\0';
            return NAME;
        }

        return c; /* not a number */
    }

    if (c == '-') /* handle negative numbers */
    {
        s[++i] = c = getch();
        if (!isdigit(c) && c != '.')
        {
            ungetch(c);
            return s[i - 1];
        }
    }

    if (isdigit(c)) /* collect integer part */
        while (isdigit(s[++i] = c = getch()))
            ;

    if (c == '.') /* collect fractional part */
        while (isdigit(s[++i] = c = getch()))
            ;
    s[i] = '\0';

    if (c != EOF)
        ungetch(c);
    return NUMBER;
}

#define BUFSIZE 100

char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0;      /* next free position in buffer */

int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}

void ungets(char s[])
{
    int len, i;
    len = strlen(s);

    if ((bufp + len) <= BUFSIZE)
        for (i = len - 1; i >= 0; i--)
            ungetch(s[i]);
    else
        printf("ungets: too many characters\n");
}

double vars[52]; /* variables A-Z and a-z */

void putvar(char c, double d)
{
    if (isalpha(c))
        vars[c - (isupper(c) ? 'A' : ('a' - 26))] = d;
    else
        printf("error: invalid variable name %c\n", c);
}

double getvar(char c)
{
    if (isalpha(c))
        return vars[c - (isupper(c) ? 'A' : ('a' - 26))];

    printf("error: invalid variable name %c\n", c);
    return 0.0;
}