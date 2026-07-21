#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define ALLOCSIZE 5000000 /* size of available space */
#define MAXLINES 5000     /* maximum number of lines to be sorted */
#define MAXLINELEN 1000   /* maximum length of any input line */
char *lineptr[MAXLINES];  /* pointers to text lines */

int readlines(char *lineptr[], int maxlines);
void writelines(char *lineptr[], int nlines);

void quicksort(void *lineptr[], int left, int right, int (*comp)(void *, void *), int order);
int numcmp(const char *, const char *);
int strcasecmp(const char *, const char *);
int strdircmp(const char *, const char *);
int strdircmp(const char *, const char *);
int strdircasecmp(const char *, const char *);
int isdircase(char c);

/* sort input lines */
int main(int argc, char *argv[])
{
    int nlines;        /* number of input lines read */
    int numeric = 0;   /* 1 if numeric sort, 0 for non numeric sort */
    int order = 1;     /* 1 for ascending order, -1 for descending order */
    int fold = 0;      /* 0 for case sensitive, 1 for case insensitive*/
    int directory = 0; /* 0 for normal, 1 for directory order */
    int ch, rc = 0;    /* character and return code */

    int (*cmpfunc)(const char *, const char *) = strcmp; /* default comparison function */

    while (--argc > 0 && (*++argv)[0] == '-')
        while ((ch = *++*argv))
            switch (ch)
            {
            case 'n':
                numeric = 1;
                break;
            case 'r':
                order = -1;
                break;
            case 'f':
                fold = 1;
                break;
            case 'd':
                directory = 1;
                break;
            default:
                printf("error: illegal option %c\n", ch);
                rc = 1;
                break;
            }

    if (rc != 0 || argc != 0)
    {
        printf("usage: ex-05-15 [-nrf]\n");
        rc = 1;
    }
    else
    {
        if (fold)
            cmpfunc = strcasecmp;

        if (directory)
            cmpfunc = strdircmp;

        if (directory && fold)
            cmpfunc = strdircasecmp;

        if (numeric)
            cmpfunc = numcmp;

        if ((nlines = readlines(lineptr, MAXLINES)) >= 0)
        {
            quicksort((void **)lineptr, 0, nlines - 1,
                      (int (*)(void *, void *))cmpfunc, order);
            writelines(lineptr, nlines);
        }
        else
        {
            printf("input too big to sort\n");
            rc = 1;
        }
    }

    return rc;
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
    {
        if (order * ((*comp)(v[i], v[left])) < 0)
            swap(v, ++last, i);
    }

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

int strcasecmp(const char *s1, const char *s2)
{
    for (; tolower(*s1) == tolower(*s2); s1++, s2++)
        if (*s1 == '\0')
            return 0;

    return tolower(*s1) - tolower(*s2);
}

int strdircmp(const char *s1, const char *s2)
{
    int f1 = 0, f2 = 0;
    int cs1 = 0, cs2 = 0;
    while (*s1 != '\0' && *s2 != '\0')
    {
        if (!f1 && *s1 != '\0')
        {
            if (isdircase(*s1))
            {
                f1 = 1;
                cs1 = *s1++;
            }
            else
                s1++;
        }

        if (!f2 && *s2 != '\0')
        {
            if (isdircase(*s2))
            {
                f2 = 1;
                cs2 = *s2++;
            }
            else
                s2++;
        }

        if (f1 && f2)
        {
            if (cs1 != cs2)
                return cs1 - cs2;
            f1 = 0;
            f2 = 0;
        }
    }

    if (!f1)
    {
        while (*s1 != '\0' && !isdircase(*s1))
            s1++;
        cs1 = *s1;
    }

    if (!f2)
    {
        while (*s2 != '\0' && !isdircase(*s2))
            s2++;
        cs2 = *s2;
    }

    return cs1 - cs2;
}

int strdircasecmp(const char *s1, const char *s2)
{
    int f1 = 0, f2 = 0;
    int cs1 = 0, cs2 = 0;
    while (*s1 != '\0' && *s2 != '\0')
    {
        if (!f1 && *s1 != '\0')
        {
            if (isdircase(*s1))
            {
                f1 = 1;
                cs1 = tolower(*s1++);
            }
            else
                s1++;
        }

        if (!f2 && *s2 != '\0')
        {
            if (isdircase(*s2))
            {
                f2 = 1;
                cs2 = tolower(*s2++);
            }
            else
                s2++;
        }

        if (f1 && f2)
        {
            if (cs1 != cs2)
                return cs1 - cs2;
            f1 = 0;
            f2 = 0;
        }
    }

    if (!f1)
    {
        while (*s1 != '\0' && !isdircase(*s1))
            s1++;
        cs1 = tolower(*s1);
    }

    if (!f2)
    {
        while (*s2 != '\0' && !isdircase(*s2))
            s2++;
        cs2 = tolower(*s2);
    }

    return cs1 - cs2;
}

inline int isdircase(char c)
{
    return isalnum((unsigned char)c) || isblank((unsigned char)c);
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