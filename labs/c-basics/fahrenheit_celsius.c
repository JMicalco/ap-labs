#include <stdio.h>
#include <stdlib.h> 

#define   LOWER  0       /* lower limit of table */
#define   UPPER  300     /* upper limit */
#define   STEP   20      /* step size */

/* print Fahrenheit-Celsius table */


int main(int argc, char  **argv)
{
    int fahr;
    if (argv[1]!=NULL && argv[2]==NULL)
    {
        fahr=atoi(argv[1]);
        printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));

    } else if (argv[1]!=NULL && argv[2]!=NULL && argv[3]==NULL)
    {
        for (fahr = atoi(argv[1]); fahr <= atoi(argv[2]); fahr = fahr + 10)
	        printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));

    } else if (argv[1]!=NULL && argv[2]!=NULL && argv[3]!=NULL){
        for (fahr = atoi(argv[1]); fahr <= atoi(argv[2]); fahr = fahr + atoi(argv[3]))
	        printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
    }
    return 0;
}
