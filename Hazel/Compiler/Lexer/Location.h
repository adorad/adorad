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

#include <string> 

#include <Hazel/Core/Types.h>

namespace Hazel {

// Location holds information about a location in a source file
// Includes the colno, lineno, fname...
struct Location {
public:
    Location& operator=(const Location& other) {
        this->__colno = other.__colno;
        this->__lineno = other.__lineno;
        this->__fname = other.__fname;
        return *this;
    }

    inline void set_lineno(UInt32 lineno) { this->__lineno = lineno; }
    inline void set_colno(UInt32 colno) { this->__colno = colno; }
    inline void set_fname(const std::string& fname) { this->__fname = fname; }
    inline UInt32 lineno() { return this->__lineno; }
    inline UInt32 colno() { return this->__colno; }
    inline std::string fname() { return this->__fname; }

private:
    // This is private because we restrict what can potentially reset a Location state
    void reset_() {
        this->__lineno = 0; 
        this->__colno = 0; 
        this->__fname = "";
    }

    UInt32 __lineno;          // the line number in the source where the token occured
    UInt32 __colno;           // the column number
    std::string __fname;      // the file name
    friend class Lexer;
    friend class Token;
};

} // namespace Hazel

#endif // _HAZEL_LEXER_LOCATION_H