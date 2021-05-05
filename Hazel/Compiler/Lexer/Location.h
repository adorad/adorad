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
#ifndef _HAZEL_LEXER_LOCATION
#define _HAZEL_LEXER_LOCATION

#include <string> 
#include <Hazel/Core/HCore.h>

// Location holds information about a location in a source file
// Includes the colno, lineno, fname...
struct Location {
public:
    Location& operator=(const Location& other) {
        this->colno = other.colno;
        this->lineno = other.lineno;
        this->fname = other.fname;
        return *this;
    }

private:
    void reset_() {
        this->lineno = 0; 
        this->colno = 0; 
        this->fname = "";
    }
    UInt32 lineno;          // the line number in the source where the token occured
    UInt32 colno;           // the column number
    std::string fname;      // the file name
    friend class Lexer;
    friend class Token;
};

#endif // _HAZEL_LEXER_LOCATION