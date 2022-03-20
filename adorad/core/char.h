/*
          _____   ____  _____            _____
    /\   |  __ \ / __ \|  __ \     /\   |  __ \
   /  \  | |  | | |  | | |__) |   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /   / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \  / ____ \| |__| | https://github.com/adorad/adorad/
/_/    \_\_____/ \____/|_|  \_\/_/    \_\_____/

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021-22 Jason Dsouza <@jasmcaus>
*/

#ifndef CORETEN_CHAR_H
#define CORETEN_CHAR_H

bool char_is_upper(char c);
bool char_is_lower(char c);
bool char_is_digit(char c);
bool char_is_alpha(char c);
bool char_is_alphanumeric(char c);
bool char_is_octal_digit(char c);
bool char_is_binary_digit(char c);
bool char_is_hex_digit(char c);
bool char_is_letter(char c);
char char_to_lower(char c);
char char_to_upper(char c) ;
bool char_is_whitespace(char c);
Int32 digit_to_int(char c);
Int32 hexdigit_to_int(char c);
char* char_first_occurence(char* str, char ch);
char* char_last_occurence(char* str, char ch);

#ifdef CORETEN_IMPL
    bool char_is_upper(char c) { 
        return c >= 'A' && c <= 'Z'; 
    }

    bool char_is_lower(char c) { 
        return c >= 'a' && c <= 'z'; 
    }

    bool char_is_digit(char c) { 
        return c >= '0' && c <= '9'; 
    }

    bool char_is_alpha(char c) { 
        return char_is_upper(c) || char_is_lower(c); 
    }

    bool char_is_alphanumeric(char c) { 
        return char_is_alpha(c) || char_is_digit(c); 
    }

    bool char_is_octal_digit(char c) { 
        return CORETEN_IS_BETWEEN(c, '0', '7'); 
    }

    bool char_is_binary_digit(char c) { 
        return c == '0' || c == '1'; 
    }

    Int32 digit_to_int(char c) { 
        return char_is_digit(c) ? c - '0' : c - 'W'; 
    }

    bool char_is_hex_digit(char c) {
        return char_is_digit(c)                ||
               CORETEN_IS_BETWEEN(c, 'a', 'f') ||
               CORETEN_IS_BETWEEN(c, 'A', 'F'); 
    }

    bool char_is_letter(char c) {
        return  (c >= 'a' && c <= 'z') || 
                (c >= 'A' && c <= 'Z') || 
                (c == '_') ;
    }

    char char_to_lower(char c) {
        if(c >= 'A' && c <= 'Z') 
            return 'a' + (c - 'A');
        return c;
    }

    char char_to_upper(char c) {
        if(c >= 'a' && c <= 'z') 
            return 'A' + (c - 'a');
        return c;
    }

    bool char_is_whitespace(char c) {
        switch(c) {
            case ' ': case '\n': case '\t': case '\r': case '\f': case '\v':
                return true;
            default:
                return false;
        }
    }

    Int32 hexdigit_to_int(char c) {
        if(char_is_digit(c))
            return digit_to_int(c);

        else if(CORETEN_IS_BETWEEN(c, 'a', 'f'))
            return c-'a' + 10; 

        else if(CORETEN_IS_BETWEEN(c, 'A', 'F'))
            return c-'A' + 10; 

        return -1; 
    }

    char* char_first_occurence(char* str, char ch) {
        for(; *str != ch; str++) {
            if(*str == nullchar)
                return null; // didn't find a `ch`
        }
        return str;
    }

    char* char_last_occurence(char* str, char ch) {
        char* result = null;
        do {
            if(*str == ch)
                result = str;
        } while(*str++);
        return result;
    }

#endif // CORETEN_IMPL

#endif // CORETEN_CHAR_H