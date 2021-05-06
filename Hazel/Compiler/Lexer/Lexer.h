/*
_ _    _           ______   _______        
| |  | |    /\    /___  /   |  ____|| |    
| |__| |   /  \      / /    | |__   | |       Hazel - The Fast, Expressive * Elegant Programming Language
|  __  |  / /\ \    / /     |  __|  | |       Languages: C, C++, and Assembly
| |  | | / ____ \  / /___   | |____ | |____   https://github.com/HazelLang/Hazel/
|_|_ |_|/_/    \_\/_______\ |______|_\______|

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/
#ifndef _HAZEL_LEXER_H
#define _HAZEL_LEXER_H

#include <string.h>

#include <Hazel/Compiler/Lexer/Location.h>
#include <Hazel/Compiler/Tokens/Tokens.h>
#include <Hazel/Core/Types.h> 

/*
    Hazel's Lexer is built in such a way that no (or negligible) memory allocations are necessary during usage. 

    In order to be able to not allocate any memory during tokenization, STRINGs and NUMBERs are just sanity checked but _not_
    converted - it is the responsibility of the Parser to perform the right conversion.

    In case of a scan error, ILLEGAL is returned and the error details can be extracted from the token itself.

    Reference: 
        1. ASCII Table: http://www.theasciicode.com.ar 
*/
typedef struct Lexer {
    const char* buffer__;     // the Lexical buffer
    UInt32 buffer_capacity__; // current buffer capacity (in Bytes)
    UInt32 offset__;          // current buffer offset (in Bytes) 
                              // offset of the beginning of the line (no. of chars b/w the beginning of the Lexical Buffer
                              // and the beginning of the line)
                              // Sometimes called the buffer position

    Token token__;            // current token
    Location location__;      // Location of the source code
} Lexer;


// Constructor 
Lexer* lexer_init(const char* buffer) {
    Lexer* lexer = calloc(1, sizeof(Lexer));
    lexer->buffer__ = buffer; 
    lexer->buffer_capacity__ = strlen(buffer);
    lexer->offset__ = 0;
    lexer_location_init(lexer);

}

// lexer_next() consumes the next element in the Lexical Buffer
// It increments the buffer offset and essentially _advances_ to the next element in the buffer
inline char lexer_next(Lexer* lexer) {
    lexer_increment_colno(lexer);
    return (char)lexer->buffer__[lexer->offset__++];
}

// lexer_peek() allows you to "look ahead" `n` characters in the Lexical buffer
// It _does not_ increment the buffer offset 
inline char lexer_peek(Lexer* lexer, int n) {
    if(lexer->offset__ + (n-1) < lexer->buffer_capacity__) {
        return (char)lexer->buffer__[lexer->offset__ + n];
    } else {
        return 0; // corresponds to TOK_ILLEGAL
    }
}

// lexer_peek_curr() returns the current element in the Lexical Buffer
inline char lexer_peek_curr(Lexer* lexer) {
    return (char)lexer->buffer__[lexer->offset__];
}

inline const char* lexer_buffer(Lexer* lexer) { 
    return lexer->buffer__; 
}
inline UInt32 lexer_buffer_capacity(Lexer* lexer) { 
    return lexer->buffer_capacity__; 
}
inline UInt32 lexer_offset(Lexer* lexer) { 
    return lexer->offset__; 
}
inline Location lexer_location(Lexer* lexer) { 
    return lexer->location__; 
}

inline bool lexer_is_EOF(Lexer* lexer);
inline void lexer_finalize_token(TokenType __tok);

inline void lexer_increment_tok_bytes(Lexer* lexer);
inline void lexer_decrement_tok_bytes(Lexer* lexer);
inline void lexer_increment_tok_length(Lexer* lexer);
inline void lexer_decrement_tok_length(Lexer* lexer);
inline void lexer_increment_lineno(Lexer* lexer);
inline void lexer_decrement_lineno(Lexer* lexer);
inline void lexer_increment_colno(Lexer* lexer);
inline void lexer_decrement_colno(Lexer* lexer);
inline void lexer_increment_offset(Lexer* lexer);
inline void lexer_decrement_offset(Lexer* lexer);

inline void lexer_set_token(Lexer* lexer, Token token);
inline void lexer_set_token_value(Lexer* lexer, const char* value);
inline void lexer_set_token_type(Lexer* lexer, TokenType tok_type);
inline void lexer_set_token_bytes(Lexer* lexer, UInt32 bytes);
inline Token lexer_extract_token(Lexer* lexer);

// Resets
void lexer_reset_(Lexer* lexer);


// A List of Compiler Pragmas recorded for functions. 
// Options are used as bits in a bitmask
// These set of values are intended to be the same as the Hazel Compiler
typedef enum {
    COMPILER_NOINLINE = 1 << 0, // Do not inline
    COMPILER_NOTINHEAP = 1 << 1, // Type not in heap
} CompilerPragmas;


TokenType lexer_lex_string(Lexer* lexer); 
TokenType lexer_lex_operator(Lexer* lexer);
TokenType lexer_lex_separator(Lexer* lexer);
TokenType lexer_lex_delimiter(Lexer* lexer);
TokenType lexer_lex_macro(Lexer* lexer);
TokenType lexer_lex_keywords(Lexer* lexer);
TokenType lexer_lex_char(Lexer* lexer); 
TokenType lexer_lex_digit(Lexer* lexer); 
TokenType lexer_lex_token_id(Lexer* lexer); 

static inline bool isBuiltinOperator(char c);
static inline bool isIdentifier(char c);
static inline bool isNewLine(Lexer* lexer, char c);
static inline bool isSlashComment(char c1, char c2);
static inline bool isHashComment(char c);
static inline bool isComment(char c1, char c2);
static inline bool isSemicolon(char c);
static inline bool isString(char c);
static inline bool isMacro(char c);



#endif // _HAZEL_LEXER_H