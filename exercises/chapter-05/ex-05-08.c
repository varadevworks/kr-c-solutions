#include <stdio.h>

static char daytab[2][13] = {{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
                             {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

int day_of_year(int year, int month, int day);
void month_day(int year, int yearday, int *pmonth, int *pday);

int main(void)
{
    /* test: day_of_year */
    int doy_year = 2000;
    int doy_month = 3;
    int doy_day = 2;
    int doy = day_of_year(doy_year, doy_month, doy_day);
    printf("input => year: %d, month: %d, day: %d | output => day of year: %d\n",
           doy_year, doy_month, doy_day, doy);

    /* test: month_day*/
    int md_year = 2000;
    int md_yearday = 63;
    int month;
    int day;

    month_day(md_year, md_yearday, &month, &day);
    printf("input => year: %d, yearday: %d | output => month: %d, day: %d\n",
           md_year, md_yearday, month, day);
}