#include <Hazel/Compiler/Tokens/Tokens.h> 

Token* token_init(int type, char* value) {
    Token* tok = calloc(1, sizeof(struct TokenNames)); 
    tok->type = type; 
    tok->value = value;
}