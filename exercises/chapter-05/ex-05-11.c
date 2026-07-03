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
    printf("usage: <invoker> <program> [<tab_stops>]\n");
    printf("parameters:\n");
    printf("\tprogram\t\t: (required) valid values 'entab' or 'detab'\n");
    printf("\ttab_stops\t: (optional) tab stop column positions\n");
    printf("sample command: ex-05-11 entab 4\n");
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

void detab(int argc, char *argv[])
{
    int colp = 0;        /* current column position */
    int ntabp = 0;       /* next tab stop column position */
    int prevntabp = -1;  /* previous tab stop column position */
    int c;               /* current character */
    int argco = argc;    /* remaining argument count */
    char **argvo = argv; /* remaining argument vector */

    while ((c = getchar()) != EOF)
    {
        if (colp >= ntabp)
        { /* current column position exceeds next tab stop */
            if (argco > 0)
            {
                prevntabp = ntabp; /* store previous tab stop */
                if (isnumeric(*argvo))
                {
                    ntabp = atoi(*argvo++);               /* set next tab stop based on argument */
                    if (ntabp <= 0 || ntabp <= prevntabp) /* tab stop should be positive and greater than previous */
                    {
                        printf("error: invalid tab stop value provided : %d\n", ntabp);
                        return;
                    }
                    argco--;
                }
                else
                {
                    printf("error: not a valid integer - %s", *argvo);
                    return;
                }
            }
            else
                ntabp = ntabp + 8; /* default tab stop every 8 columns */
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
            argco = argc;
            argvo = argv;
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
    int colp = 0;       /* current column position */
    int ntabp = 0;      /* next tab stop column position */
    int prevntabp = -1; /* previous tab stop column position */
    int argco = argc;
    char **argvo = argv;
    int nspaces = 0;

    while ((c = getchar()) != EOF)
    {
        if (colp >= ntabp) /* reached or exceeded tab stopsch */
        {
            if (nspaces > 0) /* flush pending spaces as a tab */
            {
                putchar('\t');
                nspaces = 0;
            }

            if (argco > 0) /* check if there are more arguments */
            {
                prevntabp = ntabp; /* store previous tab stop */
                if (isnumeric(*argvo))
                {
                    ntabp = atoi(*argvo++);               /* set next tab stop based on argument */
                    if (ntabp <= 0 || ntabp <= prevntabp) /* tab stop should be positive and greater than previous */
                    {
                        printf("error: invalid tab stop value provided : %d\n", ntabp);
                        return;
                    }
                    argco--;
                }
                else
                {
                    printf("error: not a valid integer - %s", *argvo);
                    return;
                }
            }
            else
                ntabp = ntabp + 8; /* default tab stop every 8 columns */
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
            argco = argc;
            argvo = argv;
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
