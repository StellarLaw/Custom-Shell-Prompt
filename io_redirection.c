#include "io_redirection.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

redirection parse_redirection(tokenlist *tokens) {
    redirection redir = {NULL, NULL, 0}; 

    for (int i = 0; i < tokens->size; i++) {
        if (strcmp(tokens->items[i], "<") == 0 && i + 1 < tokens->size) {
            
            redir.input_file = strdup(tokens->items[i + 1]); 

            
            free(tokens->items[i]);
            free(tokens->items[i + 1]);
            for (int j = i; j < tokens->size - 2; j++) {
                tokens->items[j] = tokens->items[j + 2];
            }
            tokens->size -= 2;
            tokens->items = realloc(tokens->items, (tokens->size + 1) * sizeof(char *));
            tokens->items[tokens->size] = NULL;
            i--; 
        }
        else if ((strcmp(tokens->items[i], ">") == 0 ||
            strcmp(tokens->items[i], ">>") == 0) && i + 1 < tokens->size) {

            redir.output_file = strdup(tokens->items[i + 1]); 
            redir.append = (strcmp(tokens->items[i], ">>") == 0) ? 1 : 0; 

           
            free(tokens->items[i]);
            free(tokens->items[i + 1]);
            for (int j = i; j < tokens->size - 2; j++) {
                tokens->items[j] = tokens->items[j + 2];
            }
            tokens->size -= 2;
            tokens->items = realloc(tokens->items, (tokens->size + 1) * sizeof(char *));
            tokens->items[tokens->size] = NULL;
            i--; 
        }
    }
    return redir; 
}

void apply_redirection(redirection redir) {
    if (redir.input_file) {
       
        int fd_in = open(redir.input_file, O_RDONLY);
        if (fd_in < 0) {
            perror("open input_file");
            exit(EXIT_FAILURE); 
        }
        dup2(fd_in, STDIN_FILENO); 
        close(fd_in);
    }

    if (redir.output_file) {
        int fd_out;
        if (redir.append) {
            
            fd_out = open(redir.output_file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
        } else {
            
            fd_out = open(redir.output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        }
        if (fd_out < 0) {
            perror("open output_file");
            exit(EXIT_FAILURE); 
        }
        dup2(fd_out, STDOUT_FILENO); 
        close(fd_out); 
    }
}
