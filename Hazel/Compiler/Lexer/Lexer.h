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

#include <string>
#include <optional>
#include <Hazel/Compiler/Lexer/Lexer.h>
#include <Hazel/Compiler/Lexer/Location.h>
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
class Lexer {
public:
    // Default constructor 
    Lexer() {
        reset_();
    }

    // Constructor 
    Lexer(std::string buffer, std::string fname) {
        this->buffer = buffer; 
        this->buffer_capacity = buffer.length();
        this->offset = 0; 
        this->location_.reset_();
    }

    // Lexer next() increments the buffer offset and essentially _advances_ to the next element in the buffer
    inline char next() {
        ++this->location_.colno;
        return (char)this->buffer[this->offset++];
    }

    // Lexer peek() allows you to "look ahead" `n` characters in the Lexical buffer
    // It _does not_ increment the buffer offset 
    inline char peek(int n) {
        if(this->offset + (n-1) < this->buffer_capacity) {
            return (char)this->buffer[this->offset + n];
        } else {
            return 0;
        }
    }

    // Lexer peek_curr() returns the current element in the Lexical Buffer
    inline char peek_curr() {
        return (char)this->buffer[this->offset];
    }

    inline bool is_EOF();
    void reset_lineno();
    void reset_colno();
    void reset_token();
    void finalize_token(TokenType __tok);
    inline Token extract_token();
    void set_token(TokenType tok_type);
    void set_token_value(std::string value);
    void set_token_bytes(UInt32 bytes);
    void increment_tok_bytes();
    void decrement_tok_bytes();
    void increment_tok_length();
    void decrement_tok_length();
    void increment_lineno();
    void decrement_lineno();
    void increment_colno();
    void decrement_colno();
    void increment_offset();
    void decrement_offset();
    void reset_buffer();
    void reset_();


protected:
    std::string buffer;     // the Lexical buffer
    UInt32 buffer_capacity; // current buffer capacity (in Bytes)
    UInt32 offset;          // current buffer offset (in Bytes) 
                            // offset of the beginning of the line (no. of chars b/w the beginning of the Lexical Buffer
                            // and the beginning of the line)
                            // Sometimes called the buffer position

    Token token;            // current token
    Location location_;    // Location of the source code
}; // class Lexer


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


#endif // _HAZEL_LEXER