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
    lexer->buffer__ = buffer; 
    lexer->buffer_capacity__ = strlen(buffer);
    lexer->offset__ = 0;
    LEXER_LOCATION_INIT;
    return lexer;
}

// Returns the curent character in the Lexical Buffer and advances to the next element
// It does this by incrementing the buffer offset
// Handles new line characters
// inline char lexer_next(Lexer* lexer) {
char lexer_next(Lexer* lexer) {
    if(lexer->offset__ + 1 > lexer->buffer_capacity__) {
        return nullchar; 
    } else {
        LEXER_INCREMENT_COLNO;
        return (char)lexer->buffer__[lexer->offset__++];
    }
}

// Advance `n` characters in the Lexical Buffer
// inline char lexer_next_n(Lexer* lexer, UInt32 n) {
char lexer_next_n(Lexer* lexer, UInt32 n) {
    if(lexer->offset__ + n > lexer->buffer_capacity__) {
        return nullchar;
    } else {
        lexer->location__.colno__ += n;
        lexer->offset__ += n;
        return (char)lexer->buffer__[lexer->offset__];
    }
}