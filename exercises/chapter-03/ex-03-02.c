#include <stdio.h>
enum
{
    LENGTH = 1000
};

enum
{
    OUT,
    SLASH
};

void escape(char s[], char t[]);
void revert_escape(char s[], char t[]);

int main(void)
{
    char et[] = "This is \t string with newline \n. End of string!!";
    char rt[] = "This is \\t string with newline \\n. End \tof\n string!!";
    char s[LENGTH];

    escape(s, et);
    printf("input string: %s \n escaped string: %s\n", et, s);

    revert_escape(s, rt);
    printf("input string: %s \n revert escaped string: %s\n", rt, s);

    return 0;
}

void escape(char s[], char t[])
{
    int i, o;
    char ch;

    for (i = 0, o = 0; (ch = t[i]) != '\0'; i++, o++)
    {
        switch (ch)
        {
        case '\t':
            s[o++] = '\\';
            s[o] = 't';
            break;
        case '\n':
            s[o++] = '\\';
            s[o] = 'n';
            break;
        default:
            s[o] = ch;
            break;
        }
    }
    s[o] = '\0';
}

void revert_escape(char s[], char t[])
{
    char ch;
    int i, o; // i - input, o - output
    int st = OUT;

    for (i = 0, o = 0; (ch = t[i]) != '\0'; i++)
    {
        if (st == OUT && ch == '\\')
        {
            st = SLASH;
            continue;
        }

        if (st == SLASH)
        {
            st = OUT;
            if (ch == 't')
                s[o++] = '\t';
            else if (ch == 'n')
                s[o++] = '\n';
            else
            {
                s[o++] = '\\';
                s[o++] = ch;
            }
            continue;
        }

        s[o++] = ch;
    }

    if (st == SLASH)
        s[o++] = '\\';

    s[o] = '\0';
}
