#include <stdlib.h> 
#include <string.h> 

#include <hazel/runtime/tokens/tokens.h>

Token* token_init(int type, char* value) {
    Token* token = calloc(1, sizeof(Token));
    token->type = type; 
    token->value = calloc(strlen(value)+1, sizeof(char)); 
    strcopy(token->value, value);

    return token;
}    

Token* token_clone(Token* token) {
    return token_init(token->type, token->value); 
}

void token_free(Token* token) {
    free(token->value); // Important because we allocate separately for `token->value`
    free(token);
}

char* token_to_string(Token* token) {
    char* str = calloc(strlen(token->value)+1, sizeof(char));
    strcopy(str, token->value);

    return str;
}