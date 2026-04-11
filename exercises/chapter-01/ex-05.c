#include <stdio.h>

#define LOWER_BOUND 0
#define UPPER_BOUND 300
#define STEP 20

int main(void)
{
    int fahrenheit;

    for (fahrenheit = UPPER_BOUND; fahrenheit >= LOWER_BOUND; fahrenheit = fahrenheit - STEP)
        printf("%3d %6.1f\n", fahrenheit, (5.0 / 9.0) * (fahrenheit - 32));

    return 0;
}