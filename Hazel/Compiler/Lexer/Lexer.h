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
        this->location.colno = 0; 
        this->location.lineno = 0; 
        this->location.fname = fname; 
    }

    // Lexer next() increments the buffer offset and essentially _advances_ to the next element in the buffer
    inline char next() {
        ++this->location.colno;
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

    // Check if the current Lexer state is at EOF
    inline bool is_EOF() {
        return this->offset >= this->buffer_capacity;
    }

    // Reset the line
    void reset_lineno() {
        this->location.lineno = 0;
    }

    // Reset the column number 
    void reset_colno() {
        this->location.colno = 0; 
    }

    // Reset a Lexer Token
    void reset_token() {
        this->token.reset_();
        // TODO(jasmcaus): Verify this is accurate
        this->token.value = this->buffer[this->offset]; 
        this->token.location = this->location;
    }

    // Finalize a Token
    void finalize_token(TokenType __tok) {
        this->token.type = __tok; 
        this->token.location.fname = this->location.fname;
    }

    // Extract a Token 
    inline Token extract_token() { return this->token; }

    // Set token type
    void set_token(TokenType tok_type) { this->token.type = tok_type;  }

    // Set token value 
    void set_token_value(std::string value) { this->token.value = value;  }

    // Set token bytes 
    void set_token_bytes(UInt32 bytes) { this->token.tok_bytes = bytes;  }

    // Increment Token Bytes
    void increment_tok_bytes() { ++this->token.tok_bytes; }

    // Decrement Token Bytes
    void decrement_tok_bytes() { --this->token.tok_bytes; }

    // Increment Token Length
    void increment_tok_length() { ++this->token.tok_length; }

    // Decrement Token Length
    void decrement_tok_length() { --this->token.tok_length; }

    // Increment the line number
    void increment_lineno() {
        ++this->location.lineno; 
        this->reset_colno();
    }

    // Decrement the lineno
    void decrement_lineno() {
        --this->location.lineno; 
        this->reset_colno();
    }

    // Increment the column number
    void increment_colno() {
        ++this->location.colno; 
    }

    // Decrement the colno
    void decrement_colno() {
        --this->location.colno; 
    }

    // Increment the Lexical Buffer offset
    void increment_offset() {
        ++this->offset; 
        this->increment_colno();
    }

    // Decrement the Lexical Buffer offset
    void decrement_offset() {
        --this->offset; 
        this->decrement_colno();
    }

    // Reset the buffer 
    void reset_buffer() {
        this->buffer = ""; 
        this->buffer_capacity = 0;
    }

    // Reset the Lexer state
    void reset_() {
        this->buffer = ""; 
        this->buffer_capacity = 0;
        this->offset = 0; 
        this->location.colno = 0; 
        this->location.lineno = 0; 
        this->location.fname = ""; 
    }


protected:
    std::string buffer;     // the Lexical buffer
    UInt32 buffer_capacity; // current buffer capacity (in Bytes)
    UInt32 offset;          // current buffer offset (in Bytes) 
                            // offset of the beginning of the line (no. of chars b/w the beginning of the Lexical Buffer
                            // and the beginning of the line)
                            // Sometimes called the buffer position

    Token token;            // current token
    Location location;      // Location of the source code
}; // class Lexer


// A List of Compiler Pragmas recorded for functions. 
// Options are used as bits in a bitmask
// These set of values are intended to be the same as the Hazel Compiler
typedef enum {
    COMPILER_NOINLINE = 1 << 0, // Do not inline
    COMPILER_NOTINHEAP = 1 << 1, // Type not in heap
} CompilerPragmas;


Token* lexer_get_next_token(Lexer* lexer); 
Token* lexer_advance_with_token(Lexer* lexer, int type); 

Lexer* lexer_init(const char* buffer, char* fname);
void lexer_advance(Lexer* lexer); 
void lexer_expect_char(Lexer* lexer, char c);
void lexer_skip_whitespace(Lexer* lexer); 
void lexer_skip_inline_comment(Lexer* lexer);
void lexer_skip_block_comment(Lexer* lexer);

TokenType lexer_lex_string(Lexer* lexer); 
TokenType lexer_lex_operator(Lexer* lexer);
TokenType lexer_lex_separator(Lexer* lexer);
TokenType lexer_lex_delimiter(Lexer* lexer);
TokenType lexer_lex_macro(Lexer* lexer);
TokenType lexer_lex_keywords(Lexer* lexer);
TokenType lexer_lex_char(Lexer* lexer); 
TokenType lexer_lex_digit(Lexer* lexer); 
TokenType lexer_lex_token_id(Lexer* lexer); 

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