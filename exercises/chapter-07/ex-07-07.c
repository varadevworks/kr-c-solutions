#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 1001

int find_pattern(char *filename, char *pattern);

int except = 0; /* option: except flag */
int number = 0; /* option: number flag */
int print = 0;  /* flag: print filename flag */

/* entry point
exit status:
  0 if pattern is found in at least one file
  1 if pattern is not found in any file
  2 if an error occurred
*/
int main(int argc, char *argv[])
{
    int c, r = 1, found = 0;
    char *p, *f;

    while (--argc > 0 && (*++argv)[0] == '-')
        while ((c = *++argv[0]))
            switch (c)
            {
            case 'x': /* except flag */
                except = 1;
                break;
            case 'n': /* number flag */
                number = 1;
                break;
            default:
                fprintf(stderr, "find: illegal option: %c\n", c);
                argc = 0; /* stop processing further arguments after illegal option */
                break;
            }

    if (argc < 1)
    {
        fprintf(stderr, "Usage: find [-x] [-n] pattern [filename1] [filename2] ...\n");
        exit(2);
    }

    p = *argv++; /* pattern */
    f = *argv;   /* first filename */

    if (argc > 2)
        print = 1;

    /* iterate over all filenames, if no filenames are given, read from stdin */
    do
    {
        found = find_pattern(f, p);
        if (found == 2)
            r = 2; /* set error status */
        else if (r != 2 && found == 0)
            r = 0; /* set found status if pattern is found in at least one file */
    } while ((f && (f = *++argv)));

    exit(r);
}

/* find_pattern: find given pattern in a file based on options set
 * returns:
 *   0 if pattern is found
 *   1 if pattern is not found in any file
 *   2 if an error occurred (e.g., file could not be opened)
 */
int find_pattern(char *filename, char *pattern)
{
    FILE *fp;           /* file pointer */
    char line[MAXLINE]; /* buffer to store each line of the file */
    long lineno = 1;    /* line number, default 1 */
    int r = 1;          /* r: return status. default 1 (pattern not found) */
    int l = 0;          /* l: length of the current line */
    int f = 0;          /* f: pattern found in current line */

    fp = (filename == NULL) ? stdin : fopen(filename, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: Could not open the file : %s\n", filename);
        return 2;
    }

    while ((fgets(line, sizeof(line), fp)) != NULL)
    {
        f = 0;            /* reset pattern found flag for the current line */
        l = strlen(line); /* get the length of the current line */
        if ((strstr(line, pattern) != NULL) != except)
        {
            if (number)
                printf("%ld: ", lineno);
            if (filename && print)
                printf("%s: %s", filename, line);
            else
                printf("%s", line);
            r = 0; /* set return status to 0 as pattern is found */
            f = 1; /* set pattern found flag for the current line */
        }

        if (l > 0 && line[l - 1] == '\n') /* increment line number if line ends with newline */
            ++lineno;
    }

    if (ferror(fp))
    {
        fprintf(stderr, "Error: Reading file : %s\n", filename);
        r = 2;
    }
    else
    {
        /*print newline if pattern is found and line doesnt end with newline */
        if (f > 0 && l > 0 && line[l - 1] != '\n')
            printf("\n");
    }

    if (filename != NULL)
        fclose(fp);

    return r;
}