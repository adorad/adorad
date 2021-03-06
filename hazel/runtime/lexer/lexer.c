#include <stdlib.h> 
#include <string.h>

#include <hazel/runtime/lexer/lexer.h> 
#include <hazel/runtime/parser/tokens.h> 

lexer_T* lexer_init(char* contents) {
    lexer_T* lexer = calloc(1, sizeof(struct LEXER_STRUCT)); 
    lexer->contents = contents; 
    lexer->i = 0; 
    lexer->c = contents[lexer->i];
    
    return lexer; 
}

void lexer_advance(lexer_T* lexer) {
    if(lexer->c != '\0' && lexer->i < strlen(lexer->contents)) {
        lexer->i += 1;
        lexer->c = lexer->contents[lexer->i];
    }
}

enum TokenNames* lexer_advance_with_token(lexer_T* lexer, enum TokenNames* tok) {
    lexer_advance(lexer);
    return tok; 
}

void lexer_skip_whitespace(lexer_T* lexer) {
    while(lexer->c == ' ' || lexer->c == 10) {
        lexer_advance(lexer);
    }
}

void lexer_get_next_token(lexer_T* lexer) {
    while(lexer->c != '\0' && lexer->i < strlen(lexer->contents)) {
        if(lexer->c == ' ' || lexer->c == 10) 
            lexer_skip_whitespace(lexer);
        
        switch(lexer->c) {
            case '=': return lexer_advance_with_token(lexer, token_init(EQUALS, lexer_get_curr_char_as_string(lexer))); break; 

        }
    }
}

enum TokenNames* lexer_collect_string(lexer_T* lexer) {

}

char* lexer_get_curr_char_as_string(lexer_T* lexer) {

}