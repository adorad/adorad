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

#ifndef ADORAD_ERROR_H
#define ADORAD_ERROR_H

#include <adorad/core/debug.h>

typedef enum Error {
    ErrorNone,
    ErrorFileNotFound,
    ErrorInvalidCharacter,
    
    // Compiler-specific Errors
    ErrorSyntaxError,
    ErrorParseError,
    ErrorUnexpectedToken,
    ErrorExtraToken,

    // Misc
    ErrorUnicodePointTooLarge,
    ErrorUnreachable,
    ErrorAssertionFailed,
    ErrorUnexpectedNull,
} Error;

char* error_str(Error err);

ATTRIBUTE_COLD
ATTRIBUTE_NORETURN
ATTRIBUTE_PRINTF(2, 3)
void panic(Error err, const char* format, ...);

#define unreachable()                                                                                  \
    panic(                                                                                             \
        ErrorUnreachable,                                                                              \
        "Unreachable: At %s:%d in %s. %s", __FILE__, __LINE__, __func__,                               \
        "This is a bug in Adorad's compiler. Please file an issue on Adorad's Github repository"       \
    )

// For a more compiler-specific `unreachable`, uncomment the following:
/*
#if !defined(unreachable)
    #if defined(__GNUC__) && !defined(__clang__)
        #define __GCC_VERSION  (__GNUC__ * 10000L + __GNUC_MINOR__ * 100L + __GNUC_PATCHLEVEL__)
        #if (__GCC_VERSION >= 40500L)
            #define unreachable()  do { __builtin_unreachable(); } while (0)
        #endif
        #undef __GCC_VERSION

    #elif defined(__clang__) && defined(__has_builtin)
        #if __has_builtin(__builtin_unreachable)
            #define unreachable()  do { __builtin_unreachable(); } while (0)
        #endif
    
    #else
        #define unreachable()   do {} while(0);
    #endif
#endif // unreachable
*/

#endif // ADORAD_ERROR_H