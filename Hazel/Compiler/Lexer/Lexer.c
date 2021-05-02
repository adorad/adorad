/*
_ _    _           ______   _______        
| |  | |    /\    /___  /   |  ____|| |    
| |__| |   /  \      / /    | |__   | |       Hazel - The Fast, Expressive & Elegant Programming Language
|  __  |  / /\ \    / /     |  __|  | |       Languages: C, C++, and Assembly
| |  | | / ____ \  / /___   | |____ | |____   https://github.com/HazelLang/Hazel/
|_|_ |_|/_/    \_\/_______\ |______|_\______|

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/

#include <Hazel/Compiler/Lexer/Lexer.h>

/*
    Lexer:
    In this phase, the source code is decomposed into its simplest "tokens". 

    Newlines are converted to newline tokens
*/

/*
 * INSPIRATION: https://github.com/JHG777000/marshmallow/blob/master/src/marshmallow_parser.c
 * INSPIRATION: https://github.com/k-mrm/maxc/blob/master/src/compiler/lexer.c

*/

// Useful Functions used by the Lexer 
static inline bool isNewLine(Lexer* lexer, char c) {
    // Carriage Return: U+000D (UTF-8 in hex: 0D)
    // Line Feed: U+000A (UTF-8 in hex: 0A)
    // CR+LF: CR (U+000D) followed by LF (U+000A) (UTF-8 in hex: 0D0A)
    // UTF-8 cases https://en.wikipedia.org/wiki/Newline#Unicode:
    //      1. Next Line, U+0085 (UTF-8 in hex: C285)
    //      2. Line Separator, U+2028 (UTF-8 in hex: E280A8)

    // Line Feed 
    if(c == 0x0A) return true; 

    // CR+LF or CR
    if(c == 0x0D) {
        if(PEEK_CURR == 0x0A) { NEXT; }
        return true; 
    }

    // Next Line
    if((c == 0xC2) && (PEEK_CURR == 0x85)) {
        NEXT; 
        return true;
    }
    
    // Line Separator
    if((c == 0xE2) && (PEEK_CURR == 0x80) && (0xA8)) {
        NEXT; 
        NEXT; 
        return true; 
    }

    // will add more at some point in the future 
    return false; 
}

static inline bool isSlashComment(char c1, char c2) {
    return (c1 == '/' && (c2 == '*' || c2 == '/'));
}

static inline bool isHashComment(char c) {
    return c == '#';
}

static inline bool isSemicolon(char c) {
    return c == ';';
}

static inline bool isString(char c) {
    return (c == '"' || c == '\'');
}

// Returns true if [c] is the beginning of a Macro (In Hazel, macros begin with the `@` sign)
static inline bool isMacro(char c) {
    return c == '@';
}

// Returns true if [c] is a valid (non-initial) identifier
static inline bool isIdentifier(char c) {
    return isAlpha(c) || isDigit(c) || c == '_'; 
}

static inline bool isBuiltinOperator(char c) {
    // Parenthesis
    // { } [ ] ( )
    // Punctuation
    // . ; : ? ,
    // Operators
    // + - * / < > ! = | & ^ % ~

    return ((c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '<') || (c == '>') || (c == '!') || 
            (c == '=') || (c == '|') || (c == '&') || (c == '^') || (c == '%') || (c == '~') || (c == '.') || 
            (c == ';') || (c == ':') || (c == '?') || (c == ',') || (c == '{') || (c == '}') || (c == '[') || 
            (c == ']') || (c == '(') || (c == ')') );
}

// Create a new lexer
// Lexer* lexer_init(const char* buffer) {
//     Lexer* lexer = calloc(1, sizeof(Lexer)); 
//     lexer->buffer = buffer; 
//     lexer->buffer_capacity = strlen(buffer);

//     // lexer->char_idx = 0;
//     lexer->line_no = 1; 
//     lexer->curr_char = lexer->buffer[0]; // lexer->buffer[lexer->char_idx]

//     return lexer; 
// } 

// Deallocate a lexer
void lexer_free(Lexer* lexer) {
    free(lexer->buffer);
    free(lexer);
} 

// Lexing Errors
TokenType lexer_error(Lexer* lexer, const char* message) {
    if(!LEXER_IS_EOF) {
        INCREMENT_TOKENLENGTH;
        INCREMENT_OFFSET_AND_POSITION;
    }
    TOKEN_FINALIZE(ILLEGAL);

    lexer->token.value = (char*)message; 
    lexer->token.bytes = (UInt32)strlen(message);
    return ILLEGAL; 
}


// Get the next token from the Lexer
// Token* lexer_get_next_token(Lexer* lexer) {
//     while(lexer->curr_char != nullchar && lexer->char_idx < lexer->buffer_capacity) {
//         if(isWhitespace(lexer->curr_char))
//             lexer_skip_whitespace(lexer);

//         if(isDigit(lexer->curr_char)) 
//             lexer_lex_digit(lexer);
        
//         if(isAlphanumeric(lexer->curr_char)) 
//             lexer_lex_token_id(lexer);
//     }
// } 

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


Token* lexer_lex_string(Lexer* lexer) {

} 

Token* lexer_lex_operator(Lexer* lexer) {
    TOKEN_RESET; 
    INCREMENT_TOKENLENGTH; 

    char curr = PEEK_CURR; 
    char next = NEXT; 
    int token = 0; 

    switch(next) {
        // '='
        case '=':
            // '=='
            if(curr == '=') {
                INCREMENT_OFFSET_AND_POSITION;
                INCREMENT_TOKENLENGTH;

                // 
                // Uncomment the following ONLY if Hazel ends up supporting '==='
                // curr = PEEK_CURR; 
                // // '===' 
                // if(curr == '=') {
                //     INCREMENT_OFFSET_AND_POSITION;
                //     INCREMENT_TOKENLENGTH;
                //     token = EQUALS_EQUALS_EQUALS;
                // } else {
                //     token = EQUALS_EQUALS;
                // }

                token = EQUALS_EQUALS; 
            } else {
                token = EQUALS;
            }
            break; 
        
        // '+'
        
    }

} 

Token* lexer_lex_char(Lexer* lexer) {

} 

Token* lexer_lex_digit(Lexer* lexer) {

} 

Token* lexer_lex_token_id(Lexer* lexer) {

} 


char* lexer_lex_charstr(Lexer* lexer) {

}
