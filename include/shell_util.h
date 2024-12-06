#ifndef SHELL_UTIL_H
#define SHELL_UTIL_H

#include "tokenizer.h"

void display_prompt();
void expand_env_vars(tokenlist *tokens);
void expand_tilde(tokenlist *tokens);
char *search_path(char *command);
int execute_command(tokenlist *tokens, int background, char *command_line);

#endif
