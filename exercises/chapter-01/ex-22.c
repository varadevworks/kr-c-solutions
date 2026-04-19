#include <stdio.h>
#define INBLANK 1
#define OUTBLANK 0

void fold_lines(int max_line_size, int tab_width);
int expand_tab(char buffer[], int buffer_position, int tab_width);
int find_fold_position(char buffer[], int length, int line_size);
void print_line(char buffer[], int fold_position);
int shift_buffer(char buffer[], int start, int end, int max_line_size);

int main(void)
{
    int max_line_size = 80;
    int tab_width = 8;
    fold_lines(max_line_size, tab_width);
    return 0;
}

// Read input, expand tabs, and fold long lines as needed.
void fold_lines(int max_line_size, int tab_width)
{
    int ch;                         // Current character from input.
    int fold_position = 0;          // where we fold the line: last blank before the line limit or a hard fold if no blanks.
    int buffer_position = 0;        // current position in the buffer, may go past the line limit to detect the fold point and expand tabs.
    char buffer[max_line_size + 8]; // buffer to hold the current line, plus extra space for tab expansion.

    while ((ch = getchar()) != EOF)
    {

        // Tabs are expanded before we check folding.
        if (ch == '\t')
            buffer_position = expand_tab(buffer, buffer_position, tab_width);

        // A newline flushes the current buffer as-is.
        if (ch == '\n')
        {
            buffer[buffer_position] = ch;
            print_line(buffer, buffer_position);
            buffer_position = 0;
            continue;
        }

        // Add the character to the buffer if it's not a tab (tabs are already expanded).
        if (ch != '\t')
            buffer[buffer_position] = ch;

        // Once we hit the limit, print one folded chunk and shift the rest to the front of the buffer for the next pass.
        if (buffer_position >= max_line_size)
        {
            // Fold at the nearest break point we can find.
            fold_position = find_fold_position(buffer, buffer_position, max_line_size);

            // Print the folded line and add a newline if needed.
            print_line(buffer, fold_position);

            // Keep the leftover text for the next pass.
            buffer_position = shift_buffer(buffer, fold_position + 1, buffer_position, max_line_size);
        }

        ++buffer_position;
    }

    // Print any remaining text in the buffer.
    if (buffer_position > 0)
        print_line(buffer, buffer_position - 1);
}

// Replace a tab with spaces up to the next tab stop.
int expand_tab(char buffer[], int buffer_position, int tab_width)
{
    int next_tab_stop = (((buffer_position / tab_width) + 1) * tab_width);

    while (buffer_position < next_tab_stop)
        buffer[buffer_position++] = ' ';

    return buffer_position - 1;
}

// Walk backward to find the last non-blank character before the fold size, or a newline  or hard fold if no blanks.
int find_fold_position(char buffer[], int end_position, int fold_size)
{
    int state = OUTBLANK;
    char ch;
    int i;
    for (i = end_position; i >= 0; i--)
    {
        ch = buffer[i];
        // Track runs of blanks so we can fold on a word boundary.
        if (ch == ' ')
            state = INBLANK;
        else
        {
            if (i < fold_size)
                if ((ch != ' ' && state == INBLANK) || ch == '\n')
                    return i;

            state = OUTBLANK;
        }
    }

    return (fold_size - 1);
}

// Print the buffered text and add a newline if needed.
void print_line(char buffer[], int fold_position)
{
    for (int i = 0; i <= fold_position; i++)
        putchar(buffer[i]);

    if (buffer[fold_position] != '\n')
        putchar('\n');
}

// Move leftover characters to the front after a fold. Returns the last position in the buffer after the shift, which is the new length of the buffer for the next pass.
int shift_buffer(char buffer[], int start, int end, int max_line_size)
{
    int position = 0;
    char ch;

    // Drop leading blanks here only before the fold limit so the next line starts at the next word.
    while (end > start)
    {
        ch = buffer[start];
        if ((ch == ' ') && start < max_line_size)
            start++;
        else
            break;
    }

    while ((end - start) >= position)
    {
        buffer[position] = buffer[start + position];
        position++;
    }

    return position - 1;
}
