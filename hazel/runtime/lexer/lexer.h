#ifndef HAZEL_LEXER
#define HAZEL_LEXER 

#include <hazel/runtime/parser/tokens.h>

typedef struct LexerDef {
    char c;
    unsigned int i;
    char* contents;
} Lexer;


Lexer* lexer_init(char* contents);

void lexer_advance(Lexer* lexer);
Token* lexer_advance_with_token(Lexer* lexer, Token* tok);

void lexer_skip_whitespace(Lexer* lexer);

Token* lexer_get_next_token(Lexer* lexer);

char* lexer_get_curr_char_as_string(Lexer* lexer);
Token* lexer_collect_string(Lexer* lexer); 
Token* lexer_collect_token_id(Lexer* lexer);

#endif
