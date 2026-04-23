#include <stdio.h>

void detab(int n);
void print_blanks(int n);

int main(void)
{
    int n = 8; // number of columns for tab stops
    detab(n);
    return 0;
}

void detab(int n)
{
    int character;
    int column_position = 0;
    int next_tab_stop = 0;
    int number_of_spaces = 0;

    if (n <= 0)
    {
        n = 8; // default to 8 columns if n is not positive
    }

    while ((character = getchar()) != EOF)
    {
        if (character == '\t')
        {
            next_tab_stop = ((column_position / n) + 1) * n;
            number_of_spaces = next_tab_stop - column_position;
            print_blanks(number_of_spaces);
            column_position += number_of_spaces;
            continue;
        }

        if (character == '\n')
        {
            column_position = 0;
            putchar(character);
            continue;
        }

        putchar(character);
        ++column_position;
    }
}

void print_blanks(int n)
{
    while (n > 0)
    {
        putchar(' ');
        --n;
    }
}