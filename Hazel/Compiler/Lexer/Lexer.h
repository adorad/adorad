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

#include <Hazel/Core/Misc.h>
#include <Hazel/Core/Types.h>
#include <Hazel/Core/String.h> 
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
    const char* buffer;     // the Lexical buffer
    UInt32 buffer_capacity; // current buffer capacity (in Bytes)
    UInt32 offset;          // current buffer offset (in Bytes) 
                            // offset of the beginning of the line (no. of chars b/w the beginning of the Lexical Buffer
                            // and the beginning of the line)
                            // Sometimes called the buffer position

    Token token;            // current token
    Location location;      // Location of the source code
} Lexer;


// Constructor 
Lexer* lexer_init(const char* buffer);
// Returns the current character in the Lexical Buffer and advances to the next element.
// It does this by incrementing the buffer offset.
char lexer_next(Lexer* lexer);
// Returns the previous `n` elements in the Lexical buffer.
// This is non-destructive -- the buffer offset is not updated.
static inline char lexer_prev(Lexer* lexer, UInt32 n);
// "Look ahead" `n` characters in the Lexical buffer.
// It _does not_ increment the buffer offset.
static inline char lexer_peek(Lexer* lexer, UInt32 n);
// lexer_peek_curr() returns the current element in the Lexical Buffer.
static inline char lexer_peek_curr(Lexer* lexer);

static inline bool lexer_is_EOF(Lexer* lexer);

#ifndef LEXER_MACROS_
#define LEXER_MACROS_
    // Increment Token Bytes
    #define LEXER_INCREMENT_TOK_BYTES    ++lexer->token.tok_bytes
    // Decrement Token Bytes
    #define LEXER_DECREMENT_TOK_BYTES    --lexer->token.tok_bytes 
    // Increment Token Length
    #define LEXER_INCREMENT_TOK_LENGTH   ++lexer->token.tok_length
    // Decrement Token Length
    #define LEXER_DECREMENT_TOK_LENGTH   --lexer->token.tok_length 

    // Reset the line
    #define LEXER_RESET_LINENO           lexer->location.lineno = 0
    // Reset the column number 
    #define LEXER_RESET_COLNO            lexer->location.colno = 0

    // Increment the line number
    #define LEXER_INCREMENT_LINENO       ++lexer->location.lineno; LEXER_RESET_COLNO
    // Decrement the lineno
    #define LEXER_DECREMENT_LINENO       --lexer->location.lineno; LEXER_RESET_COLNO
    // Increment the column number
    #define LEXER_INCREMENT_COLNO        ++lexer->location.colno 
    // Decrement the colno
    #define LEXER_DECREMENT_COLNO        --lexer->location.colno

    // Increment the Lexical Buffer offset
    #define LEXER_INCREMENT_OFFSET       ++lexer->offset; LEXER_INCREMENT_COLNO
    // Decrement the Lexical Buffer offset
    #define LEXER_DECREMENT_OFFSET       --lexer->offset; LEXER_DECREMENT_COLNO

    // Reset a Lexer Token
    #define LEXER_RESET_TOKEN                                       \
        /* CHECK THIS */                                            \
        lexer->token.type = TOK_ILLEGAL;                        \
        /* TODO(jasmcaus): Verify this is accurate */               \
        lexer->token.value = lexer->buffer + lexer->offset; \
        lexer->token.location = lexer->location

    // Reset the buffer 
    #define LEXER_RESET_BUFFER        \
        lexer->buffer= "";          \
        lexer->buffer_capacity = 0

    // Reset the Lexer state
    #define LEXER_RESET               \
        lexer->buffer= "";          \
        lexer->buffer_capacity = 0; \
        lexer->offset = 0;          \
        LEXER_LOCATION_INIT

    #define LEXER_LOCATION_INIT           \
        lexer->location.lineno = 1;   \
        lexer->location.colno = 1;    \
        lexer->location.fname = ""

#endif // LEXER_MACROS_

// A List of Compiler Pragmas recorded for functions. 
// Options are used as bits in a bitmask
// These set of values are intended to be the same as the Hazel Compiler
typedef enum {
    COMPILER_NOINLINE = 1 << 0, // Do not inline
    COMPILER_NOTINHEAP = 1 << 1, // Type not in heap
} CompilerPragmas;

TokenKind lexer_error(Lexer* lexer, const char* message);

static inline bool isBuiltinOperator(char c);
static inline bool isIdentifier(char c);
static inline bool isNewLine(Lexer* lexer, char c);
static inline bool isSlashComment(char c1, char c2);
static inline bool isHashComment(char c);
static inline bool isComment(char c1, char c2);
static inline bool isSemicolon(char c);
static inline bool isString(char c);
static inline bool isMacro(char c);

TokenKind lexer_lex_comment(Lexer* lexer);
TokenKind lexer_lex_string(Lexer* lexer); 
TokenKind lexer_lex_operator(Lexer* lexer);
TokenKind lexer_lex_separator(Lexer* lexer);
TokenKind lexer_lex_delimiter(Lexer* lexer);
TokenKind lexer_lex_macro(Lexer* lexer);
TokenKind lexer_lex_keywords(Lexer* lexer);
TokenKind lexer_lex_char(Lexer* lexer); 
TokenKind lexer_lex_digit(Lexer* lexer); 
TokenKind lexer_lex_token_id(Lexer* lexer); 

#endif // _HAZEL_LEXER_H