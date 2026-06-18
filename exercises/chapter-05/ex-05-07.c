#include <stdio.h>
#include <string.h>

#define MAXLINES 1000
#define MAXLINELEN 500

int readlines(char lines[], int maxlines, size_t total_storage);
int get_line(char *s, int lim);

int main(void)
{
    int nlines;
    size_t total_storage = MAXLINES * MAXLINELEN;
    char lines[total_storage];

    nlines = readlines(lines, MAXLINES, total_storage);
    printf("no of input lines read: %d", nlines);

    return 0;
}

/* readlines: read input lines */
int readlines(char lines[], int maxlines, size_t total_storage)
{
    int len, nlines;
    char line[MAXLINELEN];
    char *lineptr = lines;

    nlines = 0;

    while ((len = get_line(line, MAXLINELEN)) > 0)
    {
        if (nlines >= maxlines || (int)total_storage < ((int)(lineptr - lines) + len))
            return -1;
        else
        {
            if (line[len - 1] == '\n')
                line[len - 1] = '\0';
            strcpy(lineptr, line);
            lineptr += len;
            nlines++;
        }
    }
    return nlines;
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