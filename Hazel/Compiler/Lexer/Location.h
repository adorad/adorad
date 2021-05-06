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
#ifndef _HAZEL_LEXER_LOCATION_H
#define _HAZEL_LEXER_LOCATION_H

#include <Hazel/Core/Types.h>
#include <Hazel/Compiler/Lexer/Lexer.h>


// Location holds information about a location in a source file
// Includes the colno, lineno, fname...
typedef struct Location {
    UInt32 lineno__;     // the line number in the source where the token occured
    UInt32 colno__;      // the column number
    const char* fname__; // the file name
} Location;


void lexer_location_init(Lexer* lexer) {
    lexer->location__.lineno__ = 0; 
    lexer->location__.colno__ = 0; 
    lexer->location__.fname__ = "";
}

void token_location_init(Token* token) {
    token->location__.lineno__ = 0; 
    token->location__.colno__ = 0; 
    token->location__.fname__ = "";
}

inline void lexer_set_lineno(Lexer* lexer, UInt32 lineno) { lexer->location__.lineno__ = lineno; }
inline void lexer_set_colno(Lexer* lexer, UInt32 colno) { lexer->location__.colno__ = colno; }
inline void lexer_set_fname(Lexer* lexer, const char* fname) { lexer->location__.fname__ = fname; }
inline UInt32 lexer_lineno(Lexer* lexer) { return lexer->location__.lineno__; }
inline UInt32 lexer_colno(Lexer* lexer) { return lexer->location__.colno__; }
inline const char* fname(Lexer* lexer) { return lexer->location__.fname__; }

// Reset the line
void lexer_reset_lineno(Lexer* lexer) { lexer->location__.lineno__ = 0; }
// Reset the column number 
void lexer_reset_colno(Lexer* lexer) { lexer->location__.colno__ = 0; }

#endif // _HAZEL_LEXER_LOCATION_H