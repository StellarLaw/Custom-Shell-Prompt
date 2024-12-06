#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "tokenizer.h"
#include "shell_util.h"
#include <unistd.h>
#include "builtin.h"
#include "io_redirection.h"
#include "piping.h"
#include "background.h"
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>



void sigchld_handler(int sig){
        pid_t pid;
        int status;
        
        while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
            remove_job(pid);
        }

        
    } 

int main() {


   
    struct sigaction sa;
    sa.sa_handler = &sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    while (1) {

        
        display_prompt(); 

        char *command_line = NULL;
        size_t len = 0;
        ssize_t nread = getline(&command_line, &len, stdin);
        if (nread == -1) {
            printf("\n");
            break;
        }

        if (command_line[nread - 1] == '\n') {
            command_line[nread - 1] = '\0';
        }

        tokenlist *tokens = get_tokens(command_line);

        int background = 0;
        if (tokens->size > 0 && strcmp(tokens->items[tokens->size - 1], "&") == 0) {
            background = 1;
            free(tokens->items[tokens->size - 1]);
            tokens->size--;
            tokens->items[tokens->size] = NULL;
        }

        expand_env_vars(tokens);
        expand_tilde(tokens);

        if (execute_builtin(tokens)) {
            free_tokens(tokens);
            free(command_line);
            continue;
        }

        int has_pipe = 0;
        for (int i = 0; i < tokens->size; i++) {
            if (strcmp(tokens->items[i], "|") == 0) {
                has_pipe = 1;
                break;
            }
        }

        if (has_pipe) {
            execute_piped_commands(tokens, background, command_line);
        } else {
            execute_command(tokens, background, command_line);
        }

        free_tokens(tokens);
        free(command_line);
    }

    

    return 0;
}
