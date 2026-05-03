#include <stdio.h>
enum
{
    LENGTH = 100000
};

void fillarray(int v[], int n);
int binsearch(int x, int v[], int n);

int main(void)
{
    int v[LENGTH];
    int x = 99999;
    int result;

    // Fill array
    fillarray(v, LENGTH);

    // find x index
    result = binsearch(x, v, LENGTH);
    printf("Element %d is at index: %d\n", x, result);

    return 0;
}

void fillarray(int v[], int n)
{
    int i;

    for (i = 0; i < n; i++)
        v[i] = i;
}

int binsearch(int x, int v[], int n)
{
    int low, high, mid;

    low = 0;
    high = n - 1;
    while (low < high)
    {
        mid = (low + high) / 2;

        if (x <= v[mid])
            high = mid;
        else
            low = mid + 1;
    }

    return (n > 0 && v[low] == x) ? low : -1;
}
