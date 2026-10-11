#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    int c;

    if (argc < 1 || !argv[0])
    {
        fprintf(stderr, "Usage: program name should be [upper|lower]\n");
        return 1;
    }

    char *pgm_name = strrchr(argv[0], '/');
    pgm_name = pgm_name ? pgm_name + 1 : argv[0];

    int (*convert_func)(int);

    if (strcmp(pgm_name, "upper") == 0)
        convert_func = toupper;
    else if (strcmp(pgm_name, "lower") == 0)
        convert_func = tolower;
    else
    {
        fprintf(stderr, "Usage: program name should be [upper|lower] but provided %s\n", pgm_name);
        return 1;
    }

    while ((c = getchar()) != EOF)
        putchar(convert_func(c));

    return 0;
}