#include <stdio.h>

#define OUT 0
#define IN_CHAR 1
#define IN_STRING 2
#define IN_SLASH 3
#define IN_LINE_COMMENT 5
#define IN_BLOCK_COMMENT 6

void remove_comments(void);

int main(void)
{
    remove_comments();
    return 0;
}

void remove_comments(void)
{
    int ch;
    int prev = -1;
    int state = OUT;
    int escape_count = 0;
    while ((ch = getchar()) != EOF)
    {
        // STATE: OUT
        if (state == OUT)
        {
            if (ch == '/')
                state = IN_SLASH;
            else if (ch == '\'')
                state = IN_CHAR;
            else if (ch == '"')
                state = IN_STRING;
        }
        // STATE: IN_SLASH
        else if (state == IN_SLASH)
        {
            if (ch == '/')
                state = IN_LINE_COMMENT;
            else if (ch == '*')
                state = IN_BLOCK_COMMENT;
            else
            {
                if (ch == '\'')
                    state = IN_CHAR;
                else if (ch == '"')
                    state = IN_STRING;
                else
                    state = OUT;
                putchar('/');
            }
        }
        // STATE: IN_CHAR
        else if (state == IN_CHAR)
        {
            if (ch == '\\')
                ++escape_count;
            else if (ch == '\'')
            {
                if ((escape_count % 2) == 0)
                    state = OUT;
                escape_count = 0;
            }
            else
                escape_count = 0;
        }
        // STATE: IN_STRING
        else if (state == IN_STRING)
        {
            if (ch == '\\')
                ++escape_count;
            else if (ch == '"')
            {
                if ((escape_count % 2) == 0)
                    state = OUT;
                escape_count = 0;
            }
            else
                escape_count = 0;
        }
        // STATE: IN_LINE_COMMENT
        else if (state == IN_LINE_COMMENT)
        {
            if (ch == '\n')
                state = OUT;
        }
        // STATE: IN_BLOCK_COMMENT
        else if (state == IN_BLOCK_COMMENT)
        {
            if (ch == '/' && prev == '*')
            {
                state = OUT;
                continue;
            }
        }

        prev = ch;

        if (state != IN_LINE_COMMENT && state != IN_SLASH &&
            state != IN_BLOCK_COMMENT)
            putchar(ch);
    }

    if (state == IN_SLASH)
        putchar('/');
}