#include <stdio.h>
#include <ctype.h>

#define MAXLINE 30
#define BUFSIZE (MAXLINE + 1)

void print_line(int *line, int b, int l);
int getch(void);
void ungetch(int c);

int main(void)
{
    int c, i = 0, b = -1; /* c: current character, i: line buffer index, b: recent line break index */
    int line[MAXLINE];

    while ((c = getch()) != EOF)
    {
        if (i < MAXLINE - 1)
        {
            if (c == ' ' || c == '\n') /* store recent line break index */
                b = i;
            line[i++] = c; /* push the character to line buffer */
            if (c != '\n') /* continue reading until a newline is encountered */
                continue;
        }
        else
            ungetch(c);

        print_line(line, b, i - 1);
        i = 0;  /* reset line buffer index */
        b = -1; /* reset recent line break index */
    }

    if (i > 0) /* print any remaining characters in the buffer */
        print_line(line, -1, i - 1);

    return 0;
}

void print_line(int *line, int b, int l)
{
    int *start = line;
    int i = 0;

    b = (b == -1) ? l : b; /* if no recent line break, set b to the last index */

    while (l > b) /* push characters beyond the recent line break back to the input buffer */
        ungetch(line[l--]);

    while ((line - start) < b + 1) /* print characters up to and including the recent line break index*/
    {
        if ((*line == '\n' || isprint(*line)) && (i < MAXLINE)) /* print character if printable and within limit */
        {
            putchar(*line);
            i++;
        }
        else /* handle non-printable characters */
        {
            if (i <= MAXLINE - 4) /* check for extra 4 characters to print the hex representation */
            {
                printf("0x%02x", *line);
                i += 4;
            }
            else
            {
                putchar('\n');
                i = 0;
                continue;
            }
        }

        line++;
    }

    if (start[b] != '\n') /* ensure the line ends with a newline */
        putchar('\n');
}

int buf[BUFSIZE]; /* buffer for ungetch*/
int bufp = 0;     /* next free position in buffer */

int getch(void) /* get a (possibly pushed back) character*/
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /* push character back on input */
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}