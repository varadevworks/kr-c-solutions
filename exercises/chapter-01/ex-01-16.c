#include <stdio.h>
#define MAXLINE 1000

int get_line(char line[], int limit);
void copy(char to[], char from[]);

int main(void)
{
    int len;
    int max_len;
    char line[MAXLINE];
    char longest[MAXLINE];

    max_len = 0;

    while ((len = get_line(line, MAXLINE)) > 0)
    {
        if (len > max_len)
        {
            max_len = len;
            copy(longest, line);
        }
    }

    if (max_len > 0)
        printf("longest line: length : %d | line : %s", max_len, longest);
    return 0;
}

int get_line(char line[], int limit)
{
    int character;
    int len, stored_len;

    for (len = 0, stored_len = 0; (character = getchar()) != EOF && character != '\n'; ++len)
        if (stored_len < limit - 1)
        {
            line[stored_len] = character;
            ++stored_len;
        }

    if (character == '\n')
    {
        if (stored_len < limit - 1)
        {
            line[stored_len] = character;
            ++stored_len;
        }
        ++len;
    }

    line[stored_len] = '\0';

    return len;
}

void copy(char to[], char from[])
{
    int i = 0;
    while ((to[i] = from[i]) != '\0')
        i++;
}