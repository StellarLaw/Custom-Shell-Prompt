#ifndef IO_REDIRECTION_H
#define IO_REDIRECTION_H

#include "tokenizer.h"

typedef struct {
    char *input_file;
    char *output_file;
    int append;
} redirection;

redirection parse_redirection(tokenlist *tokens);
void apply_redirection(redirection redir);

#endif
