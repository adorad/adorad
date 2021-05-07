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
#ifndef _HAZEL_LEXER_LOCATION_H
#define _HAZEL_LEXER_LOCATION_H

#include <Hazel/Core/Types.h>

// Location holds information about a location in a source file
// Includes the colno, lineno, fname...
typedef struct Location {
    UInt32 lineno;          // the line number in the source where the token occured
    UInt32 colno;           // the column number
    const char* fname;      // the file name
} Location;

#endif // _HAZEL_LEXER_LOCATION_H