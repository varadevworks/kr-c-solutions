#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define MAXOPTS 103             /* maximum number of options */
#define MAXLMBUF 10             /* maximum length of length modifier buffer*/
#define SCANFAS '*'             /* scanf assignment suppression */
#define SCANFFW "0123456789"    /* scanf format width */
#define SCANFLM "hlL"           /* scanf length modifier */
#define SCANFCC "%diouxXcsefgp" /* scanf conversion character */

#define ADD_OPTS(c)                                \
    do                                             \
    {                                              \
        if (optindex < MAXOPTS - 3)                \
            opts[optindex++] = (c);                \
        else                                       \
        {                                          \
            fprintf(stderr, "Too many options\n"); \
            va_end(ap);                            \
            return -1;                             \
        }                                          \
    } while (0)

int minscanf(const char *fmt, ...);

int main(void)
{
    int a;
    unsigned int u;
    float b;
    double d;
    char str[100];
    long c;
    unsigned long ul;
    short unsigned int s;
    long double ld;
    int a1 = 0, b1 = 0, c1 = 0, r = 0;
    minscanf("%d", &a);
    printf("Entered decimal: %d\n", a);
    minscanf("%f", &b);
    printf("Entered float: %f\n", b);
    minscanf("%99s", str);
    printf("Entered string: %s\n", str);
    minscanf("%c", &str[0]);
    printf("Entered character: %c\n", str[0]);
    minscanf("%99[^\n]", str);
    printf("Entered string till newline: %s\n", str);
    minscanf("%ld", &c);
    printf("Entered long decimal: %ld\n", c);
    minscanf("%lf", &d);
    printf("Entered double: %f\n", d);
    minscanf("%u", &u);
    printf("Entered unsigned int: %u\n", u);
    minscanf("%hu", &s);
    printf("Entered short unsigned int: %hu\n", s);
    minscanf("%lu", &ul);
    printf("Entered long unsigned int: %lu\n", ul);
    minscanf("%Lf", &ld);
    printf("Entered long double: %Lf\n", ld);
    r = minscanf("%%%d", &a1);
    printf("Entered decimal: %d | %d | %d\n", a1, b1, c1);
    printf("minscanf return value: %d\n", r);
    return 0;
}

int minscanf(const char *fmt, ...)
{
    va_list ap; /* ap holds the next argument in the variable argument list */
    const char *p;
    char opts[MAXOPTS], lmbuf[MAXLMBUF];
    int optindex = 0, lmbufindex = 0; /* index for storing options in opts array and length modifier buffer */
    int scanset = 0;                  /* flag for bracketed scanset */
    int suppress = 0;                 /* flag for assignment suppression */
    int sr = 0;                       /* scanf return value for each individual scanf call */
    int r = 0;                        /* return value of minscanf */
    int c = -1;                       /* conversion character */
    int partial = 0;                  /* flag to indicate if a partial match occurred */
    int n = -1;                       /* number of characters successfully matched - suppressed assignments only */

    va_start(ap, fmt); /* set ap with the first variable argument */

    for (p = fmt; *p; p++)
    {

        if (*p != '%') /* read untill the next '%' character */
        {
            ADD_OPTS(*p);
            continue;
        }

        ADD_OPTS(*p++);

        /* (Optional) check for assignment suppression */
        if (*p == SCANFAS)
        {
            suppress = 1;
            ADD_OPTS(*p++);
        }

        /* (Optional) store field width */
        while (*p && (*p >= '0' && *p <= '9'))
            ADD_OPTS(*p++);

        /* (Optional) store length modifier */
        while (*p && strchr(SCANFLM, *p))
        {
            lmbuf[lmbufindex++] = *p;
            ADD_OPTS(*p++);
        }

        /* (required) store conversion character */
        while (*p && strchr(SCANFCC, *p))
            ADD_OPTS(*p++);
        if (*p == '[')
        {
            scanset = 1;
            ADD_OPTS(*p++);

            if (*p == '^')
                ADD_OPTS(*p++);
            if (*p == ']')
                ADD_OPTS(*p++);

            while (*p && *p != ']')
                ADD_OPTS(*p++);

            if (*p == ']')
                ADD_OPTS(*p++);
        }

        opts[optindex] = '\0';    /* null-terminate the options string */
        lmbuf[lmbufindex] = '\0'; /* null-terminate the length modifier buffer */

        --p; /* move back one character to point to the conversion character */
        c = (suppress) ? 'q' : *p;

        switch (c)
        {
        case 'd':
        case 'i':
        case 'o':
        case 'x':
        case 'X':
            if (strcmp(lmbuf, "h") == 0)
                sr = scanf(opts, va_arg(ap, short *));
            else if (strcmp(lmbuf, "l") == 0)
                sr = scanf(opts, va_arg(ap, long *));
            else
                sr = scanf(opts, va_arg(ap, int *));
            break;
        case 'u':
            if (strcmp(lmbuf, "h") == 0)
                sr = scanf(opts, va_arg(ap, unsigned short *));
            else if (strcmp(lmbuf, "l") == 0)
                sr = scanf(opts, va_arg(ap, unsigned long *));
            else
                sr = scanf(opts, va_arg(ap, unsigned int *));
            break;
        case 'e':
        case 'f':
        case 'g':
            if (strcmp(lmbuf, "l") == 0)
                sr = scanf(opts, va_arg(ap, double *));
            else if (strcmp(lmbuf, "L") == 0)
                sr = scanf(opts, va_arg(ap, long double *));
            else
                sr = scanf(opts, va_arg(ap, float *));
            break;
        case 'p':
            sr = scanf(opts, va_arg(ap, void **));
            break;
        case 'c':
        case ']':
        case 's':
            sr = scanf(opts, va_arg(ap, char *));
            break;
        case 'q':
        case '%':
            opts[optindex++] = '%';
            opts[optindex++] = 'n';
            opts[optindex] = '\0'; /* null-terminate the options string */
            sr = scanf(opts, &n);  /* perform the suppressed scanf */
            suppress = 1;          /* set the suppression flag */
            break;
        default:
            if (scanset)
                sr = scanf(opts, va_arg(ap, char *));
            break;
        }
        /* break: End of Input or no-suppressed assignment mismatch, or suppressed assignment mismatch */
        if (sr == EOF || (!suppress && sr == 0) || (suppress && n < 0))
        {
            partial = 1; /* set the partial match flag */
            break;
        }
        r += sr;

        optindex = 0;   /* reset option index */
        scanset = 0;    /* reset bracket flag */
        lmbufindex = 0; /* reset length modifier buffer index */
        sr = 0;         /* reset scanf return value for each individual scanf call */
        suppress = 0;   /* reset the suppression flag */
        n = -1;         /* reset the suppressed assignment counter */
    }

    va_end(ap);            /* clean up the variable argument list */
    opts[optindex] = '\0'; /* null-terminate the options string */

    if (!partial && optindex > 0) /* leftover options to be processed */
        sr = scanf(opts);

    return (r == 0 && sr == EOF) ? EOF : r;
}