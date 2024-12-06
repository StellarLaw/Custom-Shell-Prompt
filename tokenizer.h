#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef struct {
    int size;       
    char **items;   
} tokenlist;

tokenlist *new_tokenlist(void);
void add_token(tokenlist *tokens, char *item);
tokenlist *get_tokens(char *input);
void free_tokens(tokenlist *tokens);

#endif
