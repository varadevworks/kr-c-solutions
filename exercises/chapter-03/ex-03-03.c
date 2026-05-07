#include <stdio.h>
#include <ctype.h>
#define LENGTH 1000

void expand(char s1[], char s2[]);
int expandrange(char s[], int i, char start, char end);

int main(void)
{
    char s1[] = "-These are test patterns: a-z, a-s-y-s s-0-9 -a-z | z-a $-";
    char s2[LENGTH];

    expand(s1, s2);
    printf("INPUT\t\t: %s\n", s1);
    printf("EXPANDED\t: %s\n", s2);

    return 0;
}

void expand(char s1[], char s2[])
{
    int i, o;
    for (i = 0, o = 0; o < (LENGTH - 1) && s1[i] != '\0'; i++)
    {
        if (i > 1 && s1[i - 2] != '-' && s1[i - 1] == '-' && s1[i] != '-')
            if (s1[i - 2] <= s1[i] && ((islower(s1[i - 2]) && islower(s1[i])) || (isupper(s1[i - 2]) && isupper(s1[i])) || (isdigit(s1[i - 2]) && isdigit(s1[i]))))
            {
                o = expandrange(s2, (o - 2), s1[i - 2], s1[i]);
                continue;
            }
        s2[o++] = s1[i];
    }
    s2[o] = '\0';
}

int expandrange(char s[], int i, char start, char end)
{
    char ch;
    for (ch = start; i < (LENGTH - 1) && ch <= end; ch++, i++)
        s[i] = ch;

    return i;
}