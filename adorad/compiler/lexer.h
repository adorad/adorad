/*
          _____   ____  _____            _____  
    /\   |  __ \ / __ \|  __ \     /\   |  __ \ 
   /  \  | |  | | |  | | |__) |   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /   / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \  / ____ \| |__| | https://github.com/adorad/adorad/
/_/    \_\_____/ \____/|_|  \_\/_/    \_\_____/ 

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <@jasmcaus>
*/
#ifndef HAZEL_LEXER_H
#define HAZEL_LEXER_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <adorad/core/misc.h>
#include <adorad/core/types.h>
#include <adorad/core/string.h> 
#include <adorad/core/vector.h>
#include <adorad/core/buffer.h>
#include <adorad/core/debug.h>

#include <adorad/compiler/tokens.h>

/*
    Adorad's Lexer is built in such a way that no (or negligible) memory allocations are necessary during usage. 

    In order to be able to not allocate any memory during tokenization, STRINGs and NUMBERs are just sanity checked
    but _not_ converted - it is the Parser's responsibility to perform the right conversion.

    In case of a scan error, ILLEGAL is returned and the error details can be extracted from the token itself.

    Reference: 
        1. ASCII Table: http://www.theasciicode.com.ar 
*/

// This macro defines how many tokens we initially expect in lexer->tokenList. 
// When this limit is reached, we realloc using this same constant (TOKENLIST_ALLOC_CAPACITY * sizeof(Token)) bytes 
// at a time (which works out to around 0.26MB) per (re)allocation
#define TOKENLIST_ALLOC_CAPACITY    8192
// Maximum length of an individual token
#define MAX_TOKEN_LENGTH            256

typedef struct Lexer {
    const cstlBuffer* buffer;   // the Lexical buffer
    UInt32 offset;              // current buffer offset (in Bytes) 
                                // offset of the curr char (no. of chars b/w the beginning of the Lexical Buffer
                                // and the curr char)

    cstlVector* tokenList;      // list of tokens
    UInt32 lineno;              // the line number in the source where the token occured
    UInt32 colno;               // the column number
    const char* fname;          // /path/to/file.ad

    bool is_inside_str;         // set to true inside a string
    int nest_level;             // used to infer if we're inside many `{}`s
} Lexer;


#ifndef LEXER_MACROS_
#define LEXER_MACROS_
    // Get the current character in the Lexical buffer
    // NB: This does not increase the offset
    #define LEXER_CURR_CHAR                   lexer->buffer->at(lexer->buffer, lexer->offset)

    // Reset the line
    #define LEXER_RESET_LINENO                lexer->lineno = 0
    // Reset the column number 
    #define LEXER_RESET_COLNO                 lexer->colno = 0

    // Increment the line number
    #define LEXER_INCREMENT_LINENO            ++lexer->lineno; LEXER_RESET_COLNO
    // Decrement the lineno
    #define LEXER_DECREMENT_LINENO            --lexer->lineno; LEXER_RESET_COLNO
    // Increment the column number
    #define LEXER_INCREMENT_COLNO             ++lexer->colno 
    // Decrement the colno
    #define LEXER_DECREMENT_COLNO             --lexer->colno

    // Increment the Lexical Buffer offset
    #define LEXER_INCREMENT_OFFSET            ++lexer->offset; LEXER_INCREMENT_COLNO
    // Decrement the Lexical Buffer offset
    #define LEXER_DECREMENT_OFFSET            --lexer->offset; LEXER_DECREMENT_COLNO

    // Increment the Lexical Buffer offset without affecting `colno`
    #define LEXER_INCREMENT_OFFSET_ONLY       ++lexer->offset;
    // Decrement the Lexical Buffer offset without affecting `colno`
    #define LEXER_DECREMENT_OFFSET_ONLY       --lexer->offset;

    // Reset the buffer 
    #define LEXER_RESET_BUFFER              \
        lexer->buffer->free(lexer->buffer)

    // Reset the Lexer state
    #define LEXER_RESET                     \
        lexer->buffer->free(lexer->buffer); \
        lexer->offset = 0;                  \
        lexer->lineno = 1;                  \
        lexer->colno = 1;                   \
        lexer->fname = ""

#endif // LEXER_MACROS_

Lexer* lexer_init(const char* buffer, const char* fname);
static void lexer_tokenlist_append(Lexer* lexer, Token* tk);
static void lexer_free(Lexer* lexer);

// Returns the current character in the Lexical Buffer and advances to the next element.
// It does this by incrementing the buffer offset.
static inline char lexer_advance(Lexer* lexer);
// Advance `n` characters in the Lexical Buffer
static inline char lexer_advancen(Lexer* lexer, UInt32 n);

// Returns the previous element in the Lexical buffer.
// This is non-destructive --> the buffer offset is not updated.
static inline char lexer_prev(Lexer* lexer);
// Returns the previous `n` elements in the Lexical buffer.
// This is non-destructive -- the buffer offset is not updated.
static inline char lexer_prevn(Lexer* lexer, UInt32 n);

// Returns the current element in the Lexical Buffer.
static inline char lexer_peek(Lexer* lexer);
// "Look ahead" `n` characters in the Lexical buffer.
// It _does not_ increment the buffer offset.
static inline char lexer_peekn(Lexer* lexer, UInt32 n);

void lexer_error(Lexer* lexer, const char* format, ...);

// Make a token
static void lexer_maketoken(Lexer* lexer, TokenKind kind, char* value, UInt32 offset, UInt32 lineno, UInt32 colno);

// Scan a comment (single line)
static inline void lexer_lex_sl_comment(Lexer* lexer);
// Scan a comment (multi line)
static inline void lexer_lex_ml_comment(Lexer* lexer);
// Scan a character
static inline void lexer_lex_char(Lexer* lexer);
// Scan an escape char
static inline void lexer_lex_esc_char(Lexer* lexer);
// Scan a macro (begins with `@`)
static inline void lexer_lex_macro(Lexer* lexer);
// Scan a string
static inline void lexer_lex_string(Lexer* lexer);
// Returns whether `value` is a keyword or an identifier
static inline TokenKind lexer_is_keyword_or_identifier(char* value);
// Scan an identifier
static inline void lexer_lex_identifier(Lexer* lexer);
// Scan a digit
static inline void lexer_lex_digit(Lexer* lexer);
// Lex the Source files
static void lexer_lex(Lexer* lexer);

#endif // HAZEL_LEXER_H