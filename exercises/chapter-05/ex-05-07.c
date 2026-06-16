#include <stdio.h>
#define MAXLINES 1000
#define MAXLINELEN 500

/* readlines: read input lines */
int readlines(char *lineptr[], int maxlines);
/* get_line: read input line*/
int get_line(char *s, int lim);

int main(void)
{
    int nlines;
    char *lineptr[MAXLINES];
    char line[MAXLINELEN];

    // nlines = readlines(lineptr, MAXLINES);
    // printf("no of input lines read: %d", nlines);
    nlines = get_line(line, MAXLINELEN);
    printf("line: length %d , data %s", nlines, line);

    return 0;
}

int get_line(char *s, int lim)
{
    char *o = s;
    char c;
    while ((int)(s - o) < lim - 1 && (c = getchar()) != EOF && c != '\n')
        *s++ = c;

    if (c == '\n')
        *s++ = '\n';

    *s = '\0';

    return (int)(s - o);
}