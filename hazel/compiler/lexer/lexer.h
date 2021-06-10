/*
_ _    _           ______   _______        
| |  | |    /\    /___  /   |  ____|| |    
| |__| |   /  \      / /    | |__   | |       Hazel - The Fast, Expressive * Elegant Programming Language
|  __  |  / /\ \    / /     |  __|  | |       Languages: C, C++, and Assembly
| |  | | / ____ \  / /___   | |____ | |____   https://github.com/HazelLang/hazel/
|_|_ |_|/_/    \_\/_______\ |______|_\______|

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/
#ifndef _HAZEL_LEXER_H
#define _HAZEL_LEXER_H

#include <stdio.h>
#include <string.h>

#include <hazel/core/misc.h>
#include <hazel/core/types.h>
#include <hazel/core/string.h> 
#include <hazel/compiler/tokens/tokens.h>

/*
    Hazel's Lexer is built in such a way that no (or negligible) memory allocations are necessary during usage. 

    In order to be able to not allocate any memory during tokenization, STRINGs and NUMBERs are just sanity checked
    but _not_ converted - it is the Parser's responsibility to perform the right conversion.

    In case of a scan error, ILLEGAL is returned and the error details can be extracted from the token itself.

    Reference: 
        1. ASCII Table: http://www.theasciicode.com.ar 
*/
typedef struct Lexer {
    const char* buffer;     // the Lexical buffer
    UInt32 buffer_capacity; // current buffer capacity (in Bytes)
    UInt32 offset;          // current buffer offset (in Bytes) 
                            // offset of the curr char (no. of chars b/w the beginning of the Lexical Buffer
                            // and the curr char)

    Token token;            // current token
    Token* tokenList;       // list of tokens
    UInt64 tokenList_cap;   // allocated capacity of `tokenList`(changes whenever we run out of memory and realloc)
    UInt64 num_tokens;      // number of tokens in `tokenList`. This is used when appending tokens to `tokenList`
    UInt32 lineno;          // the line number in the source where the token occured
    UInt32 colno;           // the column number
    const char* fname;      // /path/to/file.hzl

    bool is_inside_str;     // set to true inside a string
    int nest_level;         // used to infer if we're inside many `{}`s
} Lexer;

// This macro defines how many tokens we initially expect in Lexer->tokenList. 
// When this limit is reached, we realloc using this same constant
#define TOKENLIST_ALLOC_SIZE   4096
// Maximum length of an individual token
#define MAX_TOKEN_SIZE         250

#ifndef LEXER_MACROS_
#define LEXER_MACROS_
    // Get current character in the Lexcial buffer
    // NB: This does not increase the offset
    #define LEXER_CURR_CHAR              lexer->buffer[lexer->offset]
    // Increment Token Length
    #define LEXER_INCREMENT_TOK_LENGTH   ++lexer->token.tok_length
    // Decrement Token Length
    #define LEXER_DECREMENT_TOK_LENGTH   --lexer->token.tok_length 

    // Reset the line
    #define LEXER_RESET_LINENO           lexer->lineno = 0
    // Reset the column number 
    #define LEXER_RESET_COLNO            lexer->colno = 0

    // Increment the line number
    #define LEXER_INCREMENT_LINENO       ++lexer->lineno; LEXER_RESET_COLNO
    // Decrement the lineno
    #define LEXER_DECREMENT_LINENO       --lexer->lineno; LEXER_RESET_COLNO
    // Increment the column number
    #define LEXER_INCREMENT_COLNO        ++lexer->colno 
    // Decrement the colno
    #define LEXER_DECREMENT_COLNO        --lexer->colno

    // Increment the Lexical Buffer offset
    #define LEXER_INCREMENT_OFFSET       ++lexer->offset; LEXER_INCREMENT_COLNO
    // Decrement the Lexical Buffer offset
    #define LEXER_DECREMENT_OFFSET       --lexer->offset; LEXER_DECREMENT_COLNO

    // Increment the Lexical Buffer offset without affecting `colno`
    #define LEXER_INCREMENT_OFFSET_ONLY       ++lexer->offset;
    // Decrement the Lexical Buffer offset without affecting `colno`
    #define LEXER_DECREMENT_OFFSET_ONLY       --lexer->offset;

    // Reset a Lexer Token
    #define LEXER_RESET_TOKEN                                       \
        /* CHECK THIS */                                            \
        lexer->token.type = TOK_ILLEGAL;                            \
        /* TODO(jasmcaus): Verify this is accurate */               \
        lexer->token.value = lexer->buffer + lexer->offset;         \
        lexer->token.location = lexer->location

    // Reset the buffer 
    #define LEXER_RESET_BUFFER        \
        lexer->buffer= "";            \
        lexer->buffer_capacity = 0

    // Reset the Lexer state
    #define LEXER_RESET               \
        lexer->buffer= "";            \
        lexer->buffer_capacity = 0;   \
        lexer->offset = 0;            \
        lexer->lineno = 1;            \
        lexer->colno = 1;             \
        lexer->fname = ""

#endif // LEXER_MACROS_

Lexer* lexer_init(const char* buffer);
static void lexer_tokenlist_append(Lexer* lexer, Token* tk);
static void lexer_free(Lexer* lexer);
static void lexer_print_stats(Lexer* lexer);

// Returns the current character in the Lexical Buffer and advances to the next element.
// It does this by incrementing the buffer offset.
static inline char lexer_advance(Lexer* lexer);
// Advance `n` characters in the Lexical Buffer
static inline char lexer_advance_n(Lexer* lexer, UInt32 n);

// Returns the previous `n` elements in the Lexical buffer.
// This is non-destructive -- the buffer offset is not updated.
static inline char lexer_prev(Lexer* lexer, UInt32 n);

// Returns the current element in the Lexical Buffer.
static inline char lexer_peek(Lexer* lexer);
// "Look ahead" `n` characters in the Lexical buffer.
// It _does not_ increment the buffer offset.
static inline char lexer_peek_n(Lexer* lexer, UInt32 n);

static inline bool lexer_is_EOF(Lexer* lexer);

void lexer_error(Lexer* lexer, const char* format, ...);

// Make a token
static void lexer_maketoken(Lexer* lexer, TokenKind kind);

// Scan a comment (single line)
static inline void lexer_lex_sl_comment(Lexer* lexer);
// Scan a comment (multi line)
static inline void lexer_lex_ml_comment(Lexer* lexer);
// Scan a character
static inline char lexer_lex_char(Lexer* lexer);
// Scan an escape char
static inline char lexer_lex_esc_char(Lexer* lexer);
// Scan an identifier
static inline void lexer_lex_identifier(Lexer* lexer);
// Scan a digit
static inline void lexer_lex_number(Lexer* lexer);
// Lex the Source files
static void lexer_lex(Lexer* lexer);

#endif // _HAZEL_LEXER_H