#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void detab(int argc, char *argv[]);
void entab(int argc, char *argv[]);
void usage(void);
int isnumeric(char *s);

int main(int argc, char *argv[])
{
    argc--; /* decrement argument count for program name */

    /* retrieve program name */
    if (argc > 0)
    {
        argv++; /* get program name */
        // printf("info: provided program name: %s\n", *argvP);
        argc--; /* decrement argument count */
    }
    else
    {
        printf("error: no arguments are provided\n");
        usage();
        return -1;
    }

    if (strcmp(*argv, "detab") == 0)
        detab(argc, ++argv);
    else if (strcmp(*argv, "entab") == 0)
        entab(argc, ++argv);
    else
    {
        printf("error: invalid program name '%s'. expected 'detab' or 'entab'.\n", *argv);
        return -1;
    }

    return 0;
}

void usage(void)
{
    printf("usage: <invoker> <program> -m +n\n");
    printf("parameters:\n");
    printf("\tprogram\t\t: (required) valid values 'entab' or 'detab'\n");
    printf("\t-m\t: (optional) start column position. eg: -3\n");
    printf("\t+n\t: (optional) tab stop every n columns. eg: +4\n");
    printf("sample command: ex-05-11 entab -3 4\n");
}

int isnumeric(char *s)
{
    int c;

    if (s == NULL || *s == '\0')
        return 0;

    while ((c = *s++) != '\0')
        if (!isdigit(c) && c != '-' && c != '+') /* allow negative and positive signs */
            return 0;

    return 1;
}

void detab(int argc, char *argv[])
{
    int colp = 0;     /* current column position */
    int ntabp = 0;    /* next tab stop column position */
    int c;            /* current character */
    int stpos = 0;    /* start column position */
    int tabstep = 8;  /* tab stop*/
    int dtabstep = 8; /* default tab stop*/

    if (argc > 0) /* validate and retrieve start column position argument */
    {
        if (isnumeric(*argv))
        {
            stpos = atoi(*argv);
            if (stpos >= 0)
            {
                printf("error: start column position argument should start with '-'. but provided : %s", *argv);
                return;
            }
            stpos = (stpos < 0) ? -stpos : stpos;
            argc--;
            argv++;
        }
        else
        {
            printf("error: non numeric argument provided for column start position: %s", *argv);
            return;
        }
    }

    if (argc > 0) /* validate and retrieve tab stop width argument */
    {
        if (isnumeric(*argv))
        {
            tabstep = atoi(*argv);
            if (tabstep <= 0)
            {
                printf("error: tab stop columns width should be positive. but provided : %s", *argv);
                return;
            }
            argc--;
            argv++;
        }
        else
        {
            printf("error: non numeric argument provided for tab width column position: %s", *argv);
            return;
        }
    }

    while ((c = getchar()) != EOF)
    {
        if (colp >= ntabp) /* reached or exceeded tab stops */
        {
            if (colp < stpos)                   /* before start column position */
                if ((ntabp + dtabstep) < stpos) /* next tab stop is still before start column position */
                    ntabp = ntabp + dtabstep;   /* tab stop every <dtabstep> columns */
                else
                    ntabp = stpos; /* set next tab stop to start column position */
            else
                ntabp = ntabp + tabstep; /* tab stop every <tabstep> columns */
        }

        if (c == '\t') /* tab character : print spaces until next tab stop */
        {
            while (colp < ntabp)
            {
                putchar(' ');
                colp++;
            }
        }
        else if (c == '\n') /* newline character : reset column position and tab stops */
        {
            colp = 0;
            ntabp = 0;
            putchar(c);
        }
        else /* any other character : print it and advance column position */
        {
            putchar(c);
            colp++;
        }
    }
}

void entab(int argc, char *argv[])
{
    int c;
    int colp = 0;  /* current column position */
    int ntabp = 0; /* next tab stop column position */
    int nspaces = 0;
    int stpos = 0;    /* start column position */
    int tabstep = 8;  /* tab stop*/
    int dtabstep = 8; /* default tab stop*/

    if (argc > 0) /* validate and retrieve start column position argument */
    {
        if (isnumeric(*argv))
        {
            stpos = atoi(*argv);
            if (stpos >= 0)
            {
                printf("error: start column position argument should start with '-'. but provided : %s", *argv);
                return;
            }
            stpos = (stpos < 0) ? -stpos : stpos;
            argc--;
            argv++;
        }
        else
        {
            printf("error: non numeric argument provided for column start position: %s", *argv);
            return;
        }
    }

    if (argc > 0)
    {
        if (isnumeric(*argv))
        {
            tabstep = atoi(*argv);
            if (tabstep <= 0)
            {
                printf("error: tab stop columns width should be positive. but provided : %s", *argv);
                return;
            }
            argc--;
            argv++;
        }
        else
        {
            printf("error: non numeric argument provided for tab width column position: %s", *argv);
            return;
        }
    }

    while ((c = getchar()) != EOF)
    {
        if (colp >= ntabp) /* reached or exceeded tab stops */
        {
            if (nspaces > 0) /* flush pending spaces as a tab */
            {
                putchar('\t');
                nspaces = 0;
            }

            if (colp < stpos)                   /* before start column position */
                if ((ntabp + dtabstep) < stpos) /* next tab stop is still before start column position */
                    ntabp = ntabp + dtabstep;   /* tab stop every <dtabstep> columns */
                else
                    ntabp = stpos; /* set next tab stop to start column position */
            else
                ntabp = ntabp + tabstep; /* tab stop every <tabstep> columns */
        }

        if (c == ' ') /* space character: increment pending spaces and column position */
        {
            nspaces++;
            colp++;
        }
        else if (c == '\t') /* tab character: set column position to next tab stop */
            colp = ntabp;
        else if (c == '\n') /* newline character : reset column position and tab stops */
        {
            colp = 0;
            ntabp = 0;
            nspaces = 0;
        }
        else /* any other character : advance column position */
            colp++;

        while (c != ' ' && nspaces > 0) /* flush pending spaces as it is not aligned with tab stop */
        {
            putchar(' ');
            nspaces--;
        }

        if (c != ' ') /* print the current character if it is not a space */
            putchar(c);
    }

    while (nspaces > 0) /* flush any remaining pending spaces at EOF */
    {
        putchar(' ');
        nspaces--;
    }
}
