#ifndef PIPING_H
#define PIPING_H

#include "tokenizer.h"

int execute_piped_commands(tokenlist *tokens, int background, char *command_line);

#endif
