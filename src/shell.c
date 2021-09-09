// PP and declerations:
    #include <sys/wait.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>

    #define PATHLEN 300 
    #define INPUTLEN 100
    #define ARGC 10
    #define DELIM " \n\r"

    void get_args(char **args);
    int execute_program(char **args);
    int execute_regular_program(char **args);
    void temp(char **args);

// inner functions:
    int cd_func(char **args)   {
        // sanity:
            if(!args) perror("cd_func0");
            if(!args[1]) perror("cd_func1");

        if(chdir(args[1]))
            perror("cd_func2");
    }

    int exit_func(char **args)   {
        return 1;
    }

char *inner_funcs_names[] = {
    "cd",
    "q"
};

int (*inner_funcs[]) (char**) = {
    cd_func,
    exit_func
};




int execute_program(char **args) {
    int i, pid;
    int inner_funcs_N = sizeof(inner_funcs)/sizeof(int);

    for(i=0; i < inner_funcs_N; i++)   {
        if(strcmp(args[0], inner_funcs_names[i]) == 0)  {
            return inner_funcs[i](args);
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
    int pid, toExit = 0;
    char *args[ARGC];
    do {
        get_args(args);
        toExit = execute_program(args);
    } while(!toExit);
    return 0;
}


void get_args(char **args) {
    
    int i;
    char str[INPUTLEN+5];
    char path[PATHLEN];

    // clean
    for(i=0; i<ARGC; i++)   {
        args[i] = 0;
    }
    getcwd(path,PATHLEN);
    printf("%s > ",path);
    fgets(str, INPUTLEN, stdin);
    if(INPUTLEN < strlen(str))
        perror("input string too long\n");

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