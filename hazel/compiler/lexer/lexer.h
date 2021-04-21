#ifndef _HAZEL_LEXER
#define _HAZEL_LEXER 

#include <hazel/core/hcore.h> 
#include <hazel/compiler/tokens/tokens.h>

/*
    Hazel's Lexer is built in such a way that no (or negligible) memory allocations are necessary during usage. 

    In order to be able to not allocate any memory during tokenization, STRINGs and NUMBERs are just sanity checked but _not_ converted - it is the responsibility of the Parser to perform the right conversion.

    Reference: 
        1. ASCII Table: http://www.theasciicode.com.ar
*/


typedef struct LexerStruct {
    char* buffer;         // buffer
    UInt32 position;      // current buffer position (in characters)
    UInt32 buffer_length; // current buffer length (in Bytes)
    UInt32 offset;        // current buffer offset (in Bytes)

    char curr_char;   // current char 
    TokensEnum token; // current token
    UInt32 char_idx;  // the index of the token
    UInt32 line_no;   // the line number in the source where the token occured
    UInt32 col_no;    // the column number
    UInt32 file_id;   // the file ID
} Lexer; 


Lexer* lexer_init(char* buffer); 
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