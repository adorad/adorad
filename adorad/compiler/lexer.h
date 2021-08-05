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
#ifndef ADORAD_LEXER_H
#define ADORAD_LEXER_H

#include <adorad/core/misc.h>
#include <adorad/core/types.h>
#include <adorad/core/string.h> 
#include <adorad/core/vector.h>
#include <adorad/core/buffer.h>
#include <adorad/core/debug.h>

#include <adorad/compiler/tokens.h>
#include <adorad/compiler/location.h>

/*
    Adorad's Lexer is built in such a way that no (or negligible) memory allocations are necessary during usage. 

    In order to be able to not allocate any memory during tokenization, STRINGs and NUMBERs are just sanity checked
    but _not_ converted - it is the Parser's responsibility to perform the right conversion.

    In case of a scan error, ILLEGAL is returned and the error details can be extracted from the token itself.

    Reference: 
        1. ASCII Table: http://www.theasciicode.com.ar 
*/

// This macro defines how many tokens we initially expect in lexer->toklist. 
// When this limit is reached, we realloc using this same constant (TOKENLIST_ALLOC_CAPACITY * sizeof(Token)) bytes 
// at a time (which works out to around 0.26MB) per (re)allocation
#define TOKENLIST_ALLOC_CAPACITY    8192
// Maximum length of an individual token
#define MAX_TOKEN_LENGTH            256

typedef struct Lexer {
    Buff* buffer;       // the Lexical buffer
    UInt32 offset;      // current buffer offset (in Bytes) 
                        // offset of the curr char (no. of chars b/w the beginning of the Lexical Buffer
                        // and the curr char)

    Vec* toklist;       // list of tokens
    Location* loc;      // location of the token in the source code

    bool is_inside_str; // set to true inside a string
    int nest_level;     // used to infer if we're inside many `{}`s
} Lexer;

typedef enum Error {
    SyntaxError,
    ParseError,
} Error;

Lexer* lexer_init(char* buffer, const char* fname);
static void lexer_free(Lexer* lexer);
void lexer_error(Lexer* lexer, Error e, const char* format, ...);
// Lex the source files
static void lexer_lex(Lexer* lexer);

#endif // ADORAD_LEXER_H