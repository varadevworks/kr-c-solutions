#include <stdio.h>
#define LENGTH 100

char *strcat(char *s, const char *t);

int main(void)
{
    char s[LENGTH] = "The only way to learn ";
    char t[] = "a new programming language is by writing programs in it.";

    printf("input\n\ts: %s\n\tt: %s\n", s, t);
    strcat(s, t);
    printf("output\n\ts: %s\n", s);

    return 0;
}

char *strcat(char *s, const char *t)
{
    char *r = s;

    while (*s++)
        ;
    s--;
    while ((*s++ = *t++))
        ;
    return r;
}