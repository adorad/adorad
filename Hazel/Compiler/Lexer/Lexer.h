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
#ifndef _HAZEL_LEXER_H
#define _HAZEL_LEXER_H

#include <Hazel/Compiler/Lexer/Location.h>
#include <Hazel/Compiler/Tokens/Tokens.h>
#include <Hazel/Core/Types.h> 
#include <Hazel/Core/String.h> 

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
inline char lexer_next(Lexer* lexer);
// Lexer peek() allows you to "look ahead" `n` characters in the Lexical buffer
// It _does not_ increment the buffer offset 
inline char lexer_peek(Lexer* lexer, int n);

// Lexer peek_curr() returns the current element in the Lexical Buffer
inline char lexer_peek_curr(Lexer* lexer);

// inline const char* buffer(Lexer* lexer) { return lexer->buffer; }
// inline UInt32 buffer_capacity(Lexer* lexer) { return lexer->buffer_capacity; }
// inline UInt32 offset(Lexer* lexer) { return lexer->offset; }
// inline Location location(Lexer* lexer) { return lexer->location; }

bool lexer_is_EOF(Lexer* lexer);
void finalize_token(Lexer* lexer, TokenType __tok);

void lexer_increment_tok_bytes(Lexer* lexer);
void lexer_decrement_tok_bytes(Lexer* lexer);
void lexer_increment_tok_length(Lexer* lexer);
void lexer_decrement_tok_length(Lexer* lexer);
void lexer_increment_lineno(Lexer* lexer);
void lexer_decrement_lineno(Lexer* lexer);
void lexer_increment_colno(Lexer* lexer);
void lexer_decrement_colno(Lexer* lexer);
void lexer_increment_offset(Lexer* lexer);
void lexer_decrement_offset(Lexer* lexer);

void lexer_set_token(Lexer* lexer, TokenType tok_type);
void lexer_set_token_value(Lexer* lexer, const char* value);
void lexer_set_token_bytes(Lexer* lexer, UInt32 bytes);
// Token lexer_extract_token(Lexer* lexer);

// Resets
void lexer_reset_lineno(Lexer* lexer);
void lexer_reset_colno(Lexer* lexer);
void lexer_reset_token(Lexer* lexer);
void lexer_reset_buffer(Lexer* lexer);
void lexer_reset_(Lexer* lexer);


// Location
void lexer_location_init(Lexer* lexer);
inline void lexer_set_lineno(Lexer* lexer, UInt32 lineno);
inline void lexer_set_colno(Lexer* lexer, UInt32 colno);
inline void lexer_set_fname(Lexer* lexer, const const char* fname);


// TokenType lexer_lex_string(Lexer* lexer); 
// TokenType lexer_lex_operator(Lexer* lexer);
// TokenType lexer_lex_separator(Lexer* lexer);
// TokenType lexer_lex_delimiter(Lexer* lexer);
// TokenType lexer_lex_macro(Lexer* lexer);
// TokenType lexer_lex_keywords(Lexer* lexer);
// TokenType lexer_lex_char(Lexer* lexer); 
// TokenType lexer_lex_digit(Lexer* lexer); 
// TokenType lexer_lex_token_id(Lexer* lexer); 

// static inline bool isBuiltinOperator(char c);
// static inline bool isIdentifier(char c);
// static inline bool isNewLine(char c);
// static inline bool isSlashComment(char c1, char c2);
// static inline bool isHashComment(char c);
// static inline bool isComment(char c1, char c2);
// static inline bool isSemicolon(char c);
// static inline bool isString(char c);
// static inline bool isMacro(char c);


#endif // _HAZEL_LEXER_H