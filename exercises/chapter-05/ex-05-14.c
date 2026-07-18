#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ALLOCSIZE 5000000 /* size of available space */
#define MAXLINES 5000     /* maximum number of lines to be sorted */
#define MAXLINELEN 1000   /* maximum length of any input line */
char *lineptr[MAXLINES];  /* pointers to text lines */

int readlines(char *lineptr[], int maxlines);
void writelines(char *lineptr[], int nlines);

void quicksort(void *lineptr[], int left, int right, int (*comp)(void *, void *), int order);
int numcmp(const char *, const char *);

/* sort input lines */
int main(int argc, char *argv[])
{
    int nlines;      /* number of input lines read */
    int numeric = 0; /* 1 if numeric sort */
    int order = 1;   /* 1 for ascending order, -1 for descending order */

    while (--argc > 0)
    {
        if (strcmp(*++argv, "-n") == 0)
            numeric = 1;
        else if (strcmp(*argv, "-r") == 0)
            order = -1;
        else
        {
            printf("usage: ex-05-14 [-n] [-r]\n");
            return 1;
        }
    }

    if ((nlines = readlines(lineptr, MAXLINES)) >= 0)
    {
        quicksort((void **)lineptr, 0, nlines - 1,
                  (int (*)(void *, void *))(numeric ? numcmp : strcmp), order);
        writelines(lineptr, nlines);
        return 0;
    }
    else
    {
        printf("input too big to sort\n");
        return 1;
    }
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

    printf("error: alloc failed\n");
    return 0; /* not enough room */
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

        if (p == 0) /* allocation failed */
        {
            printf("error: allocation failed\n");
            return -1;
        }

        line[len] = '\0'; /* null terminate the line */
        strcpy(p, line);  /* copy line to allocated buffer */

        if (lpi < maxlines) /* check for maximum lines */
            lineptr[lpi++] = p;
        else
        {
            printf("error: too many lines\n");
            return -1;
        }

        nlines++;
    }

    return nlines;
}

void quicksort(void *v[], int left, int right, int (*comp)(void *, void *), int order)
{
    int i, last;
    void swap(void *v[], int i, int j);

    if (left >= right) /* do nothing if array contains */
        return;        /* fewer than two elements */

    swap(v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i <= right; i++)
        if (order * ((*comp)(v[i], v[left])) < 0)
            swap(v, ++last, i);

    swap(v, left, last);
    quicksort(v, left, last - 1, comp, order);
    quicksort(v, last + 1, right, comp, order);
}

int numcmp(const char *s1, const char *s2)
{
    double v1, v2;

    v1 = atof(s1);
    v2 = atof(s2);
    if (v1 < v2)
        return -1;
    else if (v1 > v2)
        return 1;
    else
        return 0;
}

void swap(void *v[], int i, int j)
{
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

void writelines(char *lineptr[], int nlines)
{
    while (nlines-- > 0)
        printf("%s", *lineptr++);
}