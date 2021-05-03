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
        if(LEXER_PEEK_CURR == 0x0A) { LEXER_NEXT; }
        return true; 
    }

    // Next Line
    if((c == 0xC2) && (LEXER_PEEK_CURR == 0x85)) {
        LEXER_NEXT; 
        return true;
    }
    
    // Line Separator
    if((c == 0xE2) && (LEXER_PEEK_CURR == 0x80) && (0xA8)) {
        LEXER_NEXT; 
        LEXER_NEXT; 
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

static inline bool isComment(char c1, char c2) {
    return isSlashComment(c1, c2) || isHashComment(c1) || isHashComment(c2);
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
Lexer* lexer_init(const char* buffer, char* fname) {
    Lexer* lexer = calloc(1, sizeof(Lexer));
    if(!lexer) 
        return null; 
    
    lexer->buffer = buffer; 
    lexer->buffer_capacity = strlen(buffer);
    lexer->offset = 0; 
    lexer->line_no = 1; 
    lexer->col_no = 0; 
    lexer->fname = fname; 
    // lexer->curr_char = lexer->buffer[0]; // lexer->buffer[lexer->char_idx]

    return lexer; 
} 

// Deallocate a lexer
void lexer_free(Lexer* lexer) {
    free(lexer->buffer);
    free(lexer);
} 

// Lexing Errors
TokenType lexer_error(Lexer* lexer, const char* message) {
    if(!LEXER_IS_EOF) {
        TOKEN_INCREMENT_TOKENLENGTH;
        LEXER_INCREMENT_OFFSET;
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
    TOKEN_INCREMENT_TOKENLENGTH; 

    // Do not change the declaration order of _next_ and _curr_
    char next = LEXER_NEXT; 
    char curr = LEXER_PEEK_CURR; 
    int token = 0; 

    switch(next) {
        // '='
        case '=':
            // '=='
            if(curr == '=') {
                LEXER_INCREMENT_OFFSET;
                TOKEN_INCREMENT_TOKENLENGTH;

                // 
                // Uncomment the following ONLY if Hazel ends up supporting '==='
                // curr = LEXER_PEEK_CURR; 
                // // '===' 
                // if(curr == '=') {
                //     INCREMENT_OFFSET;
                //     INCREMENT_TOKENLENGTH;
                //     token = EQUALS_EQUALS_EQUALS;
                // } else {
                //     token = EQUALS_EQUALS;
                // }

                token = EQUALS_EQUALS; 
            } 
            
            // '=>'
            if(curr == '>') {
                LEXER_INCREMENT_OFFSET;
                TOKEN_INCREMENT_TOKENLENGTH;
                token = EQUALS_ARROW;
            } else {
                token = EQUALS;
            }
            break; 
        
        // '+'
        case '+':
            // The following might be removed at some point. 
            // '++' serves no purpose for us since we don't support pointer arithmetic
            // 
            // '++'
            if(curr == '+') {
                LEXER_INCREMENT_OFFSET;
                TOKEN_INCREMENT_TOKENLENGTH;
                token = PLUS_PLUS;
            }

            // '+='
            if(curr == '=') {
                LEXER_INCREMENT_OFFSET; 
                TOKEN_INCREMENT_TOKENLENGTH;

                token = PLUS_EQUALS;
            } else {
                token = PLUS;
            }
            break; 
        
        // '-'
        case '-':
            // The following might be removed at some point. 
            // '--' serves no purpose for us since we don't support pointer arithmetic
            // 
            // '--'
            if(curr == '-') {
                LEXER_INCREMENT_OFFSET;
                TOKEN_INCREMENT_TOKENLENGTH;
                token = MINUS_MINUS;
            }

            // '->'
            if(curr == '>') {
                LEXER_INCREMENT_OFFSET;
                TOKEN_INCREMENT_TOKENLENGTH;
                token = RARROW;
            }

            // '-='
            if(curr == '=') {
                LEXER_INCREMENT_OFFSET; 
                TOKEN_INCREMENT_TOKENLENGTH;

                token = MINUS_EQUALS;
            } else {
                token = MINUS;
            }
            break; 

        // '*'
        case '*':
            // '**'
            if(curr == '*') {
                LEXER_INCREMENT_OFFSET;
                TOKEN_INCREMENT_TOKENLENGTH;
                token = MULT_MULT;
            }

            // '*='
            if(curr == '=') {
                LEXER_INCREMENT_OFFSET; 
                TOKEN_INCREMENT_TOKENLENGTH;

                token = MULT_EQUALS;
            } else {
                token = MULT;
            }
            break;
        
        // '/'
        case '/':
            // '//'
            if(curr == '/') {
                LEXER_INCREMENT_OFFSET;
                TOKEN_INCREMENT_TOKENLENGTH;
                token = SLASH_SLASH;
            }

            // '/='
            if(curr == '=') {
                LEXER_INCREMENT_OFFSET; 
                TOKEN_INCREMENT_TOKENLENGTH;

                token = SLASH_EQUALS;
            } else {
                token = SLASH;
            }
            break;
        
        // '!'
        case '!':
            // '!='
            if(curr == '=') {
                LEXER_INCREMENT_OFFSET; 
                TOKEN_INCREMENT_TOKENLENGTH;

                token = EXCLAMATION_EQUALS;
            } else {
                token = EXCLAMATION;
            }
            break;
        
        // '%'
        case '%':
            // '%%'
            if(curr == '%') {
                LEXER_INCREMENT_OFFSET;
                TOKEN_INCREMENT_TOKENLENGTH;
                token = MOD_MOD;
            }

            // '%='
            if(curr == '=') {
                LEXER_INCREMENT_OFFSET; 
                TOKEN_INCREMENT_TOKENLENGTH;

                token = MOD_EQUALS;
            } else {
                token = MOD;
            }
            break;
        
        // '&'
        case '&':
            // '&&'
            if(curr == '&') {
                LEXER_INCREMENT_OFFSET;
                TOKEN_INCREMENT_TOKENLENGTH;
                token = AND_AND;
            }

            // '&^'
            if(curr == '^') {
                LEXER_INCREMENT_OFFSET;
                TOKEN_INCREMENT_TOKENLENGTH;
                token = AND_NOT;
            }

            // '&='
            if(curr == '=') {
                LEXER_INCREMENT_OFFSET; 
                TOKEN_INCREMENT_TOKENLENGTH;

                token = AND_EQUALS;
            } else {
                token = AND;
            }
            break;
        
        // '|'
        case '|':
            // '||'
            if(curr == '|') {
                LEXER_INCREMENT_OFFSET;
                TOKEN_INCREMENT_TOKENLENGTH;
                token = OR_OR;
            }

            // '|='
            if(curr == '=') {
                LEXER_INCREMENT_OFFSET; 
                TOKEN_INCREMENT_TOKENLENGTH;

                token = OR_EQUALS;
            } else {
                token = OR;
            }
            break;
        
        // '^'
        case '^':
            // '^='
            if(curr == '=') {
                LEXER_INCREMENT_OFFSET; 
                TOKEN_INCREMENT_TOKENLENGTH;

                token = XOR_EQUALS;
            } else {
                token = XOR;
            }
            break;
        
        // '?'
        case '?':
            token = QUESTION;
            break;

        // '<'
        case '<':
            // '<='
            if(curr == '=') {
                LEXER_INCREMENT_OFFSET; 
                TOKEN_INCREMENT_TOKENLENGTH;

                token = LESS_THAN_OR_EQUAL_TO;
            }

            // '<-'
            else if(curr == '-') {
                LEXER_INCREMENT_OFFSET; 
                TOKEN_INCREMENT_TOKENLENGTH;

                token = LARROW;
            }

            // '<<'
            else if(curr == '<') {
                LEXER_INCREMENT_OFFSET;
                TOKEN_INCREMENT_TOKENLENGTH;

                // '<<='
                curr = LEXER_PEEK_CURR;
                if(curr == '=') {
                    LEXER_INCREMENT_OFFSET;
                    TOKEN_INCREMENT_TOKENLENGTH;
                    token = LBITSHIFT_EQUALS;
                } else {
                    token = LBITSHIFT;
                }
            } else {
                token = LESS_THAN;
            }
            break;
        
        // '>'
        case '>':
            // '>='
            if(curr == '=') {
                LEXER_INCREMENT_OFFSET; 
                TOKEN_INCREMENT_TOKENLENGTH;

                token = GREATER_THAN_OR_EQUAL_TO;
            } 

            // '>>'
            else if(curr == '>') {
                LEXER_INCREMENT_OFFSET;
                TOKEN_INCREMENT_TOKENLENGTH;

                // '>>='
                curr = LEXER_PEEK_CURR;
                if(curr == '=') {
                    LEXER_INCREMENT_OFFSET;
                    TOKEN_INCREMENT_TOKENLENGTH;
                    token = RBITSHIFT_EQUALS;
                } else {
                    token = RBITSHIFT;
                }
            } else {
                token = GREATER_THAN;
            }
            break;  
        
        // '~'
        case '~':
            // '~='
            if(curr == '=') {
                LEXER_INCREMENT_OFFSET; 
                TOKEN_INCREMENT_TOKENLENGTH;

                token = TILDA_EQUALS;
            } else {
                token = TILDA;
            }
            break;
        
        default: 
            printf("LEXER ERROR - UNRECOGNIZED TOKEN");
    }

    TOKEN_FINALIZE(token);
    // LEXER_DEBUG("Found operator: %s", token_toString(token));
    return token; 
} 


Token* lexer_lex_separator(Lexer* lexer) {
    TOKEN_RESET; 
    TOKEN_INCREMENT_TOKENLENGTH; 

    // Do not change the declaration order of _next_ and _curr_
    char next = LEXER_NEXT; 
    char curr = LEXER_PEEK_CURR; 
    int token = 0; 

    switch(next) {
        // '.'
        case '.':
            // '..'
            if(curr == '.') {
                LEXER_INCREMENT_OFFSET;
                TOKEN_INCREMENT_TOKENLENGTH;

                // '...'
                curr = LEXER_PEEK_CURR;
                if(curr == '.') {
                    LEXER_INCREMENT_OFFSET;
                    TOKEN_INCREMENT_TOKENLENGTH;
                    token = ELLIPSIS;
                } else {
                    token = DDOT;
                }
            } else {
                token = DOT;
            }
            break;  
        
        // ':'
        case ':':
            token = COLON;
            break; 
        
        // ':'
        case ';':
            token = SEMICOLON;
            break; 
        
        // ','
        case ',':
            token = COMMA;
            break; 

        // '\\'
        case '\\':
            token = BACKSLASH;
            break; 
        
        default: 
            printf("LEXER ERROR - UNRECOGNIZED TOKEN");
    }

    TOKEN_FINALIZE(token);
    // LEXER_DEBUG("Found separator: %s", token_toString(token));
    return token; 
}


Token* lexer_lex_delimiter(Lexer* lexer) {
    TOKEN_RESET; 
    TOKEN_INCREMENT_TOKENLENGTH; 

    // Do not change the declaration order of _next_ and _curr_
    char next = LEXER_NEXT; 
    char curr = LEXER_PEEK_CURR; 
    int token = 0; 

    switch(next) {        
        // '['
        case '[':
            token = LSQUAREBRACK;
            break; 
        
        // ']'
        case ';':
            token = RSQUAREBRACK;
            break; 
        
        // '{'
        case '{':
            token = LBRACE;
            break; 

        // '}'
        case '}':
            token = RBRACE;
            break; 
        
        // '('
        case '(':
            token = LPAREN;
            break; 
        
        // ')'
        case ')':
            token = RPAREN;
            break; 
        
        default: 
            printf("LEXER ERROR - UNRECOGNIZED TOKEN");
    }

    TOKEN_FINALIZE(token);
    // LEXER_DEBUG("Found delimiter: %s", token_toString(token));
    return token; 
}


Token* lexer_lex_macro(Lexer* lexer) {
    TOKEN_RESET; 
    TOKEN_INCREMENT_TOKENLENGTH; 
    int token = 0;

    char curr = LEXER_PEEK_CURR; 
    if(curr == '@') {
        token = MACRO;
    }

    TOKEN_FINALIZE(token);
    return token;
}


Token* lexer_lex_keywords(Lexer* lexer) {
    
}


Token* lexer_lex_char(Lexer* lexer) {

} 

Token* lexer_lex_digit(Lexer* lexer) {

} 

Token* lexer_lex_token_id(Lexer* lexer) {

} 


char* lexer_lex_charstr(Lexer* lexer) {

}
