#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "main.h"

int main (int argc, char** argv) 
{
    // Load config files.

    // Run command loop.
    jsh_loop();

    // Shutdown/cleanup. 
    return EXIT_SUCCESS;
}

void jsh_loop(void)
{
    char* line;
    char** args;
    int status;
    do {
        printf("> ");
        line = jsh_read_line();
        args = jsh_split_line(line);
        status = jsh_execute(args);
        free(line);
        free(args);
    } while(status);
}

#define JSH_RL_BUFFSIZE 1024
char* jsh_read_line(void)
{
    int buffsize = JSH_RL_BUFFSIZE;
    int position = 0;
    char* buffer = malloc(sizeof(char) * JSH_RL_BUFFSIZE);
    // need to store character as int, not char, because EOF is int
    int c;

    if (!buffer) {
        fprintf(stderr, "Read line malloc allocation error.\n");
        exit(EXIT_FAILURE);
    }
    while(1) {
        // Read char from std input. 
        c = getchar(); 
        if (c == EOF || c == '\n') {
            // Replace EOF w/ null character. 
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        // Reallocate buffer if buffer exceeded. 
        if (position >= buffsize) {
            buffsize += JSH_RL_BUFFSIZE; 
            buffer = realloc(buffer, buffsize);
            if (!buffer) {
                fprintf(stderr, "Read line realloc allocation error.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char* jsh_read_line_2(void)
{
    char *line = NULL;
    size_t buffsize = 0;
    if (getline(&line, &buffsize, stdin) == -1) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);     // recieved EOF
        } else {
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }

    return line;
}

#define JSH_TOKEN_BUFFSIZE 64
#define JSH_TOKEN_DELIM " \t\r\n\a"
char** jsh_split_line(char* line)
{
    int buffsize = JSH_TOKEN_BUFFSIZE;
    int position = 0;
    char** tokens = malloc (buffsize * sizeof(char*));
    char* token;

    if (!tokens) {
        fprintf(stderr, "Tokens malloc allocation error.\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, JSH_TOKEN_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= buffsize) {
            buffsize += JSH_TOKEN_BUFFSIZE;
            tokens = realloc(tokens, buffsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "Tokens realloc allocation error.\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, JSH_TOKEN_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}

int jsh_launch(char** args)
{
    pid_t pid, wpid;
    int status;

    
    pid = fork();  
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("jsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("jsh");
    } else {
        // Parent Process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

/*
    Built-In Shell Commands
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
    printf("Use \"man\" comand for other programs.");
    return 1;
}

int jsh_exit(char** args)
{
    return 0;
}

int jsh_execute(char** args) 
{
    if (args[0] == NULL) {
        return 1;
    }

    for (int i=0; i<jsh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return jsh_launch(args);
}
