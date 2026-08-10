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

char *storage_class[] = {"auto", "register", "static", "extern", "typedef", NULL};
char *type_specifier[] = {"void", "char", "short", "int", "long", "float", "double", "signed", "unsigned", NULL};
char *type_qualifier[] = {"const", "volatile", NULL};

void dcl(void);
void dirdcl(void);
int save_datatype(char *datatype, char *token);
int find_storage_class(char *token);
int find_type_specifier(char *token);
int find_type_qualifier(char *token);

int gettoken(void);
int tokentype;           /*type of last token*/
int prevtokentype;       /* type of previous token */
char token[MAXTOKEN];    /* last token string */
char name[MAXTOKEN];     /* identifier name */
char datatype[MAXTOKEN]; /* data type = char, int, etc. */
char out[MAXOUT];        /* output string */
int err = 0;             /* error flag */

int main(void) /* convert declaration to words */
{
    while (gettoken() != EOF)
    {
        datatype[0] = '\0';
        while (tokentype == NAME && save_datatype(datatype, token)) /* retrieve datatype */
        {
            if (gettoken() == EOF)
            {
                printf("error: unexpected end of input\n");
                return 1;
            }
        };

        if (strlen(datatype) > 0)
            datatype[strlen(datatype) - 1] = '\0'; /* remove trailing space */

        out[0] = '\0';

        prevtokentype = tokentype; /* store the previous token type */
        dcl();                     /* parse rest of line */
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
        printf("%s:%s %s\n", name, out, datatype);
    }

    return 0;
}

/* dcl: parse a declarator */
void dcl(void)
{
    char pointer_q[MAXTOKEN][MAXTOKEN]; /* array to hold pointer qualifiers */
    int pointer_q_index = 0;

    if (prevtokentype == tokentype)
        prevtokentype = -1;
    else
        gettoken();

    while (tokentype == '*')
    {
        pointer_q[pointer_q_index][0] = '\0';                    /* initialize the qualifier string */
        while (gettoken() == NAME && find_type_qualifier(token)) /* retrieve datatype */
        {
            strcat(pointer_q[pointer_q_index], " ");
            strcat(pointer_q[pointer_q_index], token);
        }

        strcat(pointer_q[pointer_q_index], " pointer to");
        pointer_q_index++; /* move to next index for the next pointer */
    }

    dirdcl();

    if (err)
        return;

    for (int i = pointer_q_index - 1; i >= 0; i--) /* print pointer qualifiers in reverse order */
        strcat(out, pointer_q[i]);
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

int find_storage_class(char *token)
{
    for (int i = 0; storage_class[i] != NULL; i++)
    {
        if (strcmp(storage_class[i], token) == 0)
            return 1;
    }
    return 0;
}

int find_type_specifier(char *token)
{
    for (int i = 0; type_specifier[i] != NULL; i++)
    {
        if (strcmp(type_specifier[i], token) == 0)
            return 1;
    }
    return 0;
}

int find_type_qualifier(char *token)
{
    for (int i = 0; type_qualifier[i] != NULL; i++)
    {
        if (strcmp(type_qualifier[i], token) == 0)
            return 1;
    }
    return 0;
}

int save_datatype(char *datatype, char *token)
{
    int found = 0;

    if (find_storage_class(token))
    {
        strcat(datatype, token);
        strcat(datatype, " ");
        found = 1;
    }
    else if (find_type_specifier(token))
    {
        strcat(datatype, token);
        strcat(datatype, " ");
        found = 1;
    }
    else if (find_type_qualifier(token))
    {
        strcat(datatype, token);
        strcat(datatype, " ");
        found = 1;
    }
    return found;
}