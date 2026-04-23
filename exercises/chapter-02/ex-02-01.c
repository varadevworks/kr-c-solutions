#include <limits.h>
#include <float.h>
#include <stdio.h>

int main(void)
{
    printf("signed char : %d - %d\n", CHAR_MIN, CHAR_MAX);
    printf("unsigned char : %d - %d\n", 0, UCHAR_MAX);
    printf("signed short : %d - %d\n", SHRT_MIN, SHRT_MAX);
    printf("unsigned short : %d - %d\n", 0, USHRT_MAX);
    printf("signed int : %d - %d\n", INT_MIN, INT_MAX);
    printf("unsigned int : %d - %d\n", 0, UINT_MAX);
    printf("signed long : %ld - %ld", LONG_MIN, LONG_MAX);
    printf("unsigned long : %ld - %ld", 0, ULONG_MAX);
    return 0;
}