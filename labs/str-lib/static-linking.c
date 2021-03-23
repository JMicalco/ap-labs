#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc<4)
    {
        printf("You are missing the second string to apply the %s functionality\n",argv[1]);  
    } else {
        if (strcmp(argv[1],"-add")==0)
        {
            mystradd(argv[2],argv[3]);
        } else if (strcmp(argv[1],"-find")==0)
        {
            if (mystrfind(argv[2],argv[3])!=0)
            {
                printf("['%s'] string was found at [%i] position\n", argv[3], mystrfind(argv[2],argv[3]));
            }
               
        }
        
        
    }
    

    return 0;
}
