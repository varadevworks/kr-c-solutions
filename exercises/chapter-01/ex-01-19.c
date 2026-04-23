#include <stdio.h>
#define MAXLINE 1000 // maximum input line size

int get_line(char line[], int limit);
void reverse(char line[]);

int main(void)
{
    int len;
    char line[MAXLINE];
    // read lines until EOF and reverse them
    while ((len = get_line(line, MAXLINE)) > 0)
        reverse(line);

    return 0;
}

int get_line(char line[], int limit)
{
    int character;
    int i;
    // read characters until limit, EOF or newline is reached, storing them in the line array
    for (i = 0; i < limit - 1 && (character = getchar()) != EOF && character != '\n'; i++)
        line[i] = character;

    // if newline character is encountered and within limit then add it to the line array and increment the index
    if (character == '\n')
        line[i++] = character;

    // null-terminate the line array
    line[i] = '\0';
    return i;
}

void reverse(char line[])
{
    int len;
    int has_newline = 0;

    // calculate the length of the line
    for (len = 0; line[len] != '\0'; len++)
        ;

    // check if the last character is a newline and adjust the length and flag accordingly
    if (len > 0 && line[len - 1] == '\n')
    {
        len = len - 2;
        has_newline = 1;
    }

    // print the characters in reverse order
    while (len >= 0)
    {
        putchar(line[len]);
        len--;
    }

    // if the original line ended with a newline, print a newline at the end of the reversed line
    if (has_newline)
        putchar('\n');
}
