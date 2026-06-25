#include <stdio.h>

static char daytab[2][13] = {{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
                             {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

int day_of_year(int year, int month, int day);
void month_day(int year, int yearday, int *pmonth, int *pday);
int total_days_in_year(int leap);

int main(void)
{
    /* test: day_of_year */
    int doy_year = 2000;
    int doy_month = 2;
    int doy_day = 29;
    int doy = day_of_year(doy_year, doy_month, doy_day);
    printf("input => year: %d, month: %d, day: %d | output => day of year: %d\n",
           doy_year, doy_month, doy_day, doy);

    /* test: month_day*/
    int md_year = 2001;
    int md_yearday = 367;
    int month;
    int day;

    month_day(md_year, md_yearday, &month, &day);
    printf("input => year: %d, yearday: %d | output => month: %d, day: %d\n",
           md_year, md_yearday, month, day);
}

int day_of_year(int year, int month, int day)
{
    int leap = ((year % 4) == 0 && (year % 400 == 0 || year % 100 != 0)) ? 1 : 0;
    char *daytabp = daytab[leap];

    if (year < 1)
    {
        printf("invalid year provided: %d\n", year);
        return -1;
    }

    if (month < 1 || month > 12)
    {
        printf("invalid month provided: %d\n", month);
        return -1;
    }

    if (day < 1 || day > *(daytabp + month))
    {
        printf("invalid day provided: %d, days should be in between %d - %d"
               " for month: %d\n",
               day, 1, daytab[leap][month], month);
        return -1;
    }

    daytabp++;
    while (--month > 0)
        day += *daytabp++;

    return day;
}

void month_day(int year, int yearday, int *pmonth, int *pday)
{
    if (year < 1)
    {
        printf("invalid year provided: %d\n", year);
        return;
    }

    if (yearday < 1)
    {
        printf("invalid yearday provided: %d\n", yearday);
        return;
    }

    int leap = ((year % 4) == 0 && (year % 400 == 0 || year % 100 != 0)) ? 1 : 0;
    int tdays = total_days_in_year(leap);

    if (yearday > tdays)
    {
        printf("invalid yearday provided: %d, yearday should be in range of 1 - %d\n", yearday, tdays);
        return;
    }

    *pmonth = 0;
    *pday = 0;

    char *enddaytabp = daytab[leap];
    char *startdaytabp = enddaytabp;

    enddaytabp++;
    while (yearday > *enddaytabp)
        yearday -= *enddaytabp++;

    *pday = yearday;
    *pmonth = (char)(enddaytabp - startdaytabp);
}

int total_days_in_year(int leap)
{
    int days = 0;
    char *endpdaytab = daytab[leap];
    char *startpdaytab = endpdaytab;

    endpdaytab++;
    while ((char)(endpdaytab - startpdaytab) < 13)
        days += *endpdaytab++;
    return days;
}