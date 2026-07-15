#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define ALLOCSIZE 100000 /* size of available space */
#define MAXLINELEN 1000  /* maximum input line length */
#define MAXLINES 1000    /* maximum number of lines */

void tail(int n);
void usage(void);
char *alloc(int n);
int get_line(char *s, int lim);
int isnumeric(char *s);
int readlines(char *lineptr[], int maxlines);

int main(int argc, char *argv[])
{
    int n;

    if (argc == 2)
    {
        if (--argc && (*++argv)[0] == '-' && isnumeric(*argv + 1))
            n = atoi(*argv + 1); /* pass numeric part after -*/
        else
        {
            usage();
            return 1;
        }
    }
    else if (argc == 1)
        n = 10; /* default number of lines */
    else
    {
        usage();
        return 1;
    }

    tail(n);
    return 0;
}

void usage(void)
{
    printf("usage: <program> -n\n");
    printf("parameters:\n");
    printf("\t-n\t: (optional) last n number of lines of its input. eg: -4\n");
    printf("sample command: ex-05-13 -3\n");
}

static char allocbuf[ALLOCSIZE]; /* storage for alloc */
static char *allocp = allocbuf;  /* next free position */

char *alloc(int n)
{
    if (allocbuf + ALLOCSIZE - allocp >= n)
    {
        allocp += n;
        return allocp - n; /* old p */
    }

    allocp = allocbuf; /* reset alloc pointer to start of buffer */
    return allocp;
}

int get_line(char *s, int lim)
{
    char *o = s;
    int c;
    while ((int)(s - o) < lim - 1 && (c = getchar()) != EOF && c != '\n')
        *s++ = c;

    if (c == '\n')
        *s++ = '\n';

    *s = '\0';
    return (int)(s - o);
}

int readlines(char *lineptr[], int maxlines)
{
    int len, nlines = 0, lpi = 0; /* line pointer index */
    char *p, line[MAXLINELEN];
    while ((len = get_line(line, MAXLINELEN)) > 0)
    {
        p = alloc(len + 1); /* +1 for string terminator */
        line[len] = '\0';   /* null terminate the line */
        strcpy(p, line);
        if (lpi >= maxlines) /* circular buffer: wrap around  */
            lpi = 0;

        lineptr[lpi++] = p;
        nlines++;
    }
    return nlines;
}

int isnumeric(char *s)
{
    int c;

    if (s == NULL || *s == '\0')
        return 0;

    while ((c = *s++) != '\0')
        if (!isdigit(c))
            return 0;

    return 1;
}

void tail(int n)
{
    int nlines = 0;          /* number of lines read */
    char *lineptr[MAXLINES]; /* array of pointers to lines */
    int endmark = 0;         /* end mark: index of the last line to print */
    int startmark = 0;       /* start mark: index of the first line to print */
    int plines = 0;          /* number of lines to print from end */

    if (n == 0)
        return; /* no lines to print */
    else
        plines = n; /* */

    nlines = readlines(lineptr, MAXLINES); /* read lines from input */

    if (nlines == 0)
        return;

    /* calculate endmark: index of the last line to print */
    endmark = (nlines % MAXLINES == 0) ? MAXLINES - 1 : nlines % MAXLINES - 1;

    if (plines > nlines) /* recalculate plines if it exceeds the number of lines read */
        plines = nlines;

    if (plines > MAXLINES) /* recalculate plines if it exceeds the maximum number of lines */
        plines = MAXLINES;

    /* calculate startmark: index of the first line to print */
    startmark = (endmark >= (plines - 1)) ? endmark - (plines - 1) : MAXLINES - (plines - 1 - endmark);

    while (--plines >= 0)
    {
        printf("%s", lineptr[startmark++]);

        if (startmark >= MAXLINES) /* wrap around if startmark exceeds MAXLINES */
            startmark = 0;
    }
}