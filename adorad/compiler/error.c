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

#include <adorad/compiler/error.h>

char* error_str(Error err) {
    switch(err) {
        case ErrorNone : return "<no error>";
        case ErrorFileNotFound : return "FileNotFound";
        case ErrorInvalidCharacter : return "InvalidCharacter";
        case ErrorSyntaxError : return "SyntaxError";
        case ErrorParseError : return "ParseError";
        case ErrorUnexpectedToken: return "UnexpectedTokenError";
        case ErrorExtraToken: return "ExtraTokenError";
        case ErrorUnicodePointTooLarge: return "UnicodePointTooLargeError";
        case ErrorUnreachable: return "Unreachable";
        case ErrorAssertionFailed: return "AssertionFailed";
        case ErrorUnexpectedNull: return "UnexpectedNullError";
    }
    return "<invalid error>";
}

void adorad_panic(Error err, const char* format, ...) {
    va_list args;
    char buffer[256];

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    buffer[sizeof(buffer)-1] = '\0';
    const char* str;

    cstlColouredPrintf(CORETEN_COLOUR_ERROR, "%s: ", error_str(err));
    printf("%s\n", buffer);
    abort();
}