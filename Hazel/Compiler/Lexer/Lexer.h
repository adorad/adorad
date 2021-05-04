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

#ifndef _HAZEL_LEXER
#define _HAZEL_LEXER 

#include <Hazel/Core/HCore.h> 
#include <Hazel/Compiler/Tokens/Tokens.h>

/*
    Hazel's Lexer is built in such a way that no (or negligible) memory allocations are necessary during usage. 
    In order to be able to not allocate any memory during tokenization, STRINGs and NUMBERs are just sanity checked but _not_
    converted - it is the responsibility of the Parser to perform the right conversion.
    In case of a scan error, ILLEGAL is returned and the error details can be extracted from the token itself.
    Reference: 
        1. ASCII Table: http://www.theasciicode.com.ar 
*/

/*
    Lexical Analysis happens as follows:
        At each token, the lexing engine 
*/
typedef struct LexerStruct {
    char* buffer;           // the Lexical buffer
    UInt32 position;        // current buffer position (in characters)
    UInt32 buffer_capacity; // current buffer capacity (in Bytes)
    UInt32 offset;          // current buffer offset (in Bytes) 
                            // offset of the beginning of the line (no. of chars b/w the beginning of the Lexical Buffer
                            // and the beginning of the line)
                            // Sometimes called the buffer position

    Token token;            // current token
    // UInt32 char_idx;        // the index of the token
    UInt32 line_no;         // the line number in the source where the token occured
    UInt32 col_no;          // the column number
    char* fname;            // the file name
} Lexer; 


/* Useful Macros for the Lexer 
    Quick note: 
        LEXER_NEXT increments the buffer offset and buffer position
        LEXER_PEEKs _do not_ increment anything - it offers an easy way to safely view an element in the buffer
*/
#define LEXER_NEXT            lexer->buffer[lexer->offset++]; \
                              LEXER_INCREMENT_COLUMN
#define LEXER_PEEK_CURR       (int)lexer->buffer[lexer->offset]
#define LEXER_PEEK_NEXT       (lexer->offset < lexer->buffer_capacity ? (int)lexer->buffer[lexer->offset+1] : 0 
#define LEXER_PEEK_NEXT2      (lexer->offset+1 < lexer->buffer_capacity ? (int)lexer->buffer[lexer->offset+2] : 0 

#define LEXER_RESET_LINE      lexer->line_no = 1
#define LEXER_RESET_COLUMN    lexer->col_no  = 1
#define LEXER_IS_EOF          lexer->offset >= lexer->buffer_capacity

#define LEXER_INCREMENT_LINE     ++lexer->line_no; LEXER_RESET_COLUMN
#define LEXER_INCREMENT_COLUMN   ++lexer->col_no; 
#define LEXER_DECREMENT_LINE     --lexer->line_no; LEXER_RESET_COLUMN
#define LEXER_DECREMENT_COLUMN   --lexer->col_no; 
#define LEXER_INCREMENT_OFFSET   ++lexer->offset; LEXER_INCREMENT_COLUMN
#define LEXER_DECREMENT_OFFSET   --lexer->offset; LEXER_DECREMENT_COLUMN
// #define LEXER_INCREMENT_POSITION ++lexer->position;
// #define LEXER_DECREMENT_POSITION --lexer->position;

// #define LEXER_INCREMENT_OFFSET_AND_POSITION       LEXER_INCREMENT_OFFSET; LEXER_INCREMENT_POSITION

// Useful Macros for Tokens
#define TOKEN_RESET         lexer->token = NO_TOKEN; \
                            lexer->token.position = lexer->position; \
                            lexer->token.value = lexer->buffer + lexer->offset; \
                            lexer->token.line_no = lexer->line_no; \
                            lexer->token.col_no = lexer->col_no;

#define TOKEN_FINALIZE(__t)          lexer->token.type = __t; \
                                     lexer->token.fname = lexer->fname
#define TOKEN_INCREMENT_TOKENBYTES   ++lexer->token.bytes
#define TOKEN_DECREMENT_TOKENBYTES   --lexer->token.bytes
#define TOKEN_INCREMENT_TOKENLENGTH  ++lexer->token.length
#define TOKEN_DECREMENT_TOKENLENGTH  -- lexer->token.length


// Lexer* lexer_init(const char* buffer); 
void lexer_free(Lexer* lexer); 

Token* lexer_get_next_token(Lexer* lexer); 
Token* lexer_advance_with_token(Lexer* lexer, int type); 

Lexer* lexer_init(const char* buffer, char* fname);
void lexer_advance(Lexer* lexer); 
void lexer_expect_char(Lexer* lexer, char c);
void lexer_skip_whitespace(Lexer* lexer); 
void lexer_skip_inline_comment(Lexer* lexer);
void lexer_skip_block_comment(Lexer* lexer);

Token* lexer_lex_string(Lexer* lexer); 
Token* lexer_lex_operator(Lexer* lexer);
Token* lexer_lex_separator(Lexer* lexer);
Token* lexer_lex_delimiter(Lexer* lexer);
Token* lexer_lex_macro(Lexer* lexer);
Token* lexer_lex_keywords(Lexer* lexer);
Token* lexer_lex_char(Lexer* lexer); 
Token* lexer_lex_digit(Lexer* lexer); 
Token* lexer_lex_token_id(Lexer* lexer); 

char* lexer_lex_charstr(Lexer* lexer);

static inline bool isBuiltinOperator(char c);
static inline bool isIdentifier(char c);
static inline bool isNewLine(Lexer* lexer, char c);
static inline bool isSlashComment(char c1, char c2);
static inline bool isHashComment(char c);
static inline bool isComment(char c1, char c2);
static inline bool isSemicolon(char c);
static inline bool isString(char c);
static inline bool isMacro(char c);


#endif // _HAZEL_LEXER