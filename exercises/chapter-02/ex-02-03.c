#include <stdio.h>
#include <ctype.h>

int htoi(char s[]);
int int_pow(int base, int times);
int hex_digit_to_int(char c);

int main(void)
{
    char s[] = "0xABC45";
    int value;

    value = htoi(s);
    if (value != -1)
        printf("Hex value for %s : %d\n", s, value);

    return 0;
}

int htoi(char s[])
{
    int digit_start = 0;
    int length = 0;
    int result = 0;
    char ch;
    int digit_value = 0;
    enum
    {
        PREFIX_NONE,
        PREFIX_ZERO,
        PREFIX_HEX
    } prefix_state = PREFIX_NONE;

    while ((ch = s[length]) != '\0')
    {
        if (prefix_state == PREFIX_NONE && ch == '0')
            prefix_state = PREFIX_ZERO;
        else if (prefix_state == PREFIX_ZERO && (ch == 'x' || ch == 'X'))
        {
            prefix_state = PREFIX_HEX;
            digit_start = length + 1;
        }
        ++length;
    }

    for (int i = digit_start; i < length; i++)
    {
        digit_value = hex_digit_to_int(s[i]);
        if (digit_value == -1)
            return -1;

        result += (digit_value * int_pow(16, length - i - 1));
    }
    return result;
}

int int_pow(int base, int times)
{
    int value = 1;

    while (times > 0)
    {
        value = base * value;
        times--;
    }
    return value;
}

int hex_digit_to_int(char c)
{
    if (isxdigit(c))
    {
        if (isdigit(c))
            return c - '0';
        else if (islower(c))
            return c - 'a' + 10;
        return c - 'A' + 10;
    }
    return -1;
}