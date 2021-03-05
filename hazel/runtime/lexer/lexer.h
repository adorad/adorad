#ifndef HAZEL_LEXER
#define HAZEL_LEXER 

#include <hazel/runtime/parser/tokens.h>


typedef struct LEXER_STRUCT {
    char c;
    unsigned int i;
    char* contents;
} lexer_T;

lexer_T* lexer_init(char* contents);

void advance_lexer(lexer_T* lexer);

void lexer_skip_whitespace(lexer_T* lexer);

#endif
