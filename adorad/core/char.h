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

#ifndef CSTL_CHAR_H
#define CSTL_CHAR_H

#include <adorad/core/types.h>
#include <adorad/core/math.h>
#include <adorad/core/debug.h>

// UTF8 Inspiration: https://github.com/sheredom/utf8.h/blob/master/utf8.h

static inline bool char_is_upper(char c);
static inline bool char_is_lower(char c);
static inline bool char_is_digit(char c);
static inline bool char_is_alpha(char c);
static inline bool char_is_alphanumeric(char c);
static inline bool char_is_octal_digit(char c);
static inline bool char_is_binary_digit(char c);
static inline bool char_is_hex_digit(char c);
static inline bool char_is_letter(char c);
static inline char char_to_lower(char c);
static inline char char_to_upper(char c) ;
static inline bool char_is_whitespace(char c);
static inline Int32 digit_to_int(char c);
static inline Int32 hexdigit_to_int(char c);

static inline bool char_is_upper(char c) { return c >= 'A' && c <= 'Z'; }
static inline bool char_is_lower(char c) { return c >= 'a' && c <= 'z'; }
static inline bool char_is_digit(char c) { return c >= '0' && c <= '9'; }
static inline bool char_is_alpha(char c) { return char_is_upper(c) || char_is_lower(c); }
static inline bool char_is_alphanumeric(char c) { return char_is_alpha(c) || char_is_digit(c); }
static inline bool char_is_octal_digit(char c) { return CSTL_IS_BETWEEN(c, '0', '7'); }
static inline bool char_is_binary_digit(char c) { return c == '0' || c == '1'; }
static inline Int32 digit_to_int(char c) { return char_is_digit(c) ? c - '0' : c - 'W'; }

static inline bool char_is_hex_digit(char c) {
    return char_is_digit(c)                   ||
           CSTL_IS_BETWEEN(c, 'a', 'f') ||
           CSTL_IS_BETWEEN(c, 'A', 'F'); 
}

static inline bool char_is_letter(char c) {
    return  (c >= 'a' && c <= 'z') || 
            (c >= 'A' && c <= 'Z') || 
            (c == '_') ;
}

static inline char char_to_lower(char c) {
    if(c >= 'A' && c <= 'Z') 
        return 'a' + (c - 'A');
    return c;
}

static inline char char_to_upper(char c) {
    if(c >= 'a' && c <= 'z') 
        return 'A' + (c - 'a');
    return c;
}

static inline bool char_is_whitespace(char c) {
    if(c == ' '  || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v')
        return true; 
    return false;
}

static inline Int32 hexdigit_to_int(char c) {
    if(char_is_digit(c))
        return digit_to_int(c);

    else if(CSTL_IS_BETWEEN(c, 'a', 'f'))
        return c-'a' + 10; 

    else if(CSTL_IS_BETWEEN(c, 'A', 'F'))
        return c-'A' + 10; 

    return -1; 
}

#endif // CSTL_CHAR_H