#include <stdio.h>

int lower(int ch);

int main(void)
{
    int i;
    char input[] = "THIS is TEST String";

    for (i = 0; input[i] != '\0'; i++)
        putchar(lower(input[i]));
    putchar('\n');
    return 0;
}

int lower(int ch)
{
    return (ch >= 'A' && ch <= 'Z') ? 'a' + (ch - 'A') : ch;
}
