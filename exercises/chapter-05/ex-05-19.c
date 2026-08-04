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

void undcl(void);
int gettoken(void);
int tokentype;        /*type of last token*/
int prevtokentype;    /*type of previous token*/
char token[MAXTOKEN]; /* last token string */
char out[MAXOUT];     /* output string */
int err = 0;          /* error flag */
int starcount = 0;    /* count of '*' in declaration */

int main(void) /* convert declaration to words */
{
    while (gettoken() != EOF)
    {
        out[0] = '\0';             /* clear output string */
        starcount = 0;             /* reset star count for each new declaration */
        strcpy(out, token);        /* get first token */
        prevtokentype = tokentype; /* store previous token type for error checking */

        undcl(); /* parse rest of line */

        if (prevtokentype != NAME)
        {
            printf("syntax error: missing identifier\n");
            err = 1;
        }

        if (err)
        {
            while (tokentype != '\n' && tokentype != EOF) /* read the rest of the line to skip */
                gettoken();
            err = 0; /* reset error flag for next declaration */
        }
        else
            printf("%s\n", out);
    }

    return 0;
}

/* undcl: convert word description to declaration */
void undcl(void)
{
    int type;
    char temp[MAXTOKEN];
    char startemp[MAXTOKEN];

    while ((type = gettoken()) != '\n' && type != EOF)
    {
        if (type == PARENS || type == BRACKETS)
        {
            if (starcount) /* handle '*' before parentheses */
            {
                startemp[0] = '\0'; /* reset temp string */
                for (; starcount > 0; starcount--)
                    strcat(startemp, "*");
                sprintf(temp, "(%s%s)%s", startemp, out, token);
                strcpy(out, temp);
            }
            else
                strcat(out, token);
        }
        else if (type == '*')
            starcount += 1; /* set star flag for '*' in declaration */
        else if (type == NAME)
        {
            if (starcount) /* handle '*' before identifier */
            {
                startemp[0] = '\0'; /* reset temp string */
                for (; starcount > 0; starcount--)
                    strcat(startemp, "*");
                sprintf(temp, "%s%s", startemp, out);
                strcpy(out, temp);
            }
            sprintf(temp, "%s %s", token, out);
            strcpy(out, temp);
        }
        else
        {
            printf("invalid input at %s\n", token);
            err = 1;
        }
        prevtokentype = type; /* store previous token type for next iteration */
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