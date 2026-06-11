#include <stdio.h>

char *strncpy(char *s, const char *t, size_t n);
char *strncat(char *s, const char *t, size_t n);
int strncmp(const char *s, const char *t, size_t n);

int main(void)
{
    /* test strncpy*/
    char t[] = "Hello";
    char s[3];
    size_t n = sizeof(s) - 1;

    strncpy(s, t, n);
    s[n] = '\0';

    printf("strncpy output: %s\n", s);

    /* test strncat*/
    char a[30] = "Hello";
    char b[] = "World!";

    strncat(a, b, 3);
    printf("strncat output: %s\n", a);

    /* test strncmp*/
    char c[] = "Hello";
    char d[] = "Helld!";
    int diff;

    diff = strncmp(c, d, 5);
    printf("strncmp output: %i\n", diff);

    return 0;
}

char *strncpy(char *s, const char *t, size_t n)
{
    char *o = s;

    while (n > 0)
    {
        if ((*s++ = *t++))
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

char *strncat(char *s, const char *t, size_t n)
{
    char *o = s;

    while (*s++)
        ;
    s--;

    while (n > 0 && (*s++ = *t++))
        n--;

    *s = '\0';

    return o;
}

int strncmp(const char *s, const char *t, size_t n)
{
    while (n > 0 && (*s++ - *t++) == 0 && *(s - 1) != '\0')
        n--;

    return (n != 0) ? (*(--s) - *(--t)) : 0;
}