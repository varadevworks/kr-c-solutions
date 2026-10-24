#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define MAXOPTS 100           /* maximum number of options */
#define MAXLMBUF 10           /* maximum length of length modifier buffer*/
#define SCANFAS '*'           /* scanf assignment suppression */
#define SCANFFW "0123456789"  /* scanf format width */
#define SCANFLM "hlL"         /* scanf length modifier */
#define SCANFCC "diouxcsefgp" /* scanf conversion character */

int minscanf(const char *fmt, ...);

int main(void)
{
    int a;
    float b;
    double d;
    char str[100];
    long c;
    unsigned long ul;
    short unsigned int s;
    long double ld;
    minscanf("Enter decimal: %d", &a);
    printf("Entered decimal: %d\n", a);
    minscanf("Enter float: %f", &b);
    printf("Entered float: %f\n", b);
    minscanf("Enter string: %99s", str);
    printf("Entered string: %s\n", str);
    minscanf("Enter character: %c", &str[0]);
    printf("Entered character: %c\n", str[0]);
    minscanf("Enter string till newline: %99[^\n]", str);
    printf("Entered string till newline: %s\n", str);
    minscanf("Enter long decimal: %ld", &c);
    printf("Entered long decimal: %ld\n", c);
    minscanf("Enter double: %lf", &d);
    printf("Entered double: %f\n", d);
    minscanf("Enter unsigned int: %u", &a);
    printf("Entered unsigned int: %u\n", a);
    minscanf("Enter short unsigned int: %hu", &s);
    printf("Entered short unsigned int: %hu\n", s);
    minscanf("Enter long unsigned int: %lu", &ul);
    printf("Entered long unsigned int: %lu\n", ul);
    minscanf("Enter long double: %Lf", &ld);
    printf("Entered long double: %Lf\n", ld);
    return 0;
}

int minscanf(const char *fmt, ...)
{
    va_list ap; /* ap holds the next argument in the variable argument list */
    const char *p;
    char opts[100], lmbuf[MAXLMBUF];
    int optindex = 0, lmbufindex = 0; /* index for storing options in opts array and length modifier buffer */
    int assign_suppression = 0;       /* flag for assignment suppression */
    int bracket = 0;                  /* flag for bracketed scanset */
    int r = 0;                        /* return value of scanf */

    va_start(ap, fmt); /* set ap with the first variable argument */

    for (p = fmt; *p; p++)
    {
        if (*p != '%')
        {
            putchar(*p); /* print the character as it is not part of a format specifier */
            continue;
        }

        assign_suppression = 0; /* reset assignment suppression flag */
        optindex = 0;           /* reset option index */
        bracket = 0;            /* reset bracket flag */

        opts[optindex++] = '%'; /* store the '%' character */

        /* (Optional) check for assignment suppression */
        if (*++p == SCANFAS)
        {
            assign_suppression = 1;
            opts[optindex++] = '*';
            p++;
        }

        /* (Optional) store field width */
        while (*p && (*p >= '0' && *p <= '9'))
            opts[optindex++] = *p++;

        /* (Optional) store length modifier */
        while (*p && strchr(SCANFLM, *p))
        {
            lmbuf[lmbufindex++] = *p;
            opts[optindex++] = *p++;
        }

        /* (required) store conversion character */
        if (*p && strchr(SCANFCC, *p))
            opts[optindex++] = *p++;
        if (*p == '[')
        {
            bracket = 1;
            opts[optindex++] = *p++;

            while (*p && *p != ']')
                opts[optindex++] = *p++;

            if (*p == ']')
                opts[optindex++] = *p++;
        }

        opts[optindex] = '\0';    /* null-terminate the options string */
        lmbuf[lmbufindex] = '\0'; /* null-terminate the length modifier buffer */

        if (assign_suppression)
            continue; /* skip assignment if suppression flag is set */

        if (optindex > 1)
            p = p - 1;

        switch (*p)
        {
        case 'd':
        case 'i':
        case 'o':
        case 'x':
        case 'X':
            if (strcmp(lmbuf, "h") == 0)
                scanf(opts, (short *)va_arg(ap, int *));
            else if (strcmp(lmbuf, "l") == 0)
                scanf(opts, (long *)va_arg(ap, long *));
            else
                scanf(opts, va_arg(ap, int *));
            break;
        case 'u':
            if (strcmp(lmbuf, "h") == 0)
                scanf(opts, (unsigned short *)va_arg(ap, unsigned int *));
            else if (strcmp(lmbuf, "l") == 0)
                scanf(opts, (unsigned long *)va_arg(ap, unsigned long *));
            else
                scanf(opts, va_arg(ap, unsigned int *));
            break;
        case 'e':
        case 'f':
        case 'g':
            if (strcmp(lmbuf, "l") == 0)
                scanf(opts, va_arg(ap, double *));
            else if (strcmp(lmbuf, "L") == 0)
                scanf(opts, va_arg(ap, long double *));
            else
                scanf(opts, va_arg(ap, float *));
            break;
        case 'p':
            scanf(opts, va_arg(ap, void *));
            break;
        case 'c':
            scanf(opts, va_arg(ap, char *));
            break;
        case 's':
            scanf(opts, va_arg(ap, char *));
            break;
        default:
            if (bracket)
                scanf(opts, va_arg(ap, char *));
            break;
        }
        r++;
    }

    va_end(ap); /* clean up the variable argument list */
    return r;
}