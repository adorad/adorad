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

#include <Hazel/Compiler/Lexer/Lexer.h>

Lexer* lexer_init(const char* buffer) {
    Lexer* lexer = calloc(1, sizeof(Lexer));
    lexer->buffer = buffer; 
    lexer->buffer_capacity = strlen(buffer);
    lexer->offset = 0;
    LEXER_LOCATION_INIT;
    return lexer;
}

static void lexer_print_stats(Lexer* lexer) {
    printf("    Char: %c\n", lexer->buffer[lexer->offset]);
    printf("    Offset: %d\n", lexer->offset);
    printf("    Lineno: %d\n", lexer->location.lineno);
    printf("    Colno: %d\n", lexer->location.colno);
}

// Returns the curent character in the Lexical Buffer and advances to the next element
// It does this by incrementing the buffer offset.
static inline char lexer_advance(Lexer* lexer) {
    if(lexer->offset >= lexer->buffer_capacity) {
        return nullchar; 
    } else {
        LEXER_INCREMENT_COLNO;
        return (char)lexer->buffer[lexer->offset++];
    }
}

// Advance `n` characters in the Lexical Buffer
static inline char lexer_advance_n(Lexer* lexer, UInt32 n) {
    // The '>=' is here because offset and buffer_capacity are off by 1 (0-index vs 1-index respectively)
    if(lexer->offset + n >= lexer->buffer_capacity) {
        return nullchar;
    } else {
        lexer->location.colno += n;
        lexer->offset += n;
        return (char)lexer->buffer[lexer->offset];
    }
}

// Returns the previous `n` elements in the Lexical buffer.
// This is non-destructive -- the buffer offset is not updated.
static inline char lexer_prev(Lexer* lexer, UInt32 n) {
    if(lexer->offset == 0) {
        return nullchar;
    } else {
        return (char)lexer->buffer[lexer->offset-1];
    }
}

// Returns the current element in the Lexical Buffer.
static inline char lexer_peek(Lexer* lexer) {
    return (char)lexer->buffer[lexer->offset];
}

// "Look ahead" `n` characters in the Lexical buffer.
// It _does not_ increment the buffer offset.
static inline char lexer_peek_n(Lexer* lexer, UInt32 n) {
    if(lexer->offset + n >= lexer->buffer_capacity) {
        return nullchar;
    } else {
        return (char)lexer->buffer[lexer->offset+1];
    }
}
