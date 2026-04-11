#include <stdio.h>

int main(void)
{
    int lower_bound = 0;
    int upper_bound = 300;
    float fahrenheit, celsius;

    fahrenheit = lower_bound;

    printf("%s %s\n", "Fahrenheit", "Celsius");

    while (fahrenheit <= upper_bound)
    {
        celsius = (5.0 / 9.0) * (fahrenheit - 32);
        printf("%3.0f %6.1f\n", fahrenheit, celsius);
        fahrenheit = fahrenheit + 20;
    }
}