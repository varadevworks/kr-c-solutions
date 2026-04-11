#include <stdio.h>

int main(void)
{
    int character;
    int number_of_newlines = 0;
    int number_of_tabs = 0;
    int number_of_blanks = 0;

    while ((character = getchar()) != EOF)
    {
        if (character == '\n')
            ++number_of_newlines;
        if (character == '\t')
            ++number_of_tabs;
        if (character == ' ')
            ++number_of_blanks;
    }

    printf("blanks: %d, tabs: %d, newlines: %d", number_of_blanks, number_of_tabs, number_of_newlines);
}