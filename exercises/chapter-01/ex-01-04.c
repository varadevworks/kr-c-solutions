#include <stdio.h>

int main(void)
{
    int lower_bound = 0;
    int upper_bound = 300;
    float fahrenhiet, celsius;

    celsius = lower_bound;

    printf("%s %s\n", "celsius", "fahrenhiet");

    while (celsius <= upper_bound)
    {
        fahrenhiet = (celsius * 9.0 / 5.0) + 32.0;
        printf("%3.0f %6.1f\n", celsius, fahrenhiet);
        celsius = celsius + 20;
    }

    return 0;
}