/*
          _____   ____  _____            _____  
    /\   |  __ \ / __ \|  __ \     /\   |  __ \ 
   /  \  | |  | | |  | | |__) |   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /   / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \  / ____ \| |__| | https://github.com/adorad/adorad/
/_/    \_\_____/ \____/|_|  \_\/_/    \_\_____/ 

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <@jasmcaus>
*/

#ifndef CSTL_STRING_H
#define CSTL_STRING_H

#include <adorad/core/types.h>
#include <adorad/core/math.h>
#include <adorad/core/debug.h>

// UTF8 Inspiration: https://github.com/sheredom/utf8.h/blob/master/utf8.h

static inline bool isUpper(char c) { return c >= 'A' && c <= 'Z'; }
static inline bool isLower(char c) { return c >= 'a' && c <= 'z'; }
static inline bool isDigit(char c) { return c >= '0' && c <= '9'; }
static inline bool isAlpha(char c) { return isUpper(c) || isLower(c); }
static inline bool isAlphanumeric(char c) { return isAlpha(c) || isDigit(c); }

static inline bool isOctalDigit(char c) {
    return CSTL_IS_BETWEEN(c, '0', '7');
}

static inline bool isBinaryDigit(char c) {
    return c == '0' || c == '1';
}

static inline bool isHexDigit(char c) {
    return isDigit(c)                   ||
           CSTL_IS_BETWEEN(c, 'a', 'f') ||
           CSTL_IS_BETWEEN(c, 'A', 'F'); 
}

static inline bool isLetter(char c) {
    return  (c >= 'a' && c <= 'z') || 
            (c >= 'A' && c <= 'Z') || 
            (c == '_') ;
}

static inline char toLower(char c) {
    if(c >= 'A' && c <= 'Z') 
        return 'a' + (c - 'A');
    return c;
}

static inline char toUpper(char c) {
    if(c >= 'a' && c <= 'z') 
        return 'A' + (c - 'a');
    return c;
}

static inline bool isWhitespace(char c) {
    if(c == ' '  || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v')
        return true; 
    return false;
}

static inline Int32 digitToInt(char c) { 
    return isDigit(c) ? c - '0' : c - 'W'; 
}

static inline Int32 hexDigitToInt(char c) {
    if(isDigit(c))
        return digitToInt(c);

    else if(CSTL_IS_BETWEEN(c, 'a', 'f'))
        return c-'a' + 10; 

    else if(CSTL_IS_BETWEEN(c, 'A', 'F'))
        return c-'A' + 10; 

    return -1; 
}

#endif // CSTL_STRING_H