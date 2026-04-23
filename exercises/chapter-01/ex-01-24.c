#include <stdio.h>

#define OUT 0
#define IN_COMMENT_SLASH 1
#define IN_COMMENT_STAR 2
#define IN_CHARACTER 3
#define IN_STRING 4
#define IN_SLASH 5

void check_syntax(void);

int main(void)
{
    check_syntax();
    return 0;
}

void check_syntax(void)
{
    int ch;
    int prev;
    int state = OUT;
    int escape_count = 0;
    char stack[1000];
    int stack_index = 0;
    char stack_top;

    while ((ch = getchar()) != EOF)
    {
        if (state == OUT)
        {
            if (ch == '/')
                state = IN_SLASH;
            else if (ch == '"')
                state = IN_STRING;
            else if (ch == '\'')
                state = IN_CHARACTER;
            else if (ch == '(' || ch == '[' || ch == '{')
                stack[stack_index++] = ch;
            else if (ch == ')' || ch == ']' || ch == '}')
            {
                if (stack_index == 0)
                {
                    printf("Unmatched closing : %c\n", ch);
                    return;
                }
                else
                {
                    stack_top = stack[stack_index - 1];
                    if ((ch == ')' && stack_top == '(') || (ch == ']' && stack_top == '[') || (ch == '}' && stack_top == '{'))
                        --stack_index;
                    else
                    {
                        printf("Unmatched closing : %c\n", ch);
                        return;
                    }
                }
            }
        }
        else if (state == IN_CHARACTER)
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
        else if (state == IN_SLASH)
        {
            if (ch == '/')
                state = IN_COMMENT_SLASH;
            else if (ch == '*')
                state = IN_COMMENT_STAR;
            else
            {
                state = OUT;
                if (ch == '\'')
                    state = IN_CHARACTER;
                else if (ch == '"')
                    state = IN_STRING;
                else if (ch == '(' || ch == '[' || ch == '{')
                    stack[stack_index++] = ch;
                else if (ch == ')' || ch == ']' || ch == '}')
                {
                    if (stack_index == 0)
                    {
                        printf("Unmatched closing : %c\n", ch);
                        return;
                    }
                    else
                    {
                        stack_top = stack[stack_index - 1];
                        if ((ch == ')' && stack_top == '(') || (ch == ']' && stack_top == '[') || (ch == '}' && stack_top == '{'))
                            --stack_index;
                        else
                        {
                            printf("Unmatched closing : %c\n", ch);
                            return;
                        }
                    }
                }
                else
                    state = OUT;
            }
        }
        else if (state == IN_COMMENT_SLASH)
        {
            if (ch == '\n')
                state = OUT;
        }
        else if (state == IN_COMMENT_STAR)
        {
            if (ch == '/' && prev == '*')
            {
                state = OUT;
                continue;
            }
        }

        prev = ch;
    }

    if (state == IN_COMMENT_STAR)
        printf("Unterminated block comment\n");
    else if (state == IN_STRING)
        printf("Unterminated string\n");
    else if (state == IN_CHARACTER)
        printf("Unterminated character constant\n");
    else if (state == IN_SLASH)
        printf("Trailing slash\n");
    else if (stack_index > 0)
        printf("Unmatched opening : %c\n", stack[stack_index - 1]);
}
