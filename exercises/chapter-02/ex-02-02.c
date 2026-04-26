#include <stdio.h>
#define MAXLINE 1000

int get_line(char line[], int limit);
void copy(char to[], char from[]);

int main(void)
{
    char line[MAXLINE];
    char longest[MAXLINE];
    int len = 0;
    int max = 0;

    while ((len = get_line(line, MAXLINE)) > 0)
    {
        if (len > max)
        {
            max = len;
            copy(longest, line);
        }
    }

    if (max > 0)
        printf("longest line: %s", longest);

    return 0;
}

int get_line(char line[], int limit)
{
    int ch = EOF;
    int i = 0;

    while (i < limit - 1)
    {
        if ((ch = getchar()) != '\n')
        {
            if (ch != EOF)
            {
                line[i] = ch;
                ++i;
            }
            else
                break;
        }
        else
            break;
    }

    if (ch == '\n')
    {
        line[i] = ch;
        ++i;
    }

    line[i] = '\0';

    return i;
}

void copy(char to[], char from[])
{
    int i = 0;
    while ((to[i] = from[i]) != '\0')
        i++;
}
