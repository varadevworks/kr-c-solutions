#include <limits.h>
#include <float.h>
#include <stdio.h>

signed long compute_signed_long_min(int num_bits);
signed long compute_signed_long_max(int num_bits);
unsigned long compute_unsigned_long_max(int num_bits);

int main(void)
{

    // Int types limits
    printf("signed char range (from standard header)\t: %d to %d\n", SCHAR_MIN, SCHAR_MAX);
    printf("signed char range (by direct computation)\t: %ld to %ld\n", compute_signed_long_min(sizeof(char) * CHAR_BIT), compute_signed_long_max(sizeof(char) * CHAR_BIT));

    printf("unsigned char range (from standard header)\t: %d to %u\n", 0, UCHAR_MAX);
    printf("unsigned char range (by direct computation)\t: %d to %lu\n", 0, compute_unsigned_long_max(sizeof(char) * CHAR_BIT));

    printf("signed short range (from standard header)\t: %d to %d\n", SHRT_MIN, SHRT_MAX);
    printf("signed short range (by direct computation)\t: %ld to %ld\n", compute_signed_long_min(sizeof(short) * CHAR_BIT), compute_signed_long_max(sizeof(short) * CHAR_BIT));

    printf("unsigned short range (from standard header)\t: %d to %u\n", 0, USHRT_MAX);
    printf("unsigned short range (by direct computation)\t: %d to %lu\n", 0, compute_unsigned_long_max(sizeof(short) * CHAR_BIT));

    printf("signed int range (from standard header) \t: %d to %d\n", INT_MIN, INT_MAX);
    printf("signed int range (by direct computation)\t: %ld to %ld\n", compute_signed_long_min(sizeof(int) * CHAR_BIT), compute_signed_long_max(sizeof(int) * CHAR_BIT));

    printf("unsigned int range (from standard header)\t: %d to %u\n", 0, UINT_MAX);
    printf("unsigned int range (by direct computation)\t: %d to %lu\n", 0, compute_unsigned_long_max(sizeof(int) * CHAR_BIT));

    printf("signed long range (from standard header)\t: %ld to %ld\n", LONG_MIN, LONG_MAX);
    printf("signed long range (by direct computation)\t: %ld to %ld\n", compute_signed_long_min(sizeof(long) * CHAR_BIT), compute_signed_long_max(sizeof(long) * CHAR_BIT));

    printf("unsigned long range (from standard header)\t: %d to %lu\n", 0, ULONG_MAX);
    printf("unsigned long range (by direct computation)\t: %d to %lu\n", 0, compute_unsigned_long_max(sizeof(long) * CHAR_BIT));

    // floating point types limits
    printf("float\t: %e to %e\n", FLT_MIN, FLT_MAX);
    printf("double\t: %e to %e\n", DBL_MIN, DBL_MAX);

    return 0;
}

signed long compute_signed_long_min(int num_bits)
{
    signed long value = -1;

    for (int i = 1; i < num_bits; i++)
        value = 2 * value;

    return value;
}

signed long compute_signed_long_max(int num_bits)
{
    unsigned long value = 1;

    for (int i = 1; i < num_bits; i++)
        value = 2 * value;

    return --value;
}

unsigned long compute_unsigned_long_max(int num_bits)
{
    unsigned long value = 1;

    for (int i = 1; i <= num_bits; i++)
        value = 2 * value;

    return --value;
}
