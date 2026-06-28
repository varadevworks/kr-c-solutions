/* Reverse Polish calculator*/
#include <stdio.h>
#include <stdlib.h> /* for atof() */
#include <ctype.h>  /* for isspace() and isdigit() */

#define MAXOP 100  /* max size of operand or operator */
#define NUMBER '0' /* signal that a number was found */

int getop(char[]);
void push(double);
double pop(void);
int isnum(char *s);

int main(int argc, char *argv[])
{
    char *arg;
    double op2;

    while (--argc > 0)
    {
        arg = *++argv;
        if (isnum(arg))
            push(atof(arg));
        else
        {
            switch (*arg) // first character of the argument
            {
            case '+':
                push(pop() + pop());
                break;
            case '-':
                op2 = pop();
                push(pop() - op2);
                break;
            case '*':
                push(pop() * pop());
                break;
            case '/':
                op2 = pop();
                if (op2 != 0.0)
                    push(pop() / op2);
                else
                    printf("error: zero divisor\n");
                break;
            default:
                printf("error: unknown command %s\n", arg);
                break;
            }
        }
    }

    printf("\tresult: %.8f\n", pop());
    return 0;
}

#define MAXVAL 100 /* maximum depth of val stack */

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

int isnum(char *s)
{
    int isnum = 0;
    int i = 0, f = 0, e = 0;

    /* ignore spaces */
    while (isspace(*s) || *s == '+' || *s == '-')
        s++;

    /* integer part */
    while (*s != '\0' && isdigit(*s++))
        i = 1;

    /* fractional part */
    if (*s != '\0' && *s++ == '.')
        while (*s != '\0' && isdigit(*s++))
            f = 1;

    /* scientific notation */
    if (*s != '\0' && (*s == 'e' || *s == 'E'))
    {
        e = -1; // mark start of exponent
        s++;
        // exponent sign
        if (*s != '\0' && (*s == '-' || *s == '+'))
            s++;
        // exponent part
        while (*s != '\0' && isdigit(*s++))
            e = 1;
    }

    if (*s == '\0' && (i || f) && (e == 0 || (e == 1 && (i || f))))
        isnum = 1;

    return isnum;
}