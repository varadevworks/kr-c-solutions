#include <stdio.h>

void detab(int tab_width);
void print_blanks(int space_count);
int calculate_next_tab_stop(int column, int tab_width);

int main(void)
{
    int tab_width = 8;
    detab(tab_width);
}

void detab(int tab_width)
{
    int ch;
    int column = 0;
    int next_tab_stop = 0;
    int spaces_till_tab_stop = 0;
    int pending_spaces = 0;

    while ((ch = getchar()) != EOF)
    {
        if (ch == ' ')
        {
            next_tab_stop = calculate_next_tab_stop(column, tab_width);
            spaces_till_tab_stop = next_tab_stop - column;
            ++pending_spaces;
            if (spaces_till_tab_stop == 1)
            {
                putchar('\t');
                pending_spaces = 0; // Reset pending spaces after printing a tab
            }
            ++column;
            continue;
        }
        // Flush pending spaces before printing the character
        if (pending_spaces != 0)
        {
            print_blanks(pending_spaces);
            pending_spaces = 0;
        }
        // Tabs advance to the next tab stop; other characters advance by one column
        if (ch == '\t')
            column = calculate_next_tab_stop(column, tab_width);
        else
            ++column;
        // Character: Newline : Reset column to 0
        if (ch == '\n')
            column = 0;
        putchar(ch);
    }
    // Flush any pending spaces at the end of the input not followed by a newline
    if (pending_spaces != 0)
        print_blanks(pending_spaces);
}

void print_blanks(int space_count)
{
    while (space_count > 0)
    {
        putchar(' ');
        --space_count;
    }
}

int calculate_next_tab_stop(int column, int tab_width)
{
    return ((column / tab_width) + 1) * tab_width;
}