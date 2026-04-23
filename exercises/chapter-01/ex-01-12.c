#include <stdio.h>
#define IN 1  // inside a word
#define OUT 0 // outside a word

int main(void)
{
    int character;
    int state = OUT;

    while ((character = getchar()) != EOF)
    {
        if (character == ' ' || character == '\t' || character == '\n')
        {
            if (state == IN)
                putchar('\n');
            state = OUT;
            continue;
        }

        if (state == OUT)
            state = IN;
        putchar(character);
    }

    if (state == IN)
        putchar('\n');

    return 0;
}
