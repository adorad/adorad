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

#include <stdio.h>
#include <string.h>

// #include <Hazel/Compiler/Lexer/Location.h>
#include <Hazel/Core/Types.h> 
#include <Hazel/Compiler/Tokens/Tokens.h>

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
Lexer* lexer_init(const char* buffer);
// lexer_next() consumes the next element in the Lexical Buffer
// It increments the buffer offset and essentially _advances_ to the next element in the buffer
static inline char lexer_next(Lexer* lexer);
// lexer_peek() allows you to "look ahead" `n` characters in the Lexical buffer
// It _does not_ increment the buffer offset 
static inline char lexer_peek(Lexer* lexer, int n);
// lexer_peek_curr() returns the current element in the Lexical Buffer
static inline char lexer_peek_curr(Lexer* lexer);
inline const char* lexer_buffer(Lexer* lexer);
inline UInt32 lexer_buffer_capacity(Lexer* lexer);
inline UInt32 lexer_offset(Lexer* lexer);

// Locations
void lexer_location_init(Lexer* lexer);
inline Location lexer_location(Lexer* lexer);


// A List of Compiler Pragmas recorded for functions. 
// Options are used as bits in a bitmask
// These set of values are intended to be the same as the Hazel Compiler
typedef enum {
    COMPILER_NOINLINE = 1 << 0, // Do not inline
    COMPILER_NOTINHEAP = 1 << 1, // Type not in heap
} CompilerPragmas;


static inline bool lexer_is_EOF(Lexer* lexer);
static inline void lexer_finalize_token(Lexer* lexer, TokenType __tok);

static inline void lexer_increment_tok_bytes(Lexer* lexer);
static inline void lexer_decrement_tok_bytes(Lexer* lexer);
static inline void lexer_increment_tok_length(Lexer* lexer);
static inline void lexer_decrement_tok_length(Lexer* lexer);
static inline void lexer_increment_lineno(Lexer* lexer);
static inline void lexer_decrement_lineno(Lexer* lexer);
static inline void lexer_increment_colno(Lexer* lexer);
static inline void lexer_decrement_colno(Lexer* lexer);
static inline void lexer_increment_offset(Lexer* lexer);
static inline void lexer_decrement_offset(Lexer* lexer);

static inline void lexer_set_token(Lexer* lexer, Token token);
static inline void lexer_set_token_value(Lexer* lexer, const char* value);
static inline void lexer_set_token_type(Lexer* lexer, TokenType tok_type);
static inline void lexer_set_token_bytes(Lexer* lexer, UInt32 bytes);
static inline Token lexer_extract_token(Lexer* lexer);

// Resets
void lexer_reset(Lexer* lexer);
void lexer_reset_token(Lexer* lexer);

TokenType lexer_error(Lexer* lexer, const char* message);

static inline bool isBuiltinOperator(char c);
static inline bool isIdentifier(char c);
static inline bool isNewLine(Lexer* lexer, char c);
static inline bool isSlashComment(char c1, char c2);
static inline bool isHashComment(char c);
static inline bool isComment(char c1, char c2);
static inline bool isSemicolon(char c);
static inline bool isString(char c);
static inline bool isMacro(char c);

TokenType lexer_lex_string(Lexer* lexer); 
TokenType lexer_lex_operator(Lexer* lexer);
TokenType lexer_lex_separator(Lexer* lexer);
TokenType lexer_lex_delimiter(Lexer* lexer);
TokenType lexer_lex_macro(Lexer* lexer);
TokenType lexer_lex_keywords(Lexer* lexer);
TokenType lexer_lex_char(Lexer* lexer); 
TokenType lexer_lex_digit(Lexer* lexer); 
TokenType lexer_lex_token_id(Lexer* lexer); 

#endif // _HAZEL_LEXER_H