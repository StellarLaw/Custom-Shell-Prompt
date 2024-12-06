#include "shell_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "io_redirection.h" 
#include "shell_util.h"
#include "tokenizer.h"
#include "background.h" 



void display_prompt() {
     
    char *user = getenv("USER");
    if (!user) user = "unknown";

   
    #ifndef HOST_NAME_MAX
    #define HOST_NAME_MAX 256
    #endif

    
    char hostname[HOST_NAME_MAX];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        perror("gethostname");
        strcpy(hostname, "unknown");
    }

    
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        strcpy(cwd, "unknown");
    }

    
    printf("%s@%s:%s> ", user, hostname, cwd);
    fflush(stdout);
}

void expand_env_vars(tokenlist *tokens) {
    for (int i = 0; i < tokens->size; i++) {
        if (tokens->items[i][0] == '$') { 
            char *env = getenv(tokens->items[i] + 1); 
            if (env) {
                free(tokens->items[i]); 
                tokens->items[i] = strdup(env); 
            } else {
                
                free(tokens->items[i]);
                tokens->items[i] = strdup("");
            }
        }
    }
}

void expand_tilde(tokenlist *tokens) {
    char *home = getenv("HOME"); 
    if (!home) return; 

    for (int i = 0; i < tokens->size; i++) {
        if (tokens->items[i][0] == '~') { 
            if (tokens->items[i][1] == '/' || tokens->items[i][1] == '\0') {
                
                size_t home_len = strlen(home);
                size_t token_len = strlen(tokens->items[i]);
                
                char *expanded = malloc(home_len + token_len); 
                strcpy(expanded, home); 
                if (tokens->items[i][1] == '/') {
                    strcat(expanded, tokens->items[i] + 1);
                }
                free(tokens->items[i]); 
                tokens->items[i] = expanded; 
            }
            
        }
    }
}

char *search_path(char *command) {
    if (strchr(command, '/')) {
       
        if (access(command, X_OK) == 0) { 
            return strdup(command); 
        } else {
            return NULL; 
        }
    }

    
    char *path_env = getenv("PATH");
    if (!path_env) return NULL; 

    
    char *path = strdup(path_env);
    char *token = strtok(path, ":"); 
    while (token != NULL) {
        size_t len = strlen(token) + strlen(command) + 2; 
        char *full_path = malloc(len);
        snprintf(full_path, len, "%s/%s", token, command); 

        if (access(full_path, X_OK) == 0) { 
            free(path); 
            return full_path; 
        }
        free(full_path);
        token = strtok(NULL, ":"); 
    }
    free(path); 
    return NULL; 
}

int execute_command(tokenlist *tokens, int background, char *command_line) {
    if (tokens->size == 0) return 1; 

   
    redirection redir = parse_redirection(tokens);

    
    char *cmd_path = search_path(tokens->items[0]);
    if (!cmd_path) {
        fprintf(stderr, "%s: command not found\n", tokens->items[0]);
        return 1;
    }

    
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        free(cmd_path);
        return 1;
    } else if (pid == 0) {
        

        
        if (redir.input_file || redir.output_file) {
            apply_redirection(redir);
        }

        
        char **args = malloc((tokens->size + 1) * sizeof(char *));
        for (int i = 0; i < tokens->size; i++) {
            args[i] = tokens->items[i];
        }
        args[tokens->size] = NULL; 

       
        execv(cmd_path, args);

        
        perror("execv");
        free(args);
        exit(EXIT_FAILURE); 
    } else {
       

        if (!background) {
            
            int status;
            waitpid(pid, &status, 0);
        } else {
            
            add_job(pid, command_line);
        }

        free(cmd_path); 
    }

    
    if (redir.input_file) free(redir.input_file);
    if (redir.output_file) free(redir.output_file);

    return 1; 
}
