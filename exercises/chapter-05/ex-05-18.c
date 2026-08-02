#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100
#define MAXOUT 1000

enum
{
    NAME,
    PARENS,
    BRACKETS
};

void dcl(void);
void dirdcl(void);

int gettoken(void);
int tokentype;           /*type of last token*/
char token[MAXTOKEN];    /* last token string */
char name[MAXTOKEN];     /* identifier name */
char datatype[MAXTOKEN]; /* data type = char, int, etc. */
char out[MAXOUT];        /* output string */
int err = 0;             /* error flag */

int main(void) /* convert declaration to words */
{
    while (gettoken() != EOF)
    {
        strcpy(datatype, token); /* 1st token on line is the datatype */
        out[0] = '\0';
        dcl(); /* parse rest of line */
        if (tokentype != '\n' && tokentype != EOF && err == 0)
        {
            printf("syntax error.\n");
            err = 1;
        }

        if (err)
        {
            while (tokentype != '\n' && tokentype != EOF) /* read the rest of the line to skip */
                gettoken();
            err = 0;
            continue;
        }
        printf("%s: %s %s\n", name, out, datatype);
    }

    return 0;
}

/* dcl: parse a declarator */
void dcl(void)
{
    int ns;

    for (ns = 0; gettoken() == '*';) /* count *'s */
        ns++;
    dirdcl();

    if (err)
        return;

    while (ns-- > 0)
        strcat(out, " pointer to");
}

/* dirdcl: parse a direct declarator */
void dirdcl(void)
{
    int type;

    if (tokentype == '(') /* (dcl) */
    {
        dcl();
        if (tokentype != ')' && err == 0)
        {
            printf("error: missing )\n");
            err = 1;
        }
    }
    else if (tokentype == NAME) /* variable name*/
        strcpy(name, token);
    else
    {
        printf("error: expected name or (dcl)\n");
        err = 1;
    }

    if (err)
        return;

    while ((type = gettoken()) == PARENS || type == BRACKETS)
        if (type == PARENS)
            strcat(out, " function returning");
        else
        {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }
}

int gettoken(void) /* return next token */
{
    int c, getch(void);
    void ungetch(int);
    char *p = token;

    while ((c = getch()) == ' ' || c == '\t')
        ;
    if (c == '(')
    {
        if ((c = getch()) == ')')
        {
            strcpy(token, "()");
            return tokentype = PARENS;
        }
        else
        {
            ungetch(c);
            return tokentype = '(';
        }
    }
    else if (c == '[')
    {
        for (*p++ = c; (*p++ = getch()) != ']' && *(p - 1) != '\n' && *(p - 1) != EOF;)
            ;
        if (*(p - 1) == '\n' || *(p - 1) == EOF)
        {
            printf("error: missing ]\n");
            err = 1;
            ungetch(*(p - 1));
            return tokentype = '[';
        }
        *p = '\0';
        return tokentype = BRACKETS;
    }
    else if (isalpha(c))
    {
        for (*p++ = c; isalnum(c = getch());)
            *p++ = c;
        *p = '\0';
        ungetch(c);
        return tokentype = NAME;
    }
    else
        return tokentype = c;
}

#define BUFSIZE 100

char buf[BUFSIZE]; /* buffer for ungetch*/
int bufp = 0;      /* next free position in buffer */

int getch(void) /* get a (possibly pushed back) character*/
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /* push character back on input */
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}