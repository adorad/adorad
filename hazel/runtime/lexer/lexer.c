#include <stdlib.h> 
#include <string.h>
#include <ctype.h> 

#include <hazel/internal/defines.h>
#include <hazel/runtime/lexer/lexer.h> 
#include <hazel/runtime/parser/tokens.h> 


static inline bool ignore(char c) {
    return (c==' ' || c=='\t' || c=='\r');
}

static inline bool isletter(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'));
}

static inline bool isnum(char c) {
    return ((c >= '0' && c <= '9'));
}


bool lexer_is_keyword() {
    for(int i=0; i<num_all_keywords; i++) {
        if(!strcmp())
    }
}

Lexer* lexer_init(char* contents) {
    Lexer* lexer = calloc(1, sizeof(struct LexerDef)); 
    lexer->contents = contents; 
    lexer->i = 0; 
    lexer->c = contents[lexer->i];
    
    return lexer; 
}

void lexer_advance(Lexer* lexer) {
    if(lexer->c != '\0' && lexer->i < strlen(lexer->contents)) {
        lexer->i += 1;
        lexer->c = lexer->contents[lexer->i];
    }
}

Token* lexer_advance_with_token(Lexer* lexer, Token* tok) {
    lexer_advance(lexer);
    return tok; 
}

void lexer_skip_whitespace(Lexer* lexer) {
    while(lexer->c == ' ' || lexer->c == 10) {
        lexer_advance(lexer);
    }
}

Token* lexer_collect_token_id(Lexer* lexer) {
    char* value = calloc(1, sizeof(char));
    value[0] = '\0'; // NULL 

    // While Alphanumeric 
    while(isalnum(lexer->c)) {
        char* s = lexer_get_curr_char_as_string(lexer);
        // Reallocate to fit the string we create here 
        value = realloc(value, (strlen(value) + strlen(s)+1)*sizeof(char));
        // Append `s` to `value`
        strcat(value, s); 

        lexer_advance(lexer);
    }

    return token_init(TOK_ID, value);
}

Token* lexer_collect_string(Lexer* lexer) {
    // Skip over the quote ("") we encounter
    lexer_advance(lexer);

    char* value = calloc(1, sizeof(char));
    value[0] = '\0'; // NULL 

    // Find the closing quote 
    while(lexer->c != '"') {
        char* s = lexer_get_curr_char_as_string(lexer);
        // Reallocate to fit the string we create here 
        value = realloc(value, (strlen(value) + strlen(s)+1)*sizeof(char));
        // Append `s` to `value`
        strcat(value, s); 

        lexer_advance(lexer);
    }

    // Ignore closing quote
    lexer_advance(lexer);

    return token_init(STRING, value);
}

char* lexer_get_curr_char_as_string(Lexer* lexer) {
    char* str = calloc(2, sizeof(char));
    str[0] = lexer->c;
    str[1] = '\0';
}


Token* lexer_get_next_token(Lexer* lexer) {
    while(lexer->c != '\0' && lexer->i < strlen(lexer->contents)) {
        if(lexer->c == ' ' || lexer->c == 10) 
            lexer_skip_whitespace(lexer);

        if(isalnum(lexer->c)) {
            return lexer_collect_token_id(lexer);
        }

        if(lexer->c == '"') {
            return lexer_collect_string(lexer);
        }

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
    return token_init(TOK_EOF, "\0"); 
}

bool lexer_is_keyword(Token* token) {

}