#ifndef HAZEL_LEXER
#define HAZEL_LEXER 

#include <hazel/runtime/parser/tokens.h>

typedef struct Lexer {
    char c;
    unsigned int i;
    char* contents;
} lexer_T;

lexer_T* lexer_init(char* contents);

void lexer_advance(lexer_T* lexer);

void lexer_skip_whitespace(lexer_T* lexer);

void lexer_get_next_token(lexer_T* lexer);

char* lexer_get_curr_char_as_string(lexer_T* lexer);

TokenNames* lexer_collect_string(lexer_T* lexer); 

TokenNames* lexer_collect_token_id(lexer_T* lexer);

#endif
