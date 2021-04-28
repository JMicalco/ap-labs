#include <stdio.h>
#include "logger.h"
#include <ftw.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>

#define BUFF_LEN (2048 * (sizeof(struct inotify_event)) + 256)
int show(const char *fpath, const struct stat *sb,int flag);
int notify, watchD;
struct inotify_event *ev;

int main(int argc, char **argv){
    // Place your magic here
    if (argc<2)
	{
		errorf("Unable to proceed because of too few arguments");
	} else {
        notify = inotify_init();
        if (notify<0)
        {
            errorf("There is an error with the notify initializer");
        }

        if (ftw(argv[1], show, 20) == -1)
        {
            errorf("Error with ftw");
        }

        int reading, cont;
        char bff[BUFF_LEN];

        for(;;){
            cont=0;
            reading = read(notify, bff, BUFF_LEN);
            if (reading<0)
            {
                errorf("error in read number");
            }
            while (cont<reading)
            {
                struct inotify_event *event = (struct inotify_event *) &bff[cont];
                //ev = (struct inotify_event *) &bff[cont];
                if (event -> len)
                {
                    if (event -> mask & IN_CREATE)
                    {
                        infof("[CREATE] %s", event -> name);
                    } else if (event -> mask & IN_MOVED_FROM)
                    {
                       infof("[Rename] %s", event -> name);
                    } else if (event -> mask & IN_MOVED_TO)
                    {
                        infof("[Moved To] %s", event -> name);
                    } else if (event -> mask & IN_DELETE)
                    {
                        infof("[Removal] %s", event -> name);
                    } else if (event -> mask & IN_MODIFY) {
                        infof("[Modify] %s", event -> name);
                    }
                    
                    cont+= (sizeof(struct inotify_event)) + event -> len;
                    
                }
                
            }
            
        }
        
    }
    return 0;
}

int show(const char *fpath, const struct stat *sb,int flag){
    if (flag == FTW_D)
    {
        watchD= inotify_add_watch(notify, fpath, IN_CREATE | IN_MODIFY | IN_DELETE | IN_MOVE);
        return 0;
    }
    
}



