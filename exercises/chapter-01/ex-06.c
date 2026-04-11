#include <stdio.h>

int main(void)
{
    int character;
    // ctrl + d to send EOF (End of File) signal in terminal
    while ((character = getchar()) != EOF)
        printf("character[%d] => Result[%d]\n", character, (character != EOF));

    printf("character[%d] => Result[%d]\n", character, (character != EOF));
    return 0;
}