#include "tokenizer.h"
#include <stdlib.h>
#include <string.h>

tokenlist *new_tokenlist(void) {
    tokenlist *tokens = (tokenlist *)malloc(sizeof(tokenlist));
    tokens->size = 0;
    tokens->items = (char **)malloc(sizeof(char *));
    tokens->items[0] = NULL; /* Make NULL terminated */
    return tokens;
}

void add_token(tokenlist *tokens, char *item) {
    int i = tokens->size;
    tokens->items = (char **)realloc(tokens->items, (i + 2) * sizeof(char *));
    tokens->items[i] = strdup(item); // Duplicate the token string
    tokens->items[i + 1] = NULL; // NULL terminate the list
    tokens->size += 1;
}

tokenlist *get_tokens(char *input) {
    tokenlist *tokens = new_tokenlist();
    char *p = input;
    while (*p) {
        // Skip leading spaces and tabs
        while (*p && (*p == ' ' || *p == '\t')) p++;
        if (*p == '\0') break;

        char *start;
        char *token;
        if (*p == '\"' || *p == '\'') {
            // Handle quoted tokens
            char quote = *p;
            p++; // Skip the opening quote
            start = p;
            while (*p && *p != quote) p++;
            if (*p == quote) {
                *p = '\0'; // Terminate the token
                token = strdup(start);
                p++; // Skip the closing quote
            } else {
                // No closing quote found
                token = strdup(start);
            }
        } else if (*p == '>') {
            // Handle '>' and '>>'
            if (*(p + 1) == '>') {
                token = strdup(">>");
                p += 2; // Skip both '>' characters
            } else {
                token = strdup(">");
                p++; // Skip the single '>'
            }
        } else if (*p == '<') {
            // Handle '<'
            token = strdup("<");
            p++; // Skip the '<'
        } else {
            // Handle unquoted tokens
            start = p;
            while (*p && *p != ' ' && *p != '\t' && *p != '>' && *p != '<') p++;
            int len = p - start;
            token = (char *)malloc(len + 1);
            strncpy(token, start, len);
            token[len] = '\0';
        }
        add_token(tokens, token);
        free(token); // add_token duplicates the string
    }
    return tokens;
}

void free_tokens(tokenlist *tokens) {
    for (int i = 0; i < tokens->size; i++)
        free(tokens->items[i]);
    free(tokens->items);
    free(tokens);
}
