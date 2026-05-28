#include <stdio.h>
#include <string.h>

void reverse(char s[]);

int main(void)
{
    char s[] = "reverse this string!";
    printf("input string: %s\n", s);

    reverse(s);
    printf("reverse string: %s\n", s);

    return 0;
}

void recur_swap(char s[], int start, int end);

void reverse(char s[])
{
    int l = strlen(s);

    if (l > 0)
        recur_swap(s, 0, l - 1);
}

void recur_swap(char s[], int start, int end)
{
    char temp;

    if (start < end)
    {
        temp = s[start];
        s[start] = s[end];
        s[end] = temp;
        recur_swap(s, start + 1, end - 1);
    }
}