#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "func.h"
/*
    Built-In Shell Functions
*/


char* builtin_str[] = {
    "cd",
    "help", 
    "exit"
};

int (*builtin_func[]) (char**) = {
    &jsh_cd,
    &jsh_help,
    &jsh_exit
};

int jsh_num_builtins() 
{
    return sizeof(builtin_str) / sizeof(char *);
}

int jsh_cd(char** args) 
{
    if (args[1] == NULL) {
        fprintf(stderr, "jsh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("jsh");
        }
    }
    return 1;
}

int jsh_help(char** args) 
{
    printf("Josh Forden's JSH\n");
    printf("Enter shell program names and args.\n");
    printf("JSH has these programs built in:\n");
    for (int i=0; i<jsh_num_builtins(); i++) {
        printf("    %s\n", builtin_str[i]);
    }
    printf("Use \"man\" comand for other programs.\n");
    return 1;
}

int jsh_exit(char** args)
{
    return 0;
}