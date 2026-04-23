#include <stdio.h>
#define MAXLINE 1000

int get_line(char line[], int limit);

int main(void)
{
    int len;
    char line[MAXLINE];

    while ((len = get_line(line, MAXLINE)) > 0)
    {
        if (len > 80)
            printf("length: %d | line: %s", len, line);
    }
    return 0;
}

int get_line(char line[], int limit)
{
    int len, stored_len;
    int character;

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