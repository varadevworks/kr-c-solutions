#include <stdio.h>
#include <ctype.h>
#define LENGTH 100

double atof(char s[]);

int main(void)
{
    char s[] = " \t-123.456e-2";
    double num;

    num = atof(s);
    printf("input: %s, num: %.8f", s, num);

    return 0;
}

double atof(char s[])
{
    int i, sign, esign, exponent;
    double val, power;

    esign = 0;
    // skip spaces
    for (i = 0; isspace(s[i]); i++)
        ;
    sign = (s[i] == '-') ? -1 : 1;

    if (s[i] == '-' || s[i] == '+')
        i++;
    // Integer part
    for (val = 0.0; isdigit(s[i]); i++)
        val = 10.0 * val + (s[i] - '0');

    if (s[i] == '.')
        i++;
    // fractional part
    for (power = 1.0; isdigit(s[i]); i++)
    {
        val = 10.0 * val + (s[i] - '0');
        power = power * 10.0;
    }

    // scientific notation
    if (s[i] == 'e' || s[i] == 'E')
    {
        i++;

        switch (s[i])
        {
        case '-':
            i++;
            esign = -1;
            break;
        case '+':
            i++;
        default:
            esign = 1;
            break;
        }

        // exponent
        for (exponent = 0; isdigit(s[i]); i++)
            exponent = 10 * exponent + (s[i] - '0');

        for (; exponent > 0; exponent--)
            power = (esign > 0) ? (power / 10.0) : (power * 10.0);
    }

    return (sign * val) / power;
}