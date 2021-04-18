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

