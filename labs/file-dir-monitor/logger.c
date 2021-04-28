#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "logger.h"
#include <syslog.h>

#define RESET		0
#define BRIGHT 		1
#define DIM		2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

#define BLACK 		0
#define RED		1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	WHITE		7


int initLog;

void textcolor(int attr, int fg, int bg);

int initLogger(char *logType) {
    if (strcmp(logType, "stdout")==0 || strcmp(logType, "")==0)
    {
        initLog=0;
    } else if (strcmp(logType, "syslog")==0)
    {
        initLog=1;
    }    
    printf("Initializing Logger on: %s\n", logType);
    return 0;
}

void typeMessage(va_list va, const char *format, int prt){
    if (initLog==1)
    {
        openlog("logger", LOG_PID | LOG_CONS, LOG_SYSLOG);
		vsyslog(prt, format, va);
		closelog();
    } else 
    {
        vprintf(format, va);
    }
    
}


void textcolor(int attr, int fg, int bg)
{	char command[13];
	/* Command is the control command to the terminal */
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}

int infof(const char *format, ...){
    va_list va;
    va_start(va, format);
    textcolor(BRIGHT, GREEN, BLACK);	
    typeMessage(va, format, LOG_INFO);
    textcolor(RESET, WHITE, BLACK);	
    va_end(va);
    printf("\n");
    return 0;


}
int warnf(const char *format, ...){
    va_list va;
    va_start(va, format);
    textcolor(BRIGHT, YELLOW, BLACK);	
    typeMessage(va, format, LOG_WARNING);
    textcolor(RESET, WHITE, BLACK);	
    va_end(va);
    printf("\n");
    return 0;

}
int errorf(const char *format, ...){
    va_list va;
    va_start(va, format);
    textcolor(BRIGHT, RED, BLACK);	
    typeMessage(va, format, LOG_ERR);
    textcolor(RESET, WHITE, BLACK);	
    va_end(va);
    printf("\n");
    return 0;

}
int panicf(const char *format, ...){
    va_list va;
    va_start(va, format);
    textcolor(BRIGHT, CYAN, BLACK);	
    typeMessage(va, format, LOG_CRIT);
    textcolor(RESET, WHITE, BLACK);	
    va_end(va);
    printf("\n");
    return 0;
}