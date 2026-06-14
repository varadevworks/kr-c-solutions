#include <stdio.h>
#include <ctype.h>
#include <stdlib.h> /* for atof() */

#define MAXLINE 1000 /* maximum input line length */
#define MAXOP 100    /* max size of operand or operator */
#define NUMBER '0'   /* signal that number was found*/

int get_line(char *s, int limit); /* renamed to get_line to avoid conflict with library function */
int strindex(char *s, char *t);
int atoi(const char *s);
void reverse(char *s);
void itoa(int n, char *s);

/* getop & its supporting functions declaration for reverse polish calculator */
int getop(char *s);
void push(double f);
double pop(void);

int main(void)
{
    /* test: get_line function */
    /* comment get_line to test getop function as both expects input from stdin */
    char line[MAXLINE];

    while (get_line(line, MAXLINE) > 0)
        printf("%s", line);

    /* test: strindex function */
    int r = -1;
    char s[] = "Hello World!";
    char t[] = "World";
    r = strindex(s, t);
    if (r >= 0)
        printf("Found '%s' in '%s' at index %d\n", t, s, r);
    else
        printf("'%s' not found in '%s'\n", t, s);

    /* test: atoi function */
    char a[] = "-1234";
    int i = atoi(a);
    printf("input: %s converted to integer: %i\n", a, i);

    /* test: reverse function */
    char rv[] = "Hello World!";
    printf("input: %s, ", rv);
    reverse(rv);
    printf("reverse: %s\n", rv);

    /* test: itoa function */
    char ib[100];
    int n = -1234;
    itoa(n, ib);
    printf("input: %d converted to string: %s\n", n, ib);

    /* test: getop function (reverse polish calculator) */
    int type;
    double op2;
    char g[MAXOP];
    while ((type = getop(g)) != EOF)
    {
        switch (type)
        {
        case NUMBER:
            push(atof(g));
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
        case '\n':
            printf("\t%.8g\n", pop());
            break;
        default:
            printf("error: unknown command %s\n", g);
            break;
        }
    }

    return 0;
}

/* get_line: get line into s, return length */
int get_line(char *s, int limit)
{
    int c;
    char *o = s;

    while (--limit > 0 && (c = getchar()) != EOF && c != '\n')
        *s++ = c;

    if (c == '\n')
        *s++ = c;

    *s = '\0';

    return (int)(s - o);
}

/* atoi: convert s to integer */
int atoi(const char *s)
{
    int n, sign;

    for (; isspace(*s); s++) /* skip white space */
        ;
    sign = (*s == '-') ? -1 : 1;
    if (*s == '+' || *s == '-') /* skip sign */
        s++;

    for (n = 0; isdigit(*s); s++)
        n = 10 * n + (*s - '0');

    return n * sign;
}

/* itoa: convert n into characters in s*/
void itoa(int n, char *s)
{
    unsigned int u;
    char *o = s;

    u = n < 0 ? -(unsigned)n : n;

    do
    {
        *s++ = u % 10 + '0';
    } while (u /= 10);

    if (n < 0)
        *s++ = '-';
    *s = '\0';
    reverse(o);
}

/* reverse: reverse string s in place */
void reverse(char *s)
{
    char *o = s;
    char temp;
    while (*o) /* find string length */
        o++;
    o--;
    for (; s < o; s++, o--)
    {
        temp = *s;
        *s = *o;
        *o = temp;
    }
}

/* strindex: return index of t in s, -1 if none */
int strindex(char *s, char *t)
{
    char *i, *j, *k;

    for (i = s; *i; i++)
    {
        for (j = i, k = t; *k != '\0' && *j == *k; j++, k++)
            ;
        if ((int)(k - t) > 0 && *k == '\0')
            return (int)(i - s);
    }
    return -1;
}

/* reverse polish calculator implementation */
int getch(void);
void ungetch(int);

/* getop: get next character or numeric operand */
int getop(char *s)
{
    int c;
    while ((*s = c = getch()) == ' ' || c == '\t')
        ;
    *(++s) = '\0';

    if (!isdigit(c) && c != '.')
        return c; /* not a number */

    if (isdigit(c)) /* collect integer part */
        while (isdigit(*s++ = c = getch()))
            ;

    if (c == '.') /* collect fraction part */
        while (isdigit(*s++ = c = getch()))
            ;

    if (!isdigit(c)) /* remove last non-digit character */
        --s;

    *s = '\0';
    if (c != EOF)
        ungetch(c);
    return NUMBER;
}

#define MAXVAL 100  /* maximum depth of val stack */
int sp = 0;         /* next free stack position */
double val[MAXVAL]; /* value stack */

/* push: push f onto value stack */
void push(double f)
{
    if (sp < MAXVAL)
        val[sp++] = f;
    else
        printf("error: stack full, can't push %g\n", f);
}

/* pop: pop and return top value from stack */
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

#define BUFSIZE 100

char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0;      /* next free position in buf */

/* get a (possibly pushed-back) character */
int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

/* push character back on input */
void ungetch(int c)
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}