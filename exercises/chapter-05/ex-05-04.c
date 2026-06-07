#include <stdio.h>

int strend(char *s, char *t);

int main(void)
{
    char s[] = "The only way to learn";
    char t[] = "learn";
    int r = 0;

    printf("input\n\ts: %s\n\tt: %s\n", s, t);
    r = strend(s, t);
    if (r == 1)
        printf("String t occurs at the end of string s\n");
    else
        printf("String t does not occur at the end of string s\n");

    return 0;
}

int strend(char *s, char *t)
{
    int slen = 0;
    int tlen = 0;

    while (*s)
    {
        slen++;
        s++;
    }

    while (*t)
    {
        tlen++;
        t++;
    }

    if (slen > 0)
        s--;

    if (tlen > 0)
        t--;

    if (tlen > slen)
        return 0;

    while (tlen > 0)
    {
        if (*s != *t)
            return 0;

        tlen--;

        if (tlen > 0)
        {
            t--;
            s--;
        }
    }
    return 1;
}
