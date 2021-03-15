#include <stdio.h>
#include <stdbool.h>
/* month_day function's prototype*/
//void month_day(int year, int yearday, int *pmonth, int *pday);

int main(int argc, char **argv) {
    if (argc<3)
    {
        printf("Too few arguments, try again\n");
        return 0;
    } else if (atoi(argv[2])>366)
    {
        printf("Your second argument exceeds the limit of days in a year, please try again\n");
        return 0;
    } else {
        int pmonth, pday;
         month_day(atoi(argv[1]), atoi(argv[2]),pmonth,pday);
    }
    return 0;
}

void month_day(int year, int yearday, int *pmonth, int *pday){
    int leap;
    char month[12][3]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    char daytab[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };
    int m,d;
    pmonth=&m;
    pday=&d;
    int temp;
    char newMonth[3];

    //Determine if the year is a leap-year
    leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
    for (int i = 0; i < sizeof(daytab[0]); i++)
    {
        temp+=daytab[leap][i];
        
        if (temp-yearday>=0)
        {
            *pday=yearday-(temp-daytab[leap][i]);
            *pmonth=i-1;
            break;
        }
    }
    printf("%c%c%c %i, %i \n",month[*pmonth][0],month[*pmonth][1],month[*pmonth][2],*pday,year);
}