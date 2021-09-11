// PP and declerations:
    #include <sys/wait.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <dirent.h> 

    #define PATHLEN 300 
    #define INPUTLEN 350
    #define ARGNUM_MAX 10
    #define DELIM " \n\r"

    void get_args(char* inbuf, char **args, int *argc);
    int execute_program(int argc, char **args);
    int execute_regular_program(int argc, char **args);
    void temp(char **args);

// inner functions:

    int cd_func(int argc, char **args)   {
        // sanity:
        if(!args[1]) perror("cd_func0");

        if(chdir(args[1]))
            perror("cd_func1");
    }

    int ls_func(int argc, char **args)    {
        char path[PATHLEN];
        DIR *d;
        struct dirent *dir;
        getcwd(path,PATHLEN);
        printf("args[1]: %s\n",args[1]);
        printf("args[0]: %s\n",args[0]);
        if(d = opendir(".")) {
            while((dir = readdir(d)) && (dir->d_name[0] != '\\')) {
                if(!(argc == 2 && strcmp(args[1],"-a") == 0))
                    if((dir->d_name[0] == '.'))
                        continue;
                printf("%s  ", dir->d_name);
            }
            printf("\n");
            closedir(d);
        }
    }

    int exit_func(int argc, char **args)   {
        return 1;
    }

char *inner_funcs_names[] = {
    "cd",
    "ls",
    "q"
};


int (*inner_funcs[]) (int, char**) = {
    cd_func,
    ls_func,
    exit_func
};


int execute_program(int argc, char **args) {
    int i, pid;
    int inner_funcs_N = sizeof(inner_funcs)/sizeof(inner_funcs[0]);
    printf("args[1]: %s\n",args[1]);
    printf("args[0]: %s\n",args[0]);
    for(i=0; i < inner_funcs_N; i++)   {
        if(strcmp(args[0], inner_funcs_names[i]) == 0)  {
            printf("args[1]: %s\n",args[1]);
            printf("args[0]: %s\n",args[0]);
            return (*inner_funcs[i])(argc,args);
        }
    }
    
    pid = fork();
    if(pid == 0)    {
        if(execvp(args[0],args) != 0)
            perror("execute_program");
        exit(1);
    }

    waitpid(pid,0,WUNTRACED);
    return 0;
}


int main() {
    int pid, argc, toExit = 0;
    char *inbuf = malloc(INPUTLEN);
    char **args = malloc(ARGNUM_MAX*sizeof(char*));
    do {
        get_args(inbuf,args,&argc);
        toExit = execute_program(argc, args);
    } while(!toExit);
    free(inbuf);
    free(args);
    return 0;
}


void get_args(char *inbuf, char **args, int *argc) {
    
    int i;
    char path[PATHLEN];
    // clean
    getcwd(path,PATHLEN);
    printf("%s > ",path);
    fgets(inbuf, INPUTLEN, stdin);
    if(INPUTLEN < strlen(inbuf))
        perror("input string too long\n");

    for(i=0; i<ARGNUM_MAX-1; i++)   {
        if(i == 0)  {
            args[i] = strtok(inbuf, DELIM);
        }
        else {
            args[i] = strtok(0, DELIM);
        }
        if(args[i] == 0) break;
    }
    *argc = i;
    args[i] = 0;
}