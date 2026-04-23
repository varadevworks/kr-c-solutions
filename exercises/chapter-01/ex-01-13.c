#include <stdio.h>
#define MAXWORDLENGTH 20
#define IN 1  // inside a word
#define OUT 0 // outside a word

int main(void)
{
    int character;
    /* Extra entry for word length > max word length */
    int word_lengths[MAXWORDLENGTH + 1];
    int state = OUT;
    int length = 0;

    for (int i = 0; i <= MAXWORDLENGTH; i++)
        word_lengths[i] = 0;

    while ((character = getchar()) != EOF)
    {
        if (character == ' ' || character == '\t' || character == '\n')
        {
            if (state == IN)
            {
                if (length <= MAXWORDLENGTH)
                    ++word_lengths[length - 1];
                else
                    ++word_lengths[MAXWORDLENGTH];
                length = 0;
            }
            state = OUT;
            continue;
        }

        if (state == OUT)
            state = IN;

        ++length;
    }

    if (state == IN)
    {
        if (length <= MAXWORDLENGTH)
            ++word_lengths[length - 1];
        else
            ++word_lengths[MAXWORDLENGTH];
        length = 0;
    }

    // print histogram
    printf("        Number of words\n");
    printf("     ---------------------\n");
    int histogram_counter = 0;
    for (int i = 0; i <= MAXWORDLENGTH; i++)
    {
        histogram_counter = word_lengths[i];
        if (i < MAXWORDLENGTH)
            printf("%3d |", i + 1);
        else
            printf(">%d |", MAXWORDLENGTH);
        while (histogram_counter > 0)
        {
            printf("*");
            --histogram_counter;
        }
        putchar('\n');
    }

    return 0;
}