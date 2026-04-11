#include <stdio.h>
#define OUT 0 // outside blank
#define IN 1  // inside blank

int main(void)
{
    int character;
    int state = OUT;

    while ((character = getchar()) != EOF)
    {
        if (character == ' ')
        {
            state = IN;
            continue;
        }
        else if (state == IN)
        {
            state = OUT;
            putchar(' ');
        }

        putchar(character);
    }

    if (state == IN)
        putchar(' ');

    return 0;
}