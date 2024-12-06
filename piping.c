#include "piping.h"
#include "shell_util.h"
#include "io_redirection.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "background.h"  


int execute_piped_commands(tokenlist *tokens, int background, char *command_line) {
    
    int num_pipes = 0;
    for (int i = 0; i < tokens->size; i++) {
        if (strcmp(tokens->items[i], "|") == 0) num_pipes++;
    }

    if (num_pipes > 2) {
        
        fprintf(stderr, "Error: More than two pipes are not supported.\n");
        return 1;
    }

   
    tokenlist *cmds[3]; 
    for (int i = 0; i < 3; i++) cmds[i] = new_tokenlist();

    int cmd_index = 0;
    for (int i = 0; i < tokens->size; i++) {
        if (strcmp(tokens->items[i], "|") == 0) {
            cmd_index++; 
            continue;
        }
        add_token(cmds[cmd_index], tokens->items[i]);
    }

    // Create pipes
    int pipe_fds[2][2]; // Supports up to two pipes
    for (int i = 0; i < num_pipes; i++) {
        if (pipe(pipe_fds[i]) < 0) {
            perror("pipe");
            return 1;
        }
    }

    pid_t pids[3] = {0}; // Store PIDs of child processes

    for (int i = 0; i <= num_pipes; i++) {
        // Search for the command in $PATH
        char *cmd_path = search_path(cmds[i]->items[0]);
        if (!cmd_path) {
            fprintf(stderr, "%s: command not found\n", cmds[i]->items[0]);
            return 1;
        }

        // Fork a child process for each command
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork");
            return 1;
        } else if (pids[i] == 0) {
            // Child process

            
            if (i > 0) {
                dup2(pipe_fds[i - 1][0], STDIN_FILENO);
            }

           
            if (i < num_pipes) {
                dup2(pipe_fds[i][1], STDOUT_FILENO);
            }

            
            for (int j = 0; j < num_pipes; j++) {
                close(pipe_fds[j][0]);
                close(pipe_fds[j][1]);
            }

            
            if (i == num_pipes) {
                redirection redir = parse_redirection(cmds[i]);
                if (redir.input_file || redir.output_file) {
                    apply_redirection(redir);
                }
            }

           
            char **args = malloc((cmds[i]->size + 1) * sizeof(char *));
            for (int j = 0; j < cmds[i]->size; j++) {
                args[j] = cmds[i]->items[j];
            }
            args[cmds[i]->size] = NULL; 

            

           
            execv(cmd_path, args);

            
            perror("execv");
            free(args);
            exit(EXIT_FAILURE);
        }
        free(cmd_path); 
    }


   
    for (int j = 0; j < num_pipes; j++) {
        close(pipe_fds[j][0]); 
        close(pipe_fds[j][1]); 
    }

    
    for (int i = 0; i < num_pipes; i++) {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
    }



    if (!background) {
        
        for (int i = 0; i <= num_pipes; i++) {
            int status;
            waitpid(pids[i], &status, 0);
        }
    } else {
        
        add_job(pids[num_pipes], command_line);
    }

    
    for (int i = 0; i <= num_pipes; i++) {
        free_tokens(cmds[i]);
    }

    return 1; // Continue the shell loop
}
