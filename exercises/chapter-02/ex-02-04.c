#include <stdio.h>

void squeeze(char s1[], char s2[]);

int main(void)
{
    char s1[] = "ABCDEF112234";
    char s2[] = "123111SSAA";
    printf("String 1: %s\nString 2: %s\n", s1, s2);
    squeeze(s1, s2);
    printf("Output: %s\n", s1);
    return 0;
}

void squeeze(char s1[], char s2[])
{
    int i = 0, j = 0, k = 0;
    int f = 0;

    while (s1[i] != '\0')
    {
        f = 0;
        for (j = 0; s2[j] != '\0'; j++)
        {
            if (s1[i] == s2[j])
            {
                f = 1;
                break;
            }
        }

        if (!f)
            s1[k++] = s1[i];

        ++i;
    }

    s1[k] = '\0';
}