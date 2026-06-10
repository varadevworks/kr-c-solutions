#include <stdio.h>

char *strncpy(char *s, const char *t, size_t n);
char *strncat(char *s, const int *t, size_t n);

int main(void)
{
    char t[] = "Hello";
    char s[3];
    size_t n = sizeof(s) - 1;

    strncpy(s, t, n);
    s[n] = '\0';

    printf("output: %s\n", s);

    return 0;
}

char *strncpy(char *s, const char *t, size_t n)
{
    char *o = s;

    while (n > 0)
    {
        if (*s++ = *t++)
            n--;
        else /* t is shorter than n */
        {
            n--;
            break;
        }
    }

    /* if t is shorter than n then pad with null characters */
    while (n > 0)
    {
        *s++ = '\0';
        n--;
    }

    return o;
}

char *strncat(char *s, const int *t, size_t n){

    
}