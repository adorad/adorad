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
    In lexer phase, the source code is decomposed into its simplest "tokens". 

    Newlines are converted to newline tokens
*/

/*
 * INSPIRATION: https://github.com/JHG777000/marshmallow/blob/master/src/marshmallow_parser.c
 * INSPIRATION: https://github.com/k-mrm/maxc/blob/master/src/compiler/lexer.c

*/

Lexer* lexer_init(const char* buffer) {
    Lexer* lexer = calloc(1, sizeof(Lexer));
    lexer->buffer = buffer; 
    lexer->buffer_capacity = strlen(buffer);
    lexer->offset = 0; 
    lexer_location_init(lexer);
    return lexer;
}

inline char lexer_next(Lexer* lexer) {
    lexer_increment_colno(lexer);
    return (char)lexer->buffer[lexer->offset++];
}

// Lexer peek() allows you to "look ahead" `n` characters in the Lexical buffer
// It _does not_ increment the buffer offset 
inline char lexer_peek(Lexer* lexer, int n) {
    if(lexer->offset + (n-1) < lexer->buffer_capacity) {
        return (char)lexer->buffer[lexer->offset + n];
    } else {
        return 0;
    }
}

// Lexer peek_curr() returns the current element in the Lexical Buffer
inline char lexer_peek_curr(Lexer* lexer) {
    return (char)lexer->buffer[lexer->offset];
}

// ================ Useful Functions used by the Lexer ============
// Check if the current Lexer state is at EOF
inline bool lexer_is_EOF(Lexer* lexer) { return lexer->offset >= lexer->buffer_capacity; }
// Extract a Token 
// inline Token lexer_extract_token(Lexer* lexer) { return lexer->token; }
// Set token type
void lexer_set_token(Lexer* lexer, TokenType tok_type) { lexer->token.type = tok_type; }
// Set token value 
void lexer_set_token_value(Lexer* lexer, const char* value) { lexer->token.value = value; }
// Set token bytes 
void lexer_set_token_bytes(Lexer* lexer, UInt32 bytes) { lexer->token.tok_bytes = bytes; }
// Increment Token Bytes
void lexer_increment_tok_bytes(Lexer* lexer) { ++lexer->token.tok_bytes;}
// Decrement Token Bytes
void lexer_decrement_tok_bytes(Lexer* lexer) { --lexer->token.tok_bytes;}
// Increment Token Length
void lexer_increment_tok_length(Lexer* lexer) { ++lexer->token.tok_length;}
// Decrement Token Length
void lexer_decrement_tok_length(Lexer* lexer) { --lexer->token.tok_length;}

// Increment the line number
void lexer_increment_lineno(Lexer* lexer) {
	++lexer->location.lineno; 
	lexer_reset_colno(lexer);
}

// Decrement the lineno
void lexer_decrement_lineno(Lexer* lexer) {
	--lexer->location.lineno; 
    lexer_reset_colno(lexer);
}

// Increment the column number
void lexer_increment_colno(Lexer* lexer) {
	++lexer->location.colno; 
}

// Decrement the colno
void lexer_decrement_colno(Lexer* lexer) {
	--lexer->location.colno; 
}

// Increment the Lexical Buffer offset
void lexer_increment_offset(Lexer* lexer) {
	++lexer->offset; 
	lexer_increment_colno(lexer);
}

// Decrement the Lexical Buffer offset
void lexer_decrement_offset(Lexer* lexer) {
	--lexer->offset; 
	lexer_decrement_colno(lexer);
}

// Reset the line
void lexer_reset_lineno(Lexer* lexer) { lexer->location.lineno = 0; }
// Reset the column number 
void lexer_reset_colno(Lexer* lexer) { lexer->location.colno = 0; }

// Reset a Lexer Token
void lexer_reset_token(Lexer* lexer) {
    // TODO
	// lexer->token.reset_();
    // lexer_reset_token(lexer);
	// TODO(jasmcaus): Verify lexer is accurate
	lexer->token.value = lexer->buffer[lexer->offset]; 
	lexer->token.location = lexer->location;
}

// Finalize a Token
void lexer_finalize_token(Lexer* lexer, TokenType __tok) {
	lexer->token.type = __tok; 
	lexer->token.location.fname = lexer->location.fname;
}

// Reset the buffer 
void lexer_reset_buffer(Lexer* lexer) {
	lexer->buffer= ""; 
	lexer->buffer_capacity = 0;
}

// Reset the Lexer state
void lexer_reset_(Lexer* lexer) {
	lexer->buffer= ""; 
	lexer->buffer_capacity = 0;
	lexer->offset = 0; 
    lexer_location_init(lexer);
}

void lexer_location_init(Lexer* lexer) {
    lexer->location.lineno = 0; 
    lexer->location.colno = 0; 
    lexer->location.fname = "";
}

inline void lexer_set_lineno(Lexer* lexer, UInt32 lineno) { lexer->location.lineno = lineno; }
inline void lexer_set_colno(Lexer* lexer, UInt32 colno) { lexer->location.colno = colno; }
inline void lexer_set_fname(Lexer* lexer, const const char* fname) { lexer->location.fname = fname; }

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

// static inline bool isComment(char c1, char c2) { return isSlashComment(c1, c2) || isHashComment(c1) || isHashComment(c2); }
// static inline bool isSlashComment(char c1, char c2) { return (c1 == '/' && (c2 == '*' || c2 == '/')); }
// static inline bool isHashComment(char c) { return c == '#';}
// static inline bool isSemicolon(char c) { return c == ';'; }
// static inline bool isString(char c) { return (c == '"' || c == '\'');}
// // Returns true if [c] is the beginning of a Macro (In Hazel, macros begin with the `@` sign)
// static inline bool isMacro(char c) { return c == '@';}
// // Returns true if [c] is a valid (non-initial) identifier
// static inline bool isIdentifier(char c) { return isAlpha(c) || isDigit(c) || c == '_'; }

// static inline bool isBuiltinOperator(char c) {
//     // Parenthesis
//     // { } [ ] ( )
//     // Punctuation
//     // . ; : ? ,
//     // Operators
//     // + - * / < > ! = | & ^ % ~
//     return ((c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '<') || (c == '>') || (c == '!') || 
//             (c == '=') || (c == '|') || (c == '&') || (c == '^') || (c == '%') || (c == '~') || (c == '.') || 
//             (c == ';') || (c == ':') || (c == '?') || (c == ',') || (c == '{') || (c == '}') || (c == '[') || 
//             (c == ']') || (c == '(') || (c == ')') );
// }


// // Lexing Errors
// TokenType lexer_error(Lexer* lexer, const char* message) {
//     if(!lexer->is_EOF()) {
//         lexer->increment_tok_length();
//         lexer->increment_offset();
//     }
//     lexer->finalize_token(TOK_ILLEGAL);
//     lexer->set_token_value(message);
//     lexer->set_token_bytes(message.length());
//     return TOK_ILLEGAL; 
// }


// TokenType lexer_lex_operator(Lexer* lexer) {
//     lexer->reset_token(); 
//     lexer->increment_tok_length(); 

//     // Do not change the declaration order of _next_ and _curr_
//     char next = lexer->next(); 
//     char curr = lexer->peek_curr(); 
//     TokenType token = TOK_ILLEGAL; 

//     switch(next) {
//         // '='
//         case '=':
//             // '=='
//             if(curr == '=') {
//                 lexer->increment_offset();
//                 lexer->increment_tok_length();

//                 // 
//                 // Uncomment the following ONLY if Hazel ends up supporting '==='
//                 // curr = lexer->peek_curr(); 
//                 // // '===' 
//                 // if(curr == '=') {
//                 //     INCREMENT_OFFSET;
//                 //     INCREMENT_TOKENLENGTH;
//                 //     token = EQUALS_EQUALS_EQUALS;
//                 // } else {
//                 //     token = EQUALS_EQUALS;
//                 // }

//                 token = EQUALS_EQUALS; 
//             } 
            
//             // '=>'
//             if(curr == '>') {
//                 lexer->increment_offset();
//                 lexer->increment_tok_length();
//                 token = EQUALS_ARROW;
//             } else {
//                 token = EQUALS;
//             }
//             break; 
        
//         // '+'
//         case '+':
//             // The following might be removed at some point. 
//             // '++' serves no purpose for us since we don't support pointer arithmetic
//             // 
//             // '++'
//             if(curr == '+') {
//                 lexer->increment_offset();
//                 lexer->increment_tok_length();
//                 token = PLUS_PLUS;
//             }

//             // '+='
//             if(curr == '=') {
//                 lexer->increment_offset(); 
//                 lexer->increment_tok_length();

//                 token = PLUS_EQUALS;
//             } else {
//                 token = PLUS;
//             }
//             break; 
        
//         // '-'
//         case '-':
//             // The following might be removed at some point. 
//             // '--' serves no purpose for us since we don't support pointer arithmetic
//             // 
//             // '--'
//             if(curr == '-') {
//                 lexer->increment_offset();
//                 lexer->increment_tok_length();
//                 token = MINUS_MINUS;
//             }

//             // '->'
//             if(curr == '>') {
//                 lexer->increment_offset();
//                 lexer->increment_tok_length();
//                 token = RARROW;
//             }

//             // '-='
//             if(curr == '=') {
//                 lexer->increment_offset(); 
//                 lexer->increment_tok_length();

//                 token = MINUS_EQUALS;
//             } else {
//                 token = MINUS;
//             }
//             break; 

//         // '*'
//         case '*':
//             // '**'
//             if(curr == '*') {
//                 lexer->increment_offset();
//                 lexer->increment_tok_length();
//                 token = MULT_MULT;
//             }

//             // '*='
//             if(curr == '=') {
//                 lexer->increment_offset(); 
//                 lexer->increment_tok_length();

//                 token = MULT_EQUALS;
//             } else {
//                 token = MULT;
//             }
//             break;
        
//         // '/'
//         case '/':
//             // '//'
//             if(curr == '/') {
//                 lexer->increment_offset();
//                 lexer->increment_tok_length();
//                 token = SLASH_SLASH;
//             }

//             // '/='
//             if(curr == '=') {
//                 lexer->increment_offset(); 
//                 lexer->increment_tok_length();

//                 token = SLASH_EQUALS;
//             } else {
//                 token = SLASH;
//             }
//             break;
        
//         // '!'
//         case '!':
//             // '!='
//             if(curr == '=') {
//                 lexer->increment_offset(); 
//                 lexer->increment_tok_length();

//                 token = EXCLAMATION_EQUALS;
//             } else {
//                 token = EXCLAMATION;
//             }
//             break;
        
//         // '%'
//         case '%':
//             // '%%'
//             if(curr == '%') {
//                 lexer->increment_offset();
//                 lexer->increment_tok_length();
//                 token = MOD_MOD;
//             }

//             // '%='
//             if(curr == '=') {
//                 lexer->increment_offset(); 
//                 lexer->increment_tok_length();

//                 token = MOD_EQUALS;
//             } else {
//                 token = MOD;
//             }
//             break;
        
//         // '&'
//         case '&':
//             // '&&'
//             if(curr == '&') {
//                 lexer->increment_offset();
//                 lexer->increment_tok_length();
//                 token = AND_AND;
//             }

//             // '&^'
//             if(curr == '^') {
//                 lexer->increment_offset();
//                 lexer->increment_tok_length();
//                 token = AND_NOT;
//             }

//             // '&='
//             if(curr == '=') {
//                 lexer->increment_offset(); 
//                 lexer->increment_tok_length();

//                 token = AND_EQUALS;
//             } else {
//                 token = AND;
//             }
//             break;
        
//         // '|'
//         case '|':
//             // '||'
//             if(curr == '|') {
//                 lexer->increment_offset();
//                 lexer->increment_tok_length();
//                 token = OR_OR;
//             }

//             // '|='
//             if(curr == '=') {
//                 lexer->increment_offset(); 
//                 lexer->increment_tok_length();

//                 token = OR_EQUALS;
//             } else {
//                 token = OR;
//             }
//             break;
        
//         // '^'
//         case '^':
//             // '^='
//             if(curr == '=') {
//                 lexer->increment_offset(); 
//                 lexer->increment_tok_length();

//                 token = XOR_EQUALS;
//             } else {
//                 token = XOR;
//             }
//             break;
        
//         // '?'
//         case '?':
//             token = QUESTION;
//             break;

//         // '<'
//         case '<':
//             // '<='
//             if(curr == '=') {
//                 lexer->increment_offset(); 
//                 lexer->increment_tok_length();

//                 token = LESS_THAN_OR_EQUAL_TO;
//             }

//             // '<-'
//             else if(curr == '-') {
//                 lexer->increment_offset(); 
//                 lexer->increment_tok_length();

//                 token = LARROW;
//             }

//             // '<<'
//             else if(curr == '<') {
//                 lexer->increment_offset();
//                 lexer->increment_tok_length();

//                 // '<<='
//                 curr = lexer->peek_curr();
//                 if(curr == '=') {
//                     lexer->increment_offset();
//                     lexer->increment_tok_length();
//                     token = LBITSHIFT_EQUALS;
//                 } else {
//                     token = LBITSHIFT;
//                 }
//             } else {
//                 token = LESS_THAN;
//             }
//             break;
        
//         // '>'
//         case '>':
//             // '>='
//             if(curr == '=') {
//                 lexer->increment_offset(); 
//                 lexer->increment_tok_length();

//                 token = GREATER_THAN_OR_EQUAL_TO;
//             } 

//             // '>>'
//             else if(curr == '>') {
//                 lexer->increment_offset();
//                 lexer->increment_tok_length();

//                 // '>>='
//                 curr = lexer->peek_curr();
//                 if(curr == '=') {
//                     lexer->increment_offset();
//                     lexer->increment_tok_length();
//                     token = RBITSHIFT_EQUALS;
//                 } else {
//                     token = RBITSHIFT;
//                 }
//             } else {
//                 token = GREATER_THAN;
//             }
//             break;  
        
//         // '~'
//         case '~':
//             // '~='
//             if(curr == '=') {
//                 lexer->increment_offset(); 
//                 lexer->increment_tok_length();

//                 token = TILDA_EQUALS;
//             } else {
//                 token = TILDA;
//             }
//             break;
        
//         default: 
//             // printf("LexerError -> Unrecognized Token (%c)", next);
//             printf("LexerError -> Unrecognized Token");
//             abort();
//     }

//     lexer->finalize_token(token);
//     // LEXER_DEBUG("Found operator: %s", token_toString(token));
//     return token; 
// } 


// TokenType lexer_lex_separator(Lexer* lexer) {
//     lexer->reset_token(); 
//     lexer->increment_tok_length(); 

//     // Do not change the declaration order of _next_ and _curr_
//     char next = lexer->next(); 
//     char curr = lexer->peek_curr(); 
//     TokenType token = TOK_ILLEGAL; 

//     switch(next) {
//         // '.'
//         case '.':
//             // '..'
//             if(curr == '.') {
//                 lexer->increment_offset();
//                 lexer->increment_tok_length();

//                 // '...'
//                 curr = lexer->peek_curr();
//                 if(curr == '.') {
//                     lexer->increment_offset();
//                     lexer->increment_tok_length();
//                     token = ELLIPSIS;
//                 } else {
//                     token = DDOT;
//                 }
//             } else {
//                 token = DOT;
//             }
//             break;  
    
//         case ':': token = COLON; break; 
//         case ';': token = SEMICOLON; break; 
//         case ',': token = COMMA; break; 
//         case '\\': token = BACKSLASH; break; 
//         default: 
//             printf("LexerError -> Unrecognized Token");
//             abort();
//     }

//     lexer->finalize_token(token);
//     // LEXER_DEBUG("Found separator: %s", token_toString(token));
//     return token; 
// }


// TokenType lexer_lex_delimiter(Lexer* lexer) {
//     lexer->reset_token(); 
//     lexer->increment_tok_length(); 

//     // Do not change the declaration order of _next_ and _curr_
//     char next = lexer->next(); 
//     char curr = lexer->peek_curr(); 
//     TokenType token = TOK_ILLEGAL; 

//     switch(next) {       
//         case '[': token = LSQUAREBRACK; break; 
//         case ';': token = RSQUAREBRACK; break; 
//         case '{': token = LBRACE; break; 
//         case '}': token = RBRACE; break; 
//         case '(': token = LPAREN; break; 
//         case ')': token = RPAREN; break; 
//         default:  
//             printf("LexerError -> Unrecognized Token"); 
//             abort();
//     }
        
//     lexer->finalize_token(token);
//     // LEXER_DEBUG("Found delimiter: %s", token_toString(token));
//     return token; 
// }


// TokenType lexer_lex_macro(Lexer* lexer) {
//     lexer->reset_token(); 
//     lexer->increment_tok_length(); 
//     TokenType token = TOK_ILLEGAL;

//     char curr = lexer->peek_curr(); 
//     if(curr == '@')
//         token = MACRO;

//     lexer->finalize_token(token);
//     return token;
// }
