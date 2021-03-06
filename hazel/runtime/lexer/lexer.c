#include <hazel/runtime/lexer/lexer.h> 

lexer_T* lexer_init(char* contents) {
    lexer_T* lexer = calloc(1, sizeof(struct LEXER_STRUCT)); 
    lexer->contents = contents; 
    lexer->i = 0; 
    lexer->c = contents[lexer->i];
    
    return lexer; 
}

void advance_lexer(lexer_T* lexer) {

}

void lexer_skip_whitespace(lexer_T* lexer) {

}

void lexer_get_next_token(lexer_T* lexer) {

}

char* lexer_get_curr_char_as_string(lexer_T* lexer) {

}