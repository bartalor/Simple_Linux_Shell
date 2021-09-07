#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define STRLEN 100
#define ARGC 10
#define DELIM " \n\r"

char *inner_functions[] = {
    "cd"
}

char 

void get_args(char **args);

int main() {
    int pid;
    char *args[ARGC];
    do {
        get_args(args);
        pid = fork();
        if(pid == 0)    {
           if(execvp(args[0],args) == -1) 
                printf("error!!!\n");
            exit(0);
        }
        wait(0);
    } while(strcmp("q",args[0]));
    return 0;
}



void get_args(char **args) {
    
    int i;
    char str[STRLEN+5];

    // clean
    for(i=0; i<ARGC; i++)   {
        args[i] = 0;
    }

    fgets(str, STRLEN, stdin);
    if(STRLEN < strlen(str))
        printf("bad bad input string too long\n");

    for(i=0; i<ARGC-1; i++)   {
        if(i == 0)  {
            args[i] = strtok(str, DELIM);
        }
        else {
            args[i] = strtok(0, DELIM);
        }
        if(args[i] == 0) break;
    }
    args[i] = 0;
}


