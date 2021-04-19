#ifndef _HAZEL_LEXER
#define _HAZEL_LEXER 

#include <hazel/core/hcore.h> 
#include <hazel/runtime/tokens/tokens.h>

typedef struct LexerStruct {
    char* contents; 
    Ull contents_length; 

    char curr_char; 
    unsigned int char_idx; // the index of the token
    unsigned int line_no;  // the line number in the source where the token occured
} Lexer; 


Lexer* lexer_init(char* contents); 
void lexer_free(Lexer* lexer); 

Token* lexer_get_next_token(Lexer* lexer); 
Token* lexer_advance_with_token(Lexer* lexer, int type); 

void lexer_advance(Lexer* lexer); 
void lexer_expect_char(Lexer* lexer, char c);
void lexer_skip_whitespace(Lexer* lexer); 
void lexer_skip_inline_comment(Lexer* lexer);
void lexer_skip_block_comment(Lexer* lexer);

Token* lexer_collect_string(Lexer* lexer); 
Token* lexer_collect_char(Lexer* lexer); 
Token* lexer_collect_digit(Lexer* lexer); 
Token* lexer_collect_token_id(Lexer* lexer); 

char* lexer_collect_charstr(Lexer* lexer);

#endif // _HAZEL_LEXER