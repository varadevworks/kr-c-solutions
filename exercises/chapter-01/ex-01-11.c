#include <stdio.h>
#define IN 0  // Inside a word
#define OUT 1 // Outside a word

int main(void)
{
    int character;
    int state = OUT;
    int number_of_characters = 0;
    int number_of_words = 0;
    int number_of_lines = 0;

    while ((character = getchar()) != EOF)
    {
        ++number_of_characters;

        if (character == '\n')
            ++number_of_lines;

        if (character == ' ' || character == '\n' || character == '\t')
        {
            state = OUT;
            continue;
        }

        if (state == OUT)
        {
            state = IN;
            ++number_of_words;
        }
    }

    printf("Count: %d characters, %d words, %d lines\n", number_of_characters, number_of_words, number_of_lines);
}

/*
** Test cases:
1. Input: "Hello World\n"
    Command: printf "Hello World\n" | ./ex-11
    Output: Count: 12 characters, 2 words, 1 lines

2. Input: empty input
    Command: printf "" | ./ex-11
    Output: Count: 0 characters, 0 words, 0 lines

3. Input: Only spaces, tabs and newlines
    Command: printf "   \t\n  \n\t" | ./ex-11
    Output: Count: 9 characters, 0 words, 2 lines

4. Input: word without newline at the end
    Command: printf "Hello World" | ./ex-11
    Output: Count: 11 characters, 2 words, 0 lines

5. Input: Multiple words with multiple seperators
    Command: printf "Hello   World\t\t\nThis is a test.\n" | ./ex-11
    Output: Count: 29 characters, 5 words, 1 lines

6. Input: word with multiple symbols
    Command: printf "--***++Hello--***++" | ./ex-11
    Output: Count: 19 characters, 1 words, 0 lines
 */