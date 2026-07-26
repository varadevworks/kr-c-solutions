#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* Macros */
#define ALLOCSIZE 5000000  /* size of available space */
#define MAXLINES 5000      /* maximum number of lines to be sorted */
#define MAXLINELEN 1000    /* maximum length of any input line */
#define SORTTYPES 6        /* 0 - start, 1 - end, 2 - numeric, 3 - reverse, 4 - fold, 5 - directory */
#define MAXSORTKEYCOUNT 50 /* maximum number of sort keys */
#define FIELDSEP '\t'

/* Global variables */
static char *lineptr[MAXLINES];                 /* pointers to text lines */
static int options[MAXSORTKEYCOUNT][SORTTYPES]; /* stores sort key options */
static int keycnt = 0;                          /* number of sort keys */
static char allocbuf[ALLOCSIZE];                /* storage for alloc */
static char *allocp = allocbuf;                 /* next free position */

/* Function prototypes */
static int parse_options(int argc, char *argv[]);
static int readlines(char *lineptr[], int maxlines);
static void writelines(char *lineptr[], int nlines);
static void quicksort(void *lineptr[], int left, int right, int (*comp)(void *, void *));
static int comparator(const char *, const char *);

/* sort input lines */
int main(int argc, char *argv[])
{
    int nlines; /* number of input lines read */
    int ret = 0;
    int (*comp)(const char *, const char *) = comparator;

    ret = parse_options(argc, argv);
    if (ret != 0)
        return ret;

    if ((nlines = readlines(lineptr, MAXLINES)) >= 0)
    {
        quicksort((void **)lineptr, 0, nlines - 1, (int (*)(void *, void *))comp);
        writelines(lineptr, nlines);
        return 0;
    }
    else
    {
        printf("error: input too big to sort\n");
        return 1;
    }
}

/* usage: print usage information */
static void usage(void)
{
    printf("Usage: argvl [options]\n");
    printf("Options:\n");
    printf("  -n : numeric sort\n");
    printf("  -r : reverse order\n");
    printf("  -f : case insensitive (fold)\n");
    printf("  -d : directory order\n");
    printf("  +<start> : specify start column for sort key, can follow with options\n");
    printf("  -<end> : specify end column for sort key\n");
}

/* print_options: print parsed sort key options */
static void print_options(void)
{
    printf("Parsed sort key options:\n");
    for (int i = 0; i < keycnt; i++)
    {
        printf("Sort Key %d: Start Column: %d, End Column: %d, Numeric: %d, Reverse: %d, Fold: %d, Directory: %d\n",
               i + 1,
               options[i][0],
               options[i][1],
               options[i][2],
               options[i][3],
               options[i][4],
               options[i][5]);
    }
}

/* parse_options: parse command line options */
static int parse_options(int argc, char *argv[])
{
    int start = -1;      /* start column index, -1 (default - no start column specified) */
    int end = -1;        /* end column index, -1 (default - no end column specified) */
    int numeric = 0;     /* global: 1 if numeric sort, 0 for non numeric sort */
    int reverse = 0;     /* global: 1 for reverse order, 0 for normal order */
    int fold = 0;        /* global: 1 for case insensitive, 0 for case sensitive */
    int directory = 0;   /* global: 1 for directory order, 0 for normal order */
    int tnumeric = -1;   /* temp: 1 if numeric sort, 0 for non numeric sort */
    int treverse = -1;   /* temp: 1 for reverse order, 0 for normal order */
    int tfold = -1;      /* temp: 1 for case insensitive, 0 for case sensitive */
    int tdirectory = -1; /* temp: 1 for directory order, 0 for normal order */

    int ch = 0;     /* hold first character of the argument to check `+` or `-` */
    int prevch = 0; /* hold previous character used to check state change from `+` to  `-` */
    char *lastp;    /* hold the last argument pointer to check digits after '+' */

    while (--argc > 0)
    {
        ch = (*++argv)[0]; /* get the first character of the current argument */

        if (ch == '-') /* handle options starts with : '-' */
        {
            if (isdigit((*argv)[1]))
            {
                if (keycnt < MAXSORTKEYCOUNT && prevch == '+')
                {
                    end = atoi(*argv + 1);
                    if (end < start)
                    {
                        printf("error: end column %d is less than start column %d\n", end, start);
                        usage();
                        return 1;
                    }
                    options[keycnt][0] = start;
                    options[keycnt][1] = end;
                    options[keycnt][2] = (tnumeric != -1 ? tnumeric : numeric);
                    options[keycnt][3] = (treverse != -1 ? treverse : reverse);
                    options[keycnt][4] = (tfold != -1 ? tfold : fold);
                    options[keycnt][5] = (tdirectory != -1 ? tdirectory : directory);
                    start = -1;
                    end = -1;
                    tnumeric = -1;
                    treverse = -1;
                    tfold = -1;
                    tdirectory = -1;
                    prevch = 0;
                    keycnt++;
                }
                else
                {
                    printf("error: end column specified without a start column\n");
                    return 1;
                }
            }
            else
                while ((ch = *++*argv))
                    switch (ch)
                    {
                    case 'n':
                        numeric = 1;
                        break;
                    case 'r':
                        reverse = 1;
                        break;
                    case 'f':
                        fold = 1;
                        break;
                    case 'd':
                        directory = 1;
                        break;
                    default:
                        printf("error: illegal option %c\n", ch);
                        usage();
                        return 1;
                    }
        }
        else if (ch == '+') /* handle options starts with : '+' */
        {
            /* scan the digits */
            lastp = *argv;
            while (isdigit((*argv)[1]))
                ++*argv;

            if (isdigit(lastp[1]))
            {
                start = atoi(lastp + 1);
                prevch = ch;
            }

            while ((ch = *++*argv))
                switch (ch)
                {
                case 'n':
                    tnumeric = 1;
                    break;
                case 'r':
                    treverse = 1;
                    break;
                case 'f':
                    tfold = 1;
                    break;
                case 'd':
                    tdirectory = 1;
                    break;
                default:
                    printf("error: illegal option %c\n", ch);
                    usage();
                    return 1;
                }
        }
        else /* handle illegal options */
        {
            printf("error: illegal option %c\n", ch);
            usage();
            return 1;
        }
    }

    // save last parsed options if end column is not specified
    if (keycnt < MAXSORTKEYCOUNT && prevch == '+')
    {
        options[keycnt][0] = start;
        options[keycnt][1] = end;
        options[keycnt][2] = (tnumeric != -1 ? tnumeric : numeric);
        options[keycnt][3] = (treverse != -1 ? treverse : reverse);
        options[keycnt][4] = (tfold != -1 ? tfold : fold);
        options[keycnt][5] = (tdirectory != -1 ? tdirectory : directory);
        start = -1;
        end = -1;
        prevch = 0;
        keycnt++;
    }

    print_options(); // Print parsed options for debugging

    return 0;
}

/* alloc: return pointer to n characters */
static char *alloc(int n)
{
    if (allocbuf + ALLOCSIZE - allocp >= n)
    {
        allocp += n;
        return allocp - n; /* old p */
    }

    printf("error: alloc failed\n");
    return 0; /* not enough room */
}

/* get_line: read a line into s, return length */
static int get_line(char *s, int lim)
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

/* readlines: read input lines */
static int readlines(char *lineptr[], int maxlines)
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

/* writelines: write output lines */
void writelines(char *lineptr[], int nlines)
{
    while (nlines-- > 0)
        printf("%s", *lineptr++);
}

/* swap: interchange v[i] and v[j] */
void swap(void *v[], int i, int j)
{
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

/* quicksort: sort v[left]...v[right] into increasing order */
static void quicksort(void *v[], int left, int right, int (*comp)(void *, void *))
{
    int i, last;
    void swap(void *v[], int i, int j);

    if (left >= right) /* do nothing if array contains */
        return;        /* fewer than two elements */

    swap(v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i <= right; i++)
    {
        if ((*comp)(v[i], v[left]) < 0)
            swap(v, ++last, i);
    }

    swap(v, left, last);
    quicksort(v, left, last - 1, comp);
    quicksort(v, last + 1, right, comp);
}

/* numcmp: compare s1 and s2 numerically */
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

/* strcasecmp: compare s1 and s2 case-insensitively */
int strcasecmp(const char *s1, const char *s2)
{
    for (; tolower(*s1) == tolower(*s2); s1++, s2++)
        if (*s1 == '\0')
            return 0;

    return tolower(*s1) - tolower(*s2);
}

/* isdircase: check if character is valid in directory order */
int isdircase(char c)
{
    return isalnum((unsigned char)c) || c == ' ';
}

/* strdircmp: compare s1 and s2 in directory order */
int strdircmp(const char *s1, const char *s2)
{
    while (*s1 != '\0' && *s2 != '\0') /* check both strings are not at the end */
    {
        while (*s1 != '\0' && !isdircase(*s1)) /* string s1: skip non directory characters */
            s1++;

        while (*s2 != '\0' && !isdircase(*s2)) /* string s2: skip non directory characters */
            s2++;

        if (*s1 != *s2 || *s1 == '\0' || *s2 == '\0') /* stop if characters differ or end of either string */
            break;

        s1++; /* move to the next character in s1 */
        s2++; /* move to the next character in s2 */
    }

    return *s1 - *s2;
}

/* strdircasecmp: compare s1 and s2 in directory order case-insensitively */
int strdircasecmp(const char *s1, const char *s2)
{
    while (*s1 != '\0' && *s2 != '\0') /* check both strings are not at the end */
    {
        while (*s1 != '\0' && !isdircase(*s1)) /* string s1: skip non directory characters */
            s1++;

        while (*s2 != '\0' && !isdircase(*s2)) /* string s2: skip non directory characters */
            s2++;

        if (tolower(*s1) != tolower(*s2) || *s1 == '\0' || *s2 == '\0') /* stop if characters differ or end of either string */
            break;

        s1++; /* move to the next character in s1 */
        s2++; /* move to the next character in s2 */
    }

    return tolower(*s1) - tolower(*s2);
}

/* copy_field: copy field to target*/
int copy_field(char *t, const char *s, int start, int end)
{
    int i = 0, fcnt = 0;

    /* s: advance to start position*/
    for (; *s && fcnt < start; s++)
        if (*s == '\t')
            ++fcnt;

    /* copy string to t till it reaches end */
    for (; *s != '\0'; i++, s++)
    {
        if (*s == '\t')
            ++fcnt;
        if (end >= 0 && fcnt >= end)
            break;
        *(t + i) = *s;
    }

    *(t + i) = '\0'; // Null-terminate the target string

    return i;
}

/* comparator: compare s1 and s2 based on sort options */
int comparator(const char *s1, const char *s2)
{
    char sline[MAXLINELEN];
    char tline[MAXLINELEN];
    int result = 0;
    int (*cmp)(const char *, const char *) = strcmp; /* default comparison function */

    for (int i = 0; i < keycnt; i++)
    {
        int start = options[i][0];
        int end = options[i][1];
        int numeric = options[i][2];
        int reverse = options[i][3];
        int fold = options[i][4];
        int directory = options[i][5];

        /* s1: copy field to sline */
        copy_field(sline, s1, start, end);

        /* s2: copy field to tline */
        copy_field(tline, s2, start, end);

        if (fold)
            cmp = strcasecmp;

        if (directory)
            cmp = strdircmp;

        if (directory && fold)
            cmp = strdircasecmp;

        if (numeric)
            cmp = numcmp;

        result = cmp(sline, tline);

        if (result != 0) /* if equal, continue to next sort key */
            return reverse * result;
    }

    return 0; /* all fields are equal */
}
