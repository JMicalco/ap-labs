#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<fcntl.h> 
#include<errno.h> 
#define REPORT_FILE "packages_report.txt"

void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./pacman-analizer.o \n");
	return 1;
    } else if (argc>3)
    {
        analizeLog(argv[2], argv[4]);
    } else {
        printf("To few arguments\n");
    }
    

    

    return 0;
}

struct package
{
    char name[38];
    char installDate[28];
    char lastUpdate[28];
    int countUpdate;
    char removalDate[28];

};


void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);
    
    //Read File


    int f=0;
    int installed=0;
    int removed=0;
    int upgraded=0;
    int current=0;
    int scriplet=0;
    int alpm=0;
    int pacman=0;
    struct package arrPackage[600];
    int countArrPk=0;
    char oldest[50];
    char newest[50];
    int arrupdate[sizeof(arrPackage)/sizeof(arrPackage[0])];

    //int fd_to_read=open("pacman.txt", O_RDONLY);

    FILE * fp;
    FILE * fw;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(logFile, "r");
   
    if (fp == NULL){
        printf("File not found \n");
        exit(EXIT_SUCCESS);
    }
    fw = fopen(report, "w");
        
    /*if (fd_to_read ==-1)
    {
        printf("The file could not open");
    }*/

    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length %zu:\n", read);
        int wordCounter=0;
        int contChar=0;
        char newStr[read][read];
        for (int i = 0; i < read; i++)
        {
            if (line[i]==' ' ||line[i]=='\0')
            {
               newStr[wordCounter][contChar]='\0';
               wordCounter++;
               contChar=0;
            } else
            {
                newStr[wordCounter][contChar]=line[i];
                contChar++;
            }
        }
      
            //for para recorrer las palabras de la linea
            for (int i = 0; i < wordCounter+2; i++)
            {
                //printf("%s\n", newStr[i]);
                if (strcmp(newStr[i],"installed")==0 && i==3 || strcmp(newStr[i],"installed")==0 && i==2 )
                {   
                    
                    //Copiar el nombre del paquete
                    strcpy(arrPackage[countArrPk].name, "");
                    strcpy(arrPackage[countArrPk].name, newStr[i+1]);
                    strcpy(newest, newStr[i+1]);
                    arrPackage[countArrPk].countUpdate=0;
                    //condicional si la fecha esta separada o no
                    if (strlen(newStr[0])>11)
                    {   
                        removeChar(newStr[0],'[');
                        removeChar(newStr[0],']');
                        strcpy(arrPackage[countArrPk].installDate, newStr[0]);
                        
                    } else {
                        removeChar(newStr[0],'[');
                        removeChar(newStr[1],']');
                        strcpy(arrPackage[countArrPk].installDate, "");
                        strcat(arrPackage[countArrPk].installDate, newStr[0]);
                        strcat(arrPackage[countArrPk].installDate, " ");
                        strcat(arrPackage[countArrPk].installDate, newStr[1]);
                        
                        
                    }
                    installed++;
                    countArrPk++;
                    break;
                } else if (strcmp(newStr[i],"upgraded")==0 && i==3 || strcmp(newStr[i],"upgraded")==0 && i==2)
                {
                    strcpy(newest, newStr[i+1]);
                    //int size = sizeof arrPackage / sizeof arrPackage[0];
                    for (int j = 0; j < countArrPk; j++)
                    {
                        if (strcmp(newStr[i+1],arrPackage[j].name)==0)
                        {
                            if (strlen(newStr[0])>11)
                            {
                                removeChar(newStr[0],'[');
                                removeChar(newStr[0],']');
                                strcpy(arrPackage[j].lastUpdate, newStr[0]);
                                arrPackage[j].countUpdate++;
                                break;
                            } else {
                                char newUpdate[28];
                                removeChar(newStr[0],'[');
                                removeChar(newStr[1],']');
                                strcpy(newUpdate,"");
                                strcat(newUpdate, newStr[0]);
                                strcat(newUpdate, " ");
                                strcat(newUpdate, newStr[1]);
                                strcpy(arrPackage[j].lastUpdate, newUpdate);
                                strcpy(newUpdate,"");
                                //arrPackage[j].countUpdate=0;
                                arrPackage[j].countUpdate++;
                                break;
                            }    
                        }
                        
                    }
                    
                    upgraded++;
                    break;
                } else if (strcmp(newStr[i],"removed")==0 && i==3 || strcmp(newStr[i],"removed")==0 && i==2)
                {
                    
                    for (int j = 0; j < countArrPk; j++)
                    {
                        if (strcmp(newStr[i+1],arrPackage[j].name)==0)
                        {
                            if (strlen(newStr[0])>11)
                            {
                                removeChar(newStr[0],'[');
                                removeChar(newStr[0],']');
                                strcpy(arrPackage[j].removalDate, newStr[0]);
                                break;
                            } else {
                                char newUpdate[28];
                                strcpy(newUpdate,"");
                                removeChar(newStr[0],'[');
                                removeChar(newStr[1],']');
                                strcat(newUpdate, newStr[0]);
                                strcat(newUpdate, " ");
                                strcat(newUpdate, newStr[1]);
                                strcpy(arrPackage[j].removalDate, newUpdate);
                                strcpy(newUpdate,"");
                                //printf("I: %s\n",arrPackage[j].removalDate);
                                break;
                            }    
                        }
                        
                    }
                    removed++;
                    break;
                } else if(strcmp(newStr[i],"[ALPM-SCRIPTLET]")==0){
                    scriplet++;
                } else if (strcmp(newStr[i],"[ALPM]")==0)
                {
                    alpm++;
                } else if (strcmp(newStr[i],"[PACMAN]")==0)
                {
                    pacman++;
                }
                
            }        

    }

    //Verify how many packages did not have any updates
    char noUpgrades[3451];
    strcpy(noUpgrades,"");
    for (int i = 0; i < countArrPk; i++)
    {       
        if(*arrPackage[i].removalDate=='\0')
        {
            if (f==0)
            {
               strcpy(oldest,arrPackage[i].name);
               f=1;
            }
            
        }
            if (*arrPackage[i].lastUpdate=='\0')
            {
                strcat(noUpgrades,arrPackage[i].name);
                strcat(noUpgrades,", ");
                //size_t destination_size = sizeof (noUpgrades);
                
            }
            
    }

   fprintf(fw,"Pacman Packages Report\n");
    fprintf(fw,"---------------------\n");
    fprintf(fw,"- Installed packages    :%i\n", installed);
    fprintf(fw,"- Removed packages      :%i\n", removed);
    fprintf(fw,"- Upgraded packages     :%i\n", upgraded);
    fprintf(fw,"- Current packages      :%i\n", installed-removed);
    fprintf(fw,"---------------------\n");
    fprintf(fw,"General Stats\n");
    fprintf(fw,"---------------------\n");
    fprintf(fw,"- Oldest package                : %s\n",oldest);
    fprintf(fw,"- Newest package                : %s\n",newest);
    fprintf(fw,"- Package with no upgrades      : %s\n",noUpgrades);
    fprintf(fw,"- [ALPM-SCRIPTLET] type count   : %i\n", scriplet);
    fprintf(fw,"- [ALPM] count                  : %i\n", alpm);
    fprintf(fw,"- [PACMAN]count                 : %i\n", pacman);
    fprintf(fw,"---------------------\n");
    fprintf(fw,"List of packages\n");
    fprintf(fw,"---------------------\n");

    strcpy(noUpgrades,"");

   for (int i = 0; i < countArrPk; i++)
    {
        fprintf(fw,"- Package Name                          : %s\n",arrPackage[i].name);
        fprintf(fw,"    - Install date                      : %s\n",arrPackage[i].installDate);
        if (*arrPackage[i].lastUpdate=='\0')
        {
            fprintf(fw,"    - Last update date                  : - \n");
        } else {
            fprintf(fw,"    - Last update date                  : %s\n",arrPackage[i].lastUpdate);
        }
        fprintf(fw,"    - How many updates                  : %i\n",arrPackage[i].countUpdate);
        if (*arrPackage[i].removalDate=='\0')
        {
            fprintf(fw,"    - Removal date                      : - \n");
        } else {
            fprintf(fw,"    - Removal date                      : %s\n",arrPackage[i].removalDate);
        }
    }

     printf("Report is generated at: [%s]\n", report);
    //close(fd_to_read);
    
    fclose(fp);
    fclose(fw);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
        
    
}

void removeChar(char *str, char character) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != character) dst++;
    }
    *dst = '\0';
}

