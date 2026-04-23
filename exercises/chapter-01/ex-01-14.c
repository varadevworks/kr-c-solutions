#include <stdio.h>
#define MAXCHARACTER 128

int main(void)
{
    int character;
    int character_counts[MAXCHARACTER + 1];

    for (int i = 0; i <= MAXCHARACTER; i++)
        character_counts[i] = 0;

    while ((character = getchar()) != EOF)
        /* Use the last slot to group any non-ASCII input as outliers. */
        if (character <= MAXCHARACTER)
            ++character_counts[character];
        else
            ++character_counts[MAXCHARACTER];

    // print histogram
    printf("        Number of character occurance\n");
    printf("     ---------------------\n");
    int histogram_counter = 0;
    for (int i = 0; i <= MAXCHARACTER; i++)
    {
        histogram_counter = character_counts[i];
        if (histogram_counter > 0)
        {
            if (i == MAXCHARACTER)
                printf(" >%3d |", MAXCHARACTER);
            else
                printf("%3d |", i);
            while (histogram_counter > 0)
            {
                printf("*");
                --histogram_counter;
            }
            putchar('\n');
        }
    }
}
