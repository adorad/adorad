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

/*
    Lexer:
    In lexer phase, the source code is decomposed into its simplest "tokens". 

    Newlines are converted to newline tokens

 * INSPIRATION: https://github.com/JHG777000/marshmallow/blob/master/src/marshmallow_parser.c

*/

// 
// ================ Lexer Stuff ================
// 
Lexer* lexer_init(const char* buffer) {
    Lexer* lexer = calloc(1, sizeof(Lexer));
    lexer->buffer = buffer; 
    lexer->buffer_capacity = strlen(buffer);
    lexer->offset = 0;
    LEXER_LOCATION_INIT;
    return lexer;
}

void lexer_print_stats(Lexer* lexer) {
    printf("    Char: %c\n", lexer->buffer[lexer->offset]);
    printf("    Offset: %d\n", lexer->offset);
    printf("    Lineno: %d\n", lexer->location.lineno);
    printf("    Colno: %d\n", lexer->location.colno);
}

// Returns the curent character in the Lexical Buffer and advances to the next element
// It does this by incrementing the buffer offset.
static inline char lexer_next(Lexer* lexer) {
    if(lexer->offset + 1 > lexer->buffer_capacity) {
        return nullchar; 
    } else {
        LEXER_INCREMENT_COLNO;
        return (char)lexer->buffer[lexer->offset++];
    }
}

// Advance `n` characters in the Lexical Buffer
static inline char lexer_next_n(Lexer* lexer, UInt32 n) {
    if(lexer->offset + n > lexer->buffer_capacity) {
        return nullchar;
    } else {
        char c;
        for(int i=0; i<=n; i++) {
            c = (char)lexer->buffer[lexer->offset];
            if(c == '\n') {
                LEXER_RESET_COLNO;
                LEXER_INCREMENT_OFFSET;
            } else {
                LEXER_INCREMENT_COLNO;
                LEXER_INCREMENT_OFFSET;
            }
        }
        return c;
    }
}