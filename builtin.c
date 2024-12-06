#include "builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "io_redirection.h"
#include "tokenizer.h"
#include "background.h"


int execute_builtin(tokenlist *tokens) {
     if (tokens->size == 0) return 0;

    
    if (strcmp(tokens->items[0], "exit") == 0) {
        
        while (job_count > 0) {
            sleep(1); 
        }
        exit(0); 
    }

    
    if (strcmp(tokens->items[0], "cd") == 0) {
        if (tokens->size > 2) {
            fprintf(stderr, "cd: too many arguments\n");
            return 1;
        }
        char *path = NULL;
        if (tokens->size == 1) {
            
            path = getenv("HOME");
            if (!path) path = "/"; 
        } else {
            path = tokens->items[1];
        }
        if (chdir(path) != 0) {
            perror("cd"); 
        }
        return 1; 
    }

    
    if (strcmp(tokens->items[0], "jobs") == 0) {
        list_jobs(); 
        return 1;
    }

    return 0; 
}
