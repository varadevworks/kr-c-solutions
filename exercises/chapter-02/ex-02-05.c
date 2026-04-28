#include <stdio.h>

int any(char s1[], char s2[]);

int main(void)
{
    char s1[] = "abc";
    char s2[] = "a";
    int value;
    value = any(s1, s2);
    printf("S1: %s | S2: %s => Result: %d\n", s1, s2, value);
    return 0;
}

int any(char s1[], char s2[])
{
    int i = 0, j = 0, f = 0;

    while (s1[i] != '\0')
    {
        j = 0;
        while (s2[j] != '\0')
        {
            if (s1[i] == s2[j++])
            {
                f = 1;
                break;
            }
        }

        if (f)
            return i;
        ++i;
    }
    return -1;
}