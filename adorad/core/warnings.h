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

#ifndef CSTL_WARNINGS_H
#define CSTL_WARNINGS_H

#include <adorad/core/compilers.h>

// Disable Compiler Warnings
// It isn't recommended to disable compiler warnings, but sometimes, they are a bit of nuisance especially when
// compiling with external code
// Remember: Only disable those compiler warnings that you are 100% sure are coming in the way of something
// you know for a fact is right.
// For example, in a switch-case statement when you intend for a fallthrough situation, and the compiler (helpfully)
// warns you that it could potentially fallthrough.
#if defined(CSTL_COMPILER_MSVC)
    #define CSTL_MSVC_SUPPRESS_WARNING_PUSH     __pragma(warning(push))
    #define CSTL_MSVC_SUPPRESS_WARNING(w)       __pragma(warning(disable : w))
    #define CSTL_MSVC_SUPPRESS_WARNING_POP      __pragma(warning(pop))
#else
    #define CSTL_MSVC_SUPPRESS_WARNING_PUSH
    #define CSTL_MSVC_SUPPRESS_WARNING(w)
    #define CSTL_MSVC_SUPPRESS_WARNING_POP
#endif // CSTL_COMPILER_MSVC

#if defined(CSTL_COMPILER_CLANG)
    #define ___PRAGMA_TO_STR(x)                    _Pragma(#x)
    #define CSTL_CLANG_SUPPRESS_WARNING_PUSH    _Pragma("clang diagnostic push")
    #define CSTL_CLANG_SUPPRESS_WARNING_POP     _Pragma("clang diagnostic pop")
    #define CSTL_CLANG_SUPPRESS_WARNING(w)      ___PRAGMA_TO_STR(clang diagnostic ignored w)
#else
    #define CSTL_CLANG_SUPPRESS_WARNING_PUSH
    #define CSTL_CLANG_SUPPRESS_WARNING(w)
    #define CSTL_CLANG_SUPPRESS_WARNING_POP
#endif // CSTL_COMPILER_CLANG

#if defined(CSTL_COMPILER_GCC)
    #define ___PRAGMA_TO_STR(x)                    _Pragma(#x)
    #define CSTL_GCC_SUPPRESS_WARNING_PUSH      _Pragma("GCC diagnostic push")
    #define CSTL_GCC_SUPPRESS_WARNING_POP       _Pragma("GCC diagnostic pop")
    #define CSTL_GCC_SUPPRESS_WARNING(w)        ___PRAGMA_TO_STR(GCC diagnostic ignored w)
#else
    #define CSTL_GCC_SUPPRESS_WARNING_PUSH
    #define CSTL_GCC_SUPPRESS_WARNING(w)
    #define CSTL_GCC_SUPPRESS_WARNING_POP
#endif // CSTL_COMPILER_GCC

#endif // CSTL_WARNINGS_H