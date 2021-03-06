#include <stdlib.h> 
#include <string.h>

#include <hazel/runtime/lexer/lexer.h> 
#include <hazel/runtime/parser/tokens.h> 

lexer_T* lexer_init(char* contents) {
    lexer_T* lexer = calloc(1, sizeof(struct Lexer)); 
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

TokenNames* lexer_advance_with_token(lexer_T* lexer, TokenNames* tok) {
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
            case ';': return lexer_advance_with_token(lexer, token_init(COLON,  lexer_get_curr_char_as_string(lexer))); break; 
            case ',': return lexer_advance_with_token(lexer, token_init(COMMA,  lexer_get_curr_char_as_string(lexer))); break; 
            case '@': return lexer_advance_with_token(lexer, token_init(AT_SIGN, lexer_get_curr_char_as_string(lexer))); break;
            // Operators 
            case '+': return lexer_advance_with_token(lexer, token_init(ADD, lexer_get_curr_char_as_string(lexer))); break; 
            case '-': return lexer_advance_with_token(lexer, token_init(SUBTRACT, lexer_get_curr_char_as_string(lexer))); break; 
            case '*': return lexer_advance_with_token(lexer, token_init(MUTLIPLICATION, lexer_get_curr_char_as_string(lexer))); break; 
            case '/': return lexer_advance_with_token(lexer, token_init(QUOTIENT, lexer_get_curr_char_as_string(lexer))); break; 
            case '%': return lexer_advance_with_token(lexer, token_init(REM, lexer_get_curr_char_as_string(lexer))); break; 
            case '&': return lexer_advance_with_token(lexer, token_init(AND, lexer_get_curr_char_as_string(lexer))); break; 
            case '|': return lexer_advance_with_token(lexer, token_init(OR, lexer_get_curr_char_as_string(lexer))); break; 
            case '!': return lexer_advance_with_token(lexer, token_init(NOT, lexer_get_curr_char_as_string(lexer))); break; 
            case '^': return lexer_advance_with_token(lexer, token_init(XOR, lexer_get_curr_char_as_string(lexer))); break; 
            // Comparison
            case '>': return lexer_advance_with_token(lexer, token_init(GREATER, lexer_get_curr_char_as_string(lexer))); break; 
            case '<': return lexer_advance_with_token(lexer, token_init(LESS, lexer_get_curr_char_as_string(lexer))); break; 
            // Delimiters
            case '[': return lexer_advance_with_token(lexer, token_init(LBRACK, lexer_get_curr_char_as_string(lexer))); break; 
            case ']': return lexer_advance_with_token(lexer, token_init(RBRACK, lexer_get_curr_char_as_string(lexer))); break; 
            case '{': return lexer_advance_with_token(lexer, token_init(LBRACE, lexer_get_curr_char_as_string(lexer))); break; 
            case '}': return lexer_advance_with_token(lexer, token_init(RBRACE, lexer_get_curr_char_as_string(lexer))); break; 
            case '(': return lexer_advance_with_token(lexer, token_init(LPAREN, lexer_get_curr_char_as_string(lexer))); break; 
            case ')': return lexer_advance_with_token(lexer, token_init(RPAREN, lexer_get_curr_char_as_string(lexer))); break;


        }
    }
}

TokenNames* lexer_collect_string(lexer_T* lexer) {

}

char* lexer_get_curr_char_as_string(lexer_T* lexer) {

}