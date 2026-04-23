#include <stdio.h>

float convert_fahreheit_to_celsius(float farenheit);

int main(void)
{
    float fahrenheit, celsius;
    float lower, upper, step;
    lower = 0;
    upper = 300;
    step = 20;

    fahrenheit = lower;
    while (fahrenheit <= upper)
    {
        celsius = convert_fahreheit_to_celsius(fahrenheit);
        printf("%3.0f %6.1f\n", fahrenheit, celsius);
        fahrenheit = fahrenheit + step;
    }

    return 0;
}

float convert_fahreheit_to_celsius(float farenheit)
{
    float celsius;
    celsius = (5.0 / 9.0) * (farenheit - 32.0);
    return celsius;
}