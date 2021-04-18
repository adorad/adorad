#include <string.h>
#include <ctype.h> 
#include <stdio.h> 

#include <hazel/core/hdef.h>
#include <hazel/runtime/lexer/lexer.h>

// Create a new lexer
Lexer* lexer_init(char* contents) {
    Lexer* lexer = calloc(1, sizeof(Lexer)); 
    lexer->contents = contents; 
    lexer->contents_length = strlen(contents);

    lexer->char_idx = 0;
    lexer->line_no = 1; 
    lexer->curr_char = lexer->contents[0]; // lexer->contents[lexer->char_idx]

    return lexer; 
} 

// Deallocate a lexer
void lexer_free(Lexer* lexer) {
    free(lexer->contents);
    free(lexer);
} 

// Get the next token from the Lexer
Token* lexer_get_next_token(Lexer* lexer) {
    while(lexer->curr_char != nullchar && lexer->char_idx < lexer->contents_length) {
        if(isWhitespace(lexer->curr_char))
            lexer_skip_whitespace(lexer);

        if(isDigit(lexer->curr_char)) 
            lexer_collect_digit(lexer);
        
        if(isalnum(lexer->curr_char)) 
            lexer_collect_token_id(lexer);
    }
} 

Token* lexer_advance_with_token(Lexer* lexer, int type) {

} 


void lexer_advance(Lexer* lexer) {

} 

void lexer_expect_char(Lexer* lexer, char c) {

}

void lexer_skip_whitespace(Lexer* lexer) {

} 

void lexer_skip_inline_comment(Lexer* lexer) {

}

void lexer_skip_block_comment(Lexer* lexer) {

}


Token* lexer_collect_string(Lexer* lexer) {

} 

Token* lexer_collect_char(Lexer* lexer) {

} 

Token* lexer_collect_digit(Lexer* lexer) {

} 

Token* lexer_collect_token_id(Lexer* lexer) {

} 


char* lexer_collect_charstr(Lexer* lexer) {

}

static inline bool ignore(char c) {
    return (c==' ' || c=='\t' || c=='\r');
}

static inline bool isWhitespace(char c) {
    return (c==' ' || (int)c==10 || (int)c==13);
}

static inline bool isletter(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'));
}

static inline bool isUpperCase(char c) {
    return (c>='A' && c<='Z');
}

static inline bool isLowerCase(char c) {
    return (c>='a' && c<='z');
}

static inline bool isDigit(char c) {
    return ((c >= '0' && c <= '9'));
}

static inline bool isNewLine(char c) {
    return c == '\n';
}

static inline bool isEnd(char c) {
    return c == '\0';
}