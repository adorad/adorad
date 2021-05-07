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

// ================ Useful Functions used by the Lexer ============
// Check if the current Lexer state is at EOF
inline bool Lexer::is_EOF() { return this->__offset >= this->__buffer_capacity; }
// Extract a Token 
inline Token Lexer::extract_token() { return this->__token; }
// Set token type
void Lexer::set_token(TokenType tok_type) { this->__token.__type = tok_type; }
// Set token value 
void Lexer::set_token_value(std::string value) { this->__token.__value = value; }
// Set token bytes 
void Lexer::set_token_bytes(UInt32 bytes) { this->__token.__tok_bytes = bytes; }
// Increment Token Bytes
void Lexer::increment_tok_bytes() { ++this->__token.__tok_bytes;}
// Decrement Token Bytes
void Lexer::decrement_tok_bytes() { --this->__token.__tok_bytes;}
// Increment Token Length
void Lexer::increment_tok_length() { ++this->__token.__tok_length;}
// Decrement Token Length
void Lexer::decrement_tok_length() { --this->__token.__tok_length;}

// Increment the line number
void Lexer::increment_lineno() {
	++this->__location.__lineno; 
	this->reset_colno();
}

// Decrement the lineno
void Lexer::decrement_lineno() {
	--this->__location.__lineno; 
	this->reset_colno();
}

// Increment the column number
void Lexer::increment_colno() {
	++this->__location.__colno; 
}

// Decrement the colno
void Lexer::decrement_colno() {
	--this->__location.__colno; 
}

// Increment the Lexical Buffer offset
void Lexer::increment_offset() {
	++this->__offset; 
	this->increment_colno();
}

// Decrement the Lexical Buffer offset
void Lexer::decrement_offset() {
	--this->__offset; 
	this->decrement_colno();
}

// Reset the line
void Lexer::reset_lineno() { this->__location.set_lineno(0); }
// Reset the column number 
void Lexer::reset_colno() { this->__location.set_colno(0); }

// Reset a Lexer Token
void Lexer::reset_token() {
	this->__token.reset_();
	// TODO(jasmcaus): Verify this is accurate
	this->__token.__value = this->__buffer[this->__offset]; 
	this->__token.__location = this->__location;
}

// Finalize a Token
void Lexer::finalize_token(TokenType __tok) {
	this->__token.__type = __tok; 
	this->__token.__location.set_fname(this->__location.__fname);
}

// Reset the buffer 
void Lexer::reset_buffer() {
	this->__buffer = ""; 
	this->__buffer_capacity = 0;
}

// Reset the Lexer state
void Lexer::reset_() {
	this->__buffer = ""; 
	this->__buffer_capacity = 0;
	this->__offset = 0; 
	this->__location.reset_();
}

// static inline bool isNewLine(Lexer* lexer, char c) {
//     // Carriage Return: U+000D (UTF-8 in hex: 0D)
//     // Line Feed: U+000A (UTF-8 in hex: 0A)
//     // CR+LF: CR (U+000D) followed by LF (U+000A) (UTF-8 in hex: 0D0A)
//     // UTF-8 cases https://en.wikipedia.org/wiki/Newline#Unicode:
//     //      1. Next Line, U+0085 (UTF-8 in hex: C285)
//     //      2. Line Separator, U+2028 (UTF-8 in hex: E280A8)

//     // Line Feed 
//     if(c == 0x0A) return true; 

//     // CR+LF or CR
//     if(c == 0x0D) {
//         if(lexer->peek_curr() == 0x0A) { lexer->next(); }
//         return true; 
//     }

//     // Next Line
//     if((c == 0xC2) && (lexer->peek_curr() == 0x85)) {
//         lexer->next(); 
//         return true;
//     }
    
//     // Line Separator
//     if((c == 0xE2) && (lexer->peek_curr() == 0x80) && (0xA8)) {
//         lexer->next(); 
//         lexer->next(); 
//         return true; 
//     }

//     // will add more at some point in the future 
//     return false; 
// }

static inline bool isComment(char c1, char c2) { return isSlashComment(c1, c2) || isHashComment(c1) || isHashComment(c2); }
static inline bool isSlashComment(char c1, char c2) { return (c1 == '/' && (c2 == '*' || c2 == '/')); }
static inline bool isHashComment(char c) { return c == '#';}
static inline bool isSemicolon(char c) { return c == ';'; }
static inline bool isString(char c) { return (c == '"' || c == '\'');}
// Returns true if [c] is the beginning of a Macro (In Hazel, macros begin with the `@` sign)
static inline bool isMacro(char c) { return c == '@';}
// Returns true if [c] is a valid (non-initial) identifier
static inline bool isIdentifier(char c) { return isAlpha(c) || isDigit(c) || c == '_'; }

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


// Lexing Errors
TokenType lexer_error(Lexer* lexer, std::string message) {
    if(!lexer->is_EOF()) {
        lexer->increment_tok_length();
        lexer->increment_offset();
    }
    lexer->finalize_token(TOK_ILLEGAL);
    lexer->set_token_value(message);
    lexer->set_token_bytes(message.length());
    return TOK_ILLEGAL; 
}


// // Get the next token from the Lexer
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

TokenType lexer_lex_operator(Lexer* lexer) {
    lexer->reset_token(); 
    lexer->increment_tok_length(); 

    // Do not change the declaration order of _next_ and _curr_
    char next = lexer->next(); 
    char curr = lexer->peek_curr(); 
    TokenType token = TOK_ILLEGAL; 

    switch(next) {
        // '='
        case '=':
            // '=='
            if(curr == '=') {
                lexer->increment_offset();
                lexer->increment_tok_length();

                // 
                // Uncomment the following ONLY if Hazel ends up supporting '==='
                // curr = lexer->peek_curr(); 
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
                lexer->increment_offset();
                lexer->increment_tok_length();
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
                lexer->increment_offset();
                lexer->increment_tok_length();
                token = PLUS_PLUS;
            }

            // '+='
            if(curr == '=') {
                lexer->increment_offset(); 
                lexer->increment_tok_length();

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
                lexer->increment_offset();
                lexer->increment_tok_length();
                token = MINUS_MINUS;
            }

            // '->'
            if(curr == '>') {
                lexer->increment_offset();
                lexer->increment_tok_length();
                token = RARROW;
            }

            // '-='
            if(curr == '=') {
                lexer->increment_offset(); 
                lexer->increment_tok_length();

                token = MINUS_EQUALS;
            } else {
                token = MINUS;
            }
            break; 

        // '*'
        case '*':
            // '**'
            if(curr == '*') {
                lexer->increment_offset();
                lexer->increment_tok_length();
                token = MULT_MULT;
            }

            // '*='
            if(curr == '=') {
                lexer->increment_offset(); 
                lexer->increment_tok_length();

                token = MULT_EQUALS;
            } else {
                token = MULT;
            }
            break;
        
        // '/'
        case '/':
            // '//'
            if(curr == '/') {
                lexer->increment_offset();
                lexer->increment_tok_length();
                token = SLASH_SLASH;
            }

            // '/='
            if(curr == '=') {
                lexer->increment_offset(); 
                lexer->increment_tok_length();

                token = SLASH_EQUALS;
            } else {
                token = SLASH;
            }
            break;
        
        // '!'
        case '!':
            // '!='
            if(curr == '=') {
                lexer->increment_offset(); 
                lexer->increment_tok_length();

                token = EXCLAMATION_EQUALS;
            } else {
                token = EXCLAMATION;
            }
            break;
        
        // '%'
        case '%':
            // '%%'
            if(curr == '%') {
                lexer->increment_offset();
                lexer->increment_tok_length();
                token = MOD_MOD;
            }

            // '%='
            if(curr == '=') {
                lexer->increment_offset(); 
                lexer->increment_tok_length();

                token = MOD_EQUALS;
            } else {
                token = MOD;
            }
            break;
        
        // '&'
        case '&':
            // '&&'
            if(curr == '&') {
                lexer->increment_offset();
                lexer->increment_tok_length();
                token = AND_AND;
            }

            // '&^'
            if(curr == '^') {
                lexer->increment_offset();
                lexer->increment_tok_length();
                token = AND_NOT;
            }

            // '&='
            if(curr == '=') {
                lexer->increment_offset(); 
                lexer->increment_tok_length();

                token = AND_EQUALS;
            } else {
                token = AND;
            }
            break;
        
        // '|'
        case '|':
            // '||'
            if(curr == '|') {
                lexer->increment_offset();
                lexer->increment_tok_length();
                token = OR_OR;
            }

            // '|='
            if(curr == '=') {
                lexer->increment_offset(); 
                lexer->increment_tok_length();

                token = OR_EQUALS;
            } else {
                token = OR;
            }
            break;
        
        // '^'
        case '^':
            // '^='
            if(curr == '=') {
                lexer->increment_offset(); 
                lexer->increment_tok_length();

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
                lexer->increment_offset(); 
                lexer->increment_tok_length();

                token = LESS_THAN_OR_EQUAL_TO;
            }

            // '<-'
            else if(curr == '-') {
                lexer->increment_offset(); 
                lexer->increment_tok_length();

                token = LARROW;
            }

            // '<<'
            else if(curr == '<') {
                lexer->increment_offset();
                lexer->increment_tok_length();

                // '<<='
                curr = lexer->peek_curr();
                if(curr == '=') {
                    lexer->increment_offset();
                    lexer->increment_tok_length();
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
                lexer->increment_offset(); 
                lexer->increment_tok_length();

                token = GREATER_THAN_OR_EQUAL_TO;
            } 

            // '>>'
            else if(curr == '>') {
                lexer->increment_offset();
                lexer->increment_tok_length();

                // '>>='
                curr = lexer->peek_curr();
                if(curr == '=') {
                    lexer->increment_offset();
                    lexer->increment_tok_length();
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
                lexer->increment_offset(); 
                lexer->increment_tok_length();

                token = TILDA_EQUALS;
            } else {
                token = TILDA;
            }
            break;
        
        default: 
            // printf("LexerError -> Unrecognized Token (%c)", next);
            printf("LexerError -> Unrecognized Token");
            abort();
    }

    lexer->finalize_token(token);
    // LEXER_DEBUG("Found operator: %s", token_toString(token));
    return token; 
} 


TokenType lexer_lex_separator(Lexer* lexer) {
    lexer->reset_token(); 
    lexer->increment_tok_length(); 

    // Do not change the declaration order of _next_ and _curr_
    char next = lexer->next(); 
    char curr = lexer->peek_curr(); 
    TokenType token = TOK_ILLEGAL; 

    switch(next) {
        // '.'
        case '.':
            // '..'
            if(curr == '.') {
                lexer->increment_offset();
                lexer->increment_tok_length();

                // '...'
                curr = lexer->peek_curr();
                if(curr == '.') {
                    lexer->increment_offset();
                    lexer->increment_tok_length();
                    token = ELLIPSIS;
                } else {
                    token = DDOT;
                }
            } else {
                token = DOT;
            }
            break;  
    
        case ':': token = COLON; break; 
        case ';': token = SEMICOLON; break; 
        case ',': token = COMMA; break; 
        case '\\': token = BACKSLASH; break; 
        default: 
            printf("LexerError -> Unrecognized Token");
            abort();
    }

    lexer->finalize_token(token);
    // LEXER_DEBUG("Found separator: %s", token_toString(token));
    return token; 
}


TokenType lexer_lex_delimiter(Lexer* lexer) {
    lexer->reset_token(); 
    lexer->increment_tok_length(); 

    // Do not change the declaration order of _next_ and _curr_
    char next = lexer->next(); 
    char curr = lexer->peek_curr(); 
    TokenType token = TOK_ILLEGAL; 

    switch(next) {       
        case '[': token = LSQUAREBRACK; break; 
        case ';': token = RSQUAREBRACK; break; 
        case '{': token = LBRACE; break; 
        case '}': token = RBRACE; break; 
        case '(': token = LPAREN; break; 
        case ')': token = RPAREN; break; 
        default:  
            printf("LexerError -> Unrecognized Token"); 
            abort();
    }
        
    lexer->finalize_token(token);
    // LEXER_DEBUG("Found delimiter: %s", token_toString(token));
    return token; 
}


TokenType lexer_lex_macro(Lexer* lexer) {
    lexer->reset_token(); 
    lexer->increment_tok_length(); 
    TokenType token = TOK_ILLEGAL;

    char curr = lexer->peek_curr(); 
    if(curr == '@')
        token = MACRO;

    lexer->finalize_token(token);
    return token;
}


// TokenType lexer_lex_keywords(Lexer* lexer) {
    
// }


// TokenType lexer_lex_char(Lexer* lexer) {

// } 

// TokenType lexer_lex_digit(Lexer* lexer) {

// } 

// TokenType lexer_lex_token_id(Lexer* lexer) {

// } 


// char* lexer_lex_charstr(Lexer* lexer) {

// }