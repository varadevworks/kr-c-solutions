#include <stdio.h>
#define MAXLINE 1000

int get_line(char line[], int limit);
void trimming_blanks(char line[], int len);

int main(void)
{
    int len;
    char line[MAXLINE];
    while (1)
    {
        len = get_line(line, MAXLINE);
        if (len == -1)
            break;
        trimming_blanks(line, len);
        printf("%s", line);
    }
    return 0;
}

int get_line(char line[], int limit)
{
    int len;
    int character;

    // read characters until limit, EOF or newline is reached, storing them in the line array
    for (len = 0; len < limit - 1 && (character = getchar()) != EOF && character != '\n'; ++len)
        line[len] = character;

    // if EOF is encountered, return -1 to indicate end of input
    if (character == EOF)
        return -1;

    // if a newline character is encountered, add it to the line array and increment the length
    if (character == '\n')
        line[len++] = character;

    // null-terminate the line array
    line[len] = '\0';

    return len;
}

void trimming_blanks(char line[], int len)
{
    int has_newline = len > 0 && line[len - 1] == '\n';
    int i = has_newline ? len - 2 : len - 1;

    if (!has_newline)
        return;

    for (; i >= 0 && (line[i] == ' ' || line[i] == '\t'); --i)
        ;

    if (i < 0)
        line[0] = '\0';
    else
    {
        line[i + 1] = '\n';
        line[i + 2] = '\0';
    }
}