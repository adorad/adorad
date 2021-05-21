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

static void lexer_print_stats(Lexer* lexer) {
    printf("    Char: %c\n", lexer->buffer[lexer->offset]);
    printf("    Offset: %d\n", lexer->offset);
    printf("    Lineno: %d\n", lexer->location.lineno);
    printf("    Colno: %d\n", lexer->location.colno);
}

// Returns the curent character in the Lexical Buffer and advances to the next element
// It does this by incrementing the buffer offset.
// This will ideally _never_ return newline characters
static inline char lexer_next(Lexer* lexer) {
    if(lexer->offset + 1 > lexer->buffer_capacity) {
        return nullchar; 
    } else {
        // Get current character
        char c = lexer->buffer[lexer->offset];

        // If current character is a newline, are there more newlines?
        // If so, skip them
        while(c == '\n') {
            c = lexer->buffer[++lexer->offset];
            LEXER_RESET_COLNO;
        }
        // When newlines are hit, colno will always have to be 
        LEXER_INCREMENT_COLNO;
        // Return the previous char
        return (char)lexer->buffer[lexer->offset];
    }
}

// Advance `n` characters in the Lexical Buffer
// inline char lexer_next_n(Lexer* lexer, UInt32 n) {
static inline char lexer_next_n(Lexer* lexer, UInt32 n) {
    if(lexer->offset + n > lexer->buffer_capacity) {
        return nullchar;
    } else {
        lexer->location.colno += n;
        lexer->offset += n;
        return (char)lexer->buffer[lexer->offset];
    }
}