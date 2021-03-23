

int mystrlen(char *str){
    int i=0;
    while (*str)
    {
        str++;
        i++;
    }
    return i;
}

char *mystradd(char *origin, char *addition){
    char newString[mystrlen(origin)+mystrlen(addition)];
    int i=0;
    int size=mystrlen(origin)+mystrlen(addition);
    printf("Initial Length  : %i\n",mystrlen(origin));
    printf("New String      : ");
    while (i<size)
    {
        if (*origin)
        {
            newString[i]=*origin;
            printf("%c",newString[i]);
            origin++;
            i++;
        } else {
            break;
        }
    }
    while (i<size)
    {
        if (*addition)
        {
            newString[i]=*addition;
            printf("%c",newString[i]);
            addition++;
            i++;
        }
        else {
            break;
        }
    }
    printf("\n");
    printf("New Length      : %i\n",size);
}

int mystrfind(char *origin, char *substr){
    int sizeSub=mystrlen(substr);
    char *beginning= substr;
    int i=0, k=0;
    while(*origin){
        if(*origin==*substr){
            while(*substr){
                if(*substr==*origin){
                    i++;
                    substr++;
                    origin++;
                } else {
                    break;
                }
                if(i==sizeSub){
                    return k;
                }
            }
            k++;
            i=0;
            substr=beginning;
        } else {
            k++;
            origin++;
        }
    }
    return 0;
}
