#include <stdio.h>
#include <ctype.h>
#include <string.h>

/*  Our version of getword does not properly handle underscores, string constants,
comments, or preprocessor control lines. Write a better version. */

#define MAXWORD 100
#define NKEYS (sizeof(keytab) / sizeof(keytab[0]))

struct key
{
    char *word;
    int count;
} keytab[] = {
    {"auto", 0},
    {"break", 0},
    {"case", 0},
    {"char", 0},
    {"const", 0},
    {"continue", 0},
    {"default", 0},
    {"do", 0},
    {"double", 0},
    {"else", 0},
    {"enum", 0},
    {"extern", 0},
    {"float", 0},
    {"for", 0},
    {"goto", 0},
    {"if", 0},
    {"int", 0},
    {"long", 0},
    {"register", 0},
    {"return", 0},
    {"short", 0},
    {"signed", 0},
    {"sizeof", 0},
    {"static", 0},
    {"struct", 0},
    {"switch", 0},
    {"typedef", 0},
    {"union", 0},
    {"unsigned", 0},
    {"void", 0},
    {"volatile", 0},
    {"while", 0}};

int getword(char *, int);
int binsearch(const char *, struct key *, int);

/* count C keywords */
int main(void)
{
    int n;
    char word[MAXWORD];

    while (getword(word, MAXWORD) != EOF)
        if (isalpha(word[0]) || word[0] == '_')
            if ((n = binsearch(word, keytab, NKEYS)) >= 0)
                keytab[n].count++;

    for (n = 0; n < (int)NKEYS; n++)
        if (keytab[n].count > 0)
            printf("%4d %s\n", keytab[n].count, keytab[n].word);

    return 0;
}

/* binsearch: find word in tab[0]...tab[n-1]*/
int binsearch(const char *word, struct key tab[], int n)
{
    int cond, mid;

    int low = 0;
    int high = n - 1;

    while (low <= high)
    {
        mid = (low + high) / 2;
        if ((cond = strcmp(word, tab[mid].word)) < 0)
            high = mid - 1;
        else if (cond > 0)
            low = mid + 1;
        else
            return mid;
    }
    return -1;
}

int getch(void);
void ungetch(int);

/* getword: get next word or character from input*/
int getword(char *word, int lim)
{
    int skip_string_constants(void);
    int skip_comments(void);
    int skip_preprocessor_control_lines(void);

    int c, skip = 0;
    void ungetch(int);
    char *w = word;

    while (isspace(c = getch()))
        ;

    switch (c)
    {
    case '"':
        skip = skip_string_constants();
        break;
    case '/':
        skip = skip_comments();
        break;
    case '#':
        skip = skip_preprocessor_control_lines();
        break;
    }

    if (c != EOF)
        *w++ = (char)c;
    if (skip || (!isalpha(c) && c != '_'))
    {
        *w = '\0';
        return c;
    }

    for (; --lim > 0; w++)
        if (!isalnum(*w = (char)getch()) && *w != '_')
        {
            ungetch(*w);
            break;
        }
    *w = '\0';
    return word[0];
}

/* skip_string_constants: skip over string constants and return 1 if skipped, 0 otherwise */
int skip_string_constants(void)
{
    int c, temp = -1, skip = 0, slash_count = 0;
    while ((c = getch()) != EOF)
    {
        if (c == '\\')
            slash_count++;

        if (c == '"')
            if ((temp != '\\') || (temp == '\\' && ((slash_count % 2) == 0)))
            {
                skip = 1;
                break;
            }

        if (c != '\\')
            slash_count = 0;

        temp = c;
    }
    return skip;
}

/* skip_comments: skip over comments and return 1 if skipped, 0 otherwise */
int skip_comments(void)
{
    int c, temp = -1, skip = 0;

    if ((c = getch()) == '*')
    {
        while ((c = getch()) != EOF)
        {
            if (temp == '*' && c == '/')
            {
                skip = 1;
                break;
            }
            temp = c;
        }
    }
    else if (c == '/')
    {
        while ((c = getch()) != EOF)
            if (c == '\n')
            {
                skip = 1;
                break;
            }
    }
    else
        ungetch(c);

    return skip;
}

/* skip_preprocessor_control_lines: skip over preprocessor control lines and return 1 if skipped, 0 otherwise */
int skip_preprocessor_control_lines(void)
{
    int c, temp = -1, skip = 0;

    while ((c = getch()) != EOF)
    {
        if (temp != '\\' && (c == '\n' || c == EOF))
        {
            skip = 1;
            break;
        }
        temp = c;
    }

    return skip;
}

#define BUFSIZE 100

int buf[BUFSIZE]; /* buffer for ungetch*/
int bufp = 0;     /* next free position in buffer */

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