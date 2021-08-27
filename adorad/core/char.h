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

#endif // CORETEN_CHAR_H