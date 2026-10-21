#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define FMTBUFSIZE 100
#define PRINTFFLAGS "-+ 0#"                    /* flags for printf */
#define PRINTFWP "0123456789*"                 /* minimum field width or precision */
#define PRINTFWPSEP '.'                        /* field width and precision separator */
#define PRINTFLENMODIFIERS "hlL"               /* length modifiers for printf */
#define PRINTFCONVSPECIFIERS "dioxXucsfeEgGp%" /* conversion specifiers for printf */
#define MAXLENMODBUF 3                         /* maximum length of length modifier buffer */

void minprintf(char *fmt, ...);

int main(void)
{
    int ival = 42;
    minprintf("No format specifiers here.\n");
    minprintf("Hello, %s!\n", "world");
    minprintf("The answer is %d\n", 42);
    minprintf("Signed short: %hd\n", (short)42);
    minprintf("Signed long: %ld\n", 1234567890L);
    minprintf("Signed long long: %lld\n", 123456789012345LL);
    minprintf("Unsigned short: %hu\n", (unsigned short)300);
    minprintf("Unsigned long: %lu\n", 1234567890UL);
    minprintf("Unsigned long long: %llu\n", 123456789012345ULL);
    minprintf("Character: %c\n", 'A');
    minprintf("Character with width: %5c\n", 'A');
    minprintf("Hex: %x\n", 255);
    minprintf("Pointer: %p\n", &ival);
    minprintf("String: %s\n", "test");
    minprintf("Unsigned: %u\n", 300);
    minprintf("Octal: %o\n", 300);
    minprintf("Floating point: %f\n", 3.14);
    minprintf("Exponential: %e\n", 3.14);
    minprintf("General format: %g\n", 3.14);
    minprintf("Long double: %Lf\n", 3.141592653589793238L);
    minprintf("Percent sign: %%\n");
    minprintf("Left-justified: %-10d!\n", 42);
    minprintf("Zero-padded: %010d!\n", 42);
    minprintf("Sign: %+d!\n", 42);
    minprintf("Space: % d!\n", 42);
    minprintf("Width: %5d!\n", 42);
    minprintf("Precision: %.2f!\n", 3.14159);
    minprintf("Length modifier: %ld!\n", 123456789012345L);
    minprintf("Pointer with width and precision: |%*.*p|\n", 20, 5, &ival);
    minprintf("Integer with width and precision: |%*.*d|\n", 10, 5, ival);
    minprintf("Unknown format specifier: %qbbc\n", 42);
    minprintf("Ending with %");
    return 0;
}

/* minprintf: minimal printf with variable argument list */
void minprintf(char *fmt, ...)
{
    va_list ap;                                             /* points to each unnamed argument in turn */
    char *p, *sval;                                         /* string value */
    int ival;                                               /* integer value */
    unsigned int uval;                                      /* unsigned integer value */
    long lval;                                              /* long integer value */
    long long llval;                                        /* long long integer value */
    unsigned long ulval;                                    /* unsigned long integer value */
    unsigned long long ullval;                              /* unsigned long long integer value */
    double dval;                                            /* double value */
    long double ldval;                                      /* long double value */
    void *pval;                                             /* pointer value */
    char fmtbuf[FMTBUFSIZE];                                /* buffer to hold the format string */
    int fmtbufindex = 0;                                    /* index for the format buffer */
    int dynamic_width_flag = 0, dynamic_precision_flag = 0; /* flag to indicate if dynamic width, precision is used */
    int dynamic_width = 0, dynamic_precision = 0;           /* dynamic field width, dynamic precision */
    char lmod[MAXLENMODBUF];                                /* length modifier buffer */
    int lmodindex = 0;                                      /* index for the length modifier buffer */

    va_start(ap, fmt); /* make ap point to the first unnamed argument arg */
    for (p = fmt; *p; p++)
    {
        if (*p != '%')
        {
            putchar(*p);
            continue;
        }

        fmtbufindex = 0;              /* reset the format buffer index */
        lmodindex = 0;                /* reset the length modifier index */
        lmod[0] = '\0';               /* reset the length modifier buffer */
        dynamic_width_flag = 0;       /* reset dynamic field width flag*/
        dynamic_precision_flag = 0;   /* reset dynamic precision flag*/
        fmtbuf[fmtbufindex++] = *p++; /* copy the '%' character to the format buffer */

        /* 1. (Optional) handle flags */
        while (fmtbufindex < FMTBUFSIZE - 1 && *p && strchr(PRINTFFLAGS, *p) != NULL)
            fmtbuf[fmtbufindex++] = *p++;

        /* 2. (Optional) handle field width */
        while (fmtbufindex < FMTBUFSIZE - 1 && *p && strchr(PRINTFWP, *p) != NULL)
        {
            if (*p == '*')
            {
                dynamic_width_flag = 1;
                dynamic_width = va_arg(ap, int);
            }
            fmtbuf[fmtbufindex++] = *p++;
        }

        /* 3. (Optional) handle precision */
        if (*p == PRINTFWPSEP)
        {
            fmtbuf[fmtbufindex++] = *p++; /* copy the '.' character */
            while (fmtbufindex < FMTBUFSIZE - 1 && *p && strchr(PRINTFWP, *p) != NULL)
            {
                if (*p == '*')
                {
                    dynamic_precision_flag = 1;
                    dynamic_precision = va_arg(ap, int);
                }
                fmtbuf[fmtbufindex++] = *p++;
            }
        }

        /* 4. (Optional) handle length modifiers */
        while (fmtbufindex < FMTBUFSIZE - 1 && lmodindex < MAXLENMODBUF - 1 && *p && strchr(PRINTFLENMODIFIERS, *p) != NULL)
        {
            lmod[lmodindex++] = *p;
            fmtbuf[fmtbufindex++] = *p++;
        }
        lmod[lmodindex] = '\0'; /* null-terminate the length modifier buffer */

        /* 5. (Required) handle conversion specifier */
        if (fmtbufindex < FMTBUFSIZE - 1 && *p && strchr(PRINTFCONVSPECIFIERS, *p) != NULL)
            fmtbuf[fmtbufindex++] = *p++;
        fmtbuf[fmtbufindex] = '\0'; /* null-terminate the format string */

        if (fmtbufindex > 1)
            p = p - 1; /* step back to the conversion specifier */

        if (*p == '\0')
            break;

        switch (*p)
        {
        case 'd':
        case 'i':
            if (strcmp(lmod, "l") == 0)
            {
                lval = va_arg(ap, long);
                if (dynamic_width_flag && dynamic_precision_flag)
                    printf(fmtbuf, dynamic_width, dynamic_precision, lval);
                else if (dynamic_width_flag)
                    printf(fmtbuf, dynamic_width, lval);
                else if (dynamic_precision_flag)
                    printf(fmtbuf, dynamic_precision, lval);
                else
                    printf(fmtbuf, lval);
            }
            else if (strcmp(lmod, "ll") == 0)
            {
                llval = va_arg(ap, long long);
                if (dynamic_width_flag && dynamic_precision_flag)
                    printf(fmtbuf, dynamic_width, dynamic_precision, llval);
                else if (dynamic_width_flag)
                    printf(fmtbuf, dynamic_width, llval);
                else if (dynamic_precision_flag)
                    printf(fmtbuf, dynamic_precision, llval);
                else
                    printf(fmtbuf, llval);
            }
            else
            {
                ival = va_arg(ap, int);
                if (dynamic_width_flag && dynamic_precision_flag)
                    printf(fmtbuf, dynamic_width, dynamic_precision, ival);
                else if (dynamic_width_flag)
                    printf(fmtbuf, dynamic_width, ival);
                else if (dynamic_precision_flag)
                    printf(fmtbuf, dynamic_precision, ival);
                else
                    printf(fmtbuf, ival);
            }
            break;
        case 'c':
            ival = va_arg(ap, int);
            if (dynamic_width_flag && dynamic_precision_flag)
                printf(fmtbuf, dynamic_width, dynamic_precision, ival);
            else if (dynamic_width_flag)
                printf(fmtbuf, dynamic_width, ival);
            else if (dynamic_precision_flag)
                printf(fmtbuf, dynamic_precision, ival);
            else
                printf(fmtbuf, ival);
            break;
        case 'o':
        case 'u':
        case 'x':
        case 'X':
            if (strcmp(lmod, "l") == 0)
            {
                ulval = va_arg(ap, unsigned long);
                if (dynamic_width_flag && dynamic_precision_flag)
                    printf(fmtbuf, dynamic_width, dynamic_precision, ulval);
                else if (dynamic_width_flag)
                    printf(fmtbuf, dynamic_width, ulval);
                else if (dynamic_precision_flag)
                    printf(fmtbuf, dynamic_precision, ulval);
                else
                    printf(fmtbuf, ulval);
            }
            else if (strcmp(lmod, "ll") == 0)
            {
                ullval = va_arg(ap, unsigned long long);
                if (dynamic_width_flag && dynamic_precision_flag)
                    printf(fmtbuf, dynamic_width, dynamic_precision, ullval);
                else if (dynamic_width_flag)
                    printf(fmtbuf, dynamic_width, ullval);
                else if (dynamic_precision_flag)
                    printf(fmtbuf, dynamic_precision, ullval);
                else
                    printf(fmtbuf, ullval);
            }
            else
            {
                uval = va_arg(ap, unsigned int);
                if (dynamic_width_flag && dynamic_precision_flag)
                    printf(fmtbuf, dynamic_width, dynamic_precision, uval);
                else if (dynamic_width_flag)
                    printf(fmtbuf, dynamic_width, uval);
                else if (dynamic_precision_flag)
                    printf(fmtbuf, dynamic_precision, uval);
                else
                    printf(fmtbuf, uval);
            }
            break;
        case 'f':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            if (strcmp(lmod, "L") == 0)
            {
                ldval = va_arg(ap, long double);
                if (dynamic_width_flag && dynamic_precision_flag)
                    printf(fmtbuf, dynamic_width, dynamic_precision, ldval);
                else if (dynamic_width_flag)
                    printf(fmtbuf, dynamic_width, ldval);
                else if (dynamic_precision_flag)
                    printf(fmtbuf, dynamic_precision, ldval);
                else
                    printf(fmtbuf, ldval);
            }
            else
            {
                dval = va_arg(ap, double);
                if (dynamic_width_flag && dynamic_precision_flag)
                    printf(fmtbuf, dynamic_width, dynamic_precision, dval);
                else if (dynamic_width_flag)
                    printf(fmtbuf, dynamic_width, dval);
                else if (dynamic_precision_flag)
                    printf(fmtbuf, dynamic_precision, dval);
                else
                    printf(fmtbuf, dval);
            }
            break;
        case 'p':
            pval = va_arg(ap, void *);
            if (dynamic_width_flag && dynamic_precision_flag)
                printf(fmtbuf, dynamic_width, dynamic_precision, pval);
            else if (dynamic_width_flag)
                printf(fmtbuf, dynamic_width, pval);
            else if (dynamic_precision_flag)
                printf(fmtbuf, dynamic_precision, pval);
            else
                printf(fmtbuf, pval);
            break;
        case 's':
            sval = va_arg(ap, char *);
            if (dynamic_width_flag && dynamic_precision_flag)
                printf(fmtbuf, dynamic_width, dynamic_precision, sval);
            else if (dynamic_width_flag)
                printf(fmtbuf, dynamic_width, sval);
            else if (dynamic_precision_flag)
                printf(fmtbuf, dynamic_precision, sval);
            else
                printf(fmtbuf, sval);
            break;
        default:
            putchar(*p);
            break;
        }
    }
    va_end(ap); /* clean up when done */
}