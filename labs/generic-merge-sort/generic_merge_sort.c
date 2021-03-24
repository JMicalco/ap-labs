#include <stdio.h>
#include <stdlib.h>

char *arrayStr[100][100];
int arrayInt[100];
char narrayStr[100][100];
int narrayInt[100];

void mergeSort(char *file, int i);
void merge(int bottom, int mid, int top);
void sort(int bottom, int top);
void mergeStr(int bottom, int mid, int top);
void sortStr(int bottom, int top);

int main(int argc, char** argv)
{
    if (argc<2)
    {
        printf("Missing parameters to run merge sort\n");
    } else if (argc>2 && strcmp(argv[1],"-n")==0)
    {
        mergeSort(argv[2],1);
    } else if (argc<3 && strcmp(argv[1],"-n")!=0)
    {
        mergeSort(argv[1],0);
    } else if (argc<3 && strcmp(argv[1],"-n")==0)
    {
        printf("Incomplete argumments to perform number merge sorting\n");
    }
    return 0;
}

void mergeSort(char *file, int i){
    
    FILE * fp;
    FILE * fw;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int k=0;
    char newFile[7+strlen(file)];
    //Define destination File
    strcat(newFile,"sorted_");
    strcat(newFile,file);

    printf("NewFile: %s\n",newFile);
    fp = fopen(file, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    fw = fopen(newFile, "w");    

    while ((read = getline(&line, &len, fp)) != -1) {
        char tmpArray[read];
        if (i==1)
        {
            arrayInt[k]=atoi(line);
            
        } else if (i==0)
        {
            strcpy(arrayStr[k],line);
        }
        k++;
        
    }

    //Merge sort
    if (i==1)
    { //Numbers
        sort(0,k);
        for (int i = 0; i < k; i++)
        {
            fprintf(fw,"%d\n", arrayInt[i]);
        }
        
    }//Strings
    if (i==0)
    {
        sortStr(0,k);
        for (int i = 0; i < k+1; i++)
        {
            fprintf(fw,"%s", arrayStr[i]);
        }   
    }

    printf("Results file can be found at ./%s\n",newFile);

    fclose(fp);
    fclose(fw);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}

void merge(int bottom, int mid, int top) {
   int left1, 
        left2, 
        i;
   for(left1 = bottom, left2 = mid + 1, i = bottom; left1 <= mid && left2 <= top; i++) {
      if(arrayInt[left1] <= arrayInt[left2])
         narrayInt[i] = arrayInt[left1++];
      else
         narrayInt[i] = arrayInt[left2++];
   }
   
   while(left1 <= mid)    
      narrayInt[i++] = arrayInt[left1++];

   while(left2 <= top)   
      narrayInt[i++] = arrayInt[left2++];

   for(i = bottom; i <= top; i++)
      arrayInt[i] = narrayInt[i];
}

void sort(int bottom, int top) {
   int mid;
   
   if(bottom < top) {
      mid = (bottom + top) / 2;
      sort(bottom, mid);
      sort(mid+1, top);
      merge(bottom, mid, top);
   } else { 
      return;
   }   
}


void mergeStr(int bottom, int mid, int top) {
    int left1= mid-bottom+1,
        left2= top-mid;

    char** left=malloc(sizeof(char *)*left1);
    char** right=malloc(sizeof(char *)*left2);
    int i;
    for(i=0; i< left1; i++)
    {
        left[i]=malloc(sizeof(arrayStr[bottom+i]));
        strcpy(left[i],arrayStr[bottom+i]);
    }
    for(i=0; i< left2; i++)
    {
        right[i]=malloc(sizeof(arrayStr[mid+i+1]));
        strcpy(right[i],arrayStr[mid+i+1]);
    }
    int j=0,
        k;
    i=0;
    k=bottom;
    while(i < left1 && j < left2)
    {
        if(strcmp(left[i],right[j])<0){
            strcpy(arrayStr[k++],left[i++]);
        } else strcpy(arrayStr[k++],right[j++]);
    }
    while(i < left1){
        strcpy(arrayStr[k++],left[i++]);
    }
    while(j < left2){
        strcpy(arrayStr[k++],right[j++]);
    }
}

void sortStr(int bottom, int top) {
   int mid;
   
   if(bottom < top) {
      mid = (bottom + top) / 2;
      sortStr(bottom, mid);
      sortStr(mid+1, top);
      mergeStr(bottom, mid, top);
   } else { 
      return;
   }   
}
