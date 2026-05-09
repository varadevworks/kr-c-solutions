#include <stdio.h>
#define MAXLINE 1000

int get_line(char s[], int lim);
int strrindex(char source[], char searchfor[]);

char pattern[] = "ould";

int main(void)
{
    char line[MAXLINE];

    while (get_line(line, MAXLINE) > 0)
        if (strrindex(line, pattern) >= 0)
            printf("%s", line);

    return 0;
}

int get_line(char line[], int lim)
{
    int ch, i;

    i = 0;
    while (--lim > 0 && (ch = getchar()) != EOF && ch != '\n')
        line[i++] = ch;

    if (ch == '\n')
        line[i++] = ch;
    line[i] = '\0';

    return i;
}

int strrindex(char source[], char searchfor[])
{
    int i, j, k, p;

    p = -1;
    for (i = 0; source[i] != '\0'; i++)
    {
        for (j = i, k = 0; searchfor[k] != '\0' && source[j] == searchfor[k]; j++, k++)
            ;
        if (k > 0 && searchfor[k] == '\0')
            p = i;
    }

    return p;
}