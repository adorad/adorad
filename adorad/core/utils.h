/*
          _____   ____  _____            _____  
    /\   |  __ \ / __ \|  __ \     /\   |  __ \ 
   /  \  | |  | | |  | | |__) |   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /   / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \  / ____ \| |__| | https://github.com/adorad/AdoradInternalTests/
/_/    \_\_____/ \____/|_|  \_\/_/    \_\_____/ 

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <@jasmcaus>
*/

#ifndef CORETEN_UTILS_H
#define CORETEN_UTILS_H

#include <stdlib.h>
#include <adorad/core/compilers.h>

#ifdef _WIN32
    #define __func__        __FUNCTION__
#endif

#define abort()     exit(1)

// Make __attribute__ annotations (e.g. for checking printf-like functions a no-op for MSVC
// That way, the known semantics of __attribute__(...) remains clear and no wrapper needs to be used.
#if defined(CORETEN_COMPILER_MSVC)
    #define __attribute__(X)
    #define __zu               "%Iu"
    #define strdup              _strdup
    // This function or variable may be unsafe. Consider using <str(n)cpy>_s instead. 
    // To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details. 
    // #define strncpy             strncpy_s
    // #define strcpy              strcpy_s
#endif // CORETEN_COMPILER_MSVC

// Enable the use of the non-standard keyword __attribute__ to silence warnings under some compilers
#if defined(__GNUC__) || defined(CORETEN_COMPILER_CLANG)
    #define CORETEN_ATTRIBUTE_(attr)    __attribute__((attr))
#else
    #define CORETEN_ATTRIBUTE_(attr)
#endif // __GNUC__

#ifdef CORETEN_COMPILER_MSVC
    #define ATTRIBUTE_COLD        __declspec(noinline)
    #define ATTRIBUTE_PRINTF(a,b)
    #define ATTRIBUTE_NORETURN    __declspec(noreturn)
    #define ATTRIBUTE_WEAK
    #define ATTRIBUTE_UNUSED
    #define BREAKPOINT            __debugbreak())
#else
    #define ATTRIBUTE_COLD        CORETEN_ATTRIBUTE_(cold)
    #define ATTRIBUTE_PRINTF(a,b) CORETEN_ATTRIBUTE_(format(printf, a, b))
    #define ATTRIBUTE_NORETURN    CORETEN_ATTRIBUTE_(noreturn)
    #define ATTRIBUTE_WEAK        CORETEN_ATTRIBUTE_(weak)
    #define ATTRIBUTE_UNUSED      CORETEN_ATTRIBUTE_(unused)

    #if defined(__MINGW32__) || defined(__MINGW64__)
        #define BREAKPOINT        __debugbreak()
    #elif defined(__i386__) || defined(__x86_64__)
        #define BREAKPOINT        __asm__ volatile("int $0x03");
    #elif defined(__clang__)
        #define BREAKPOINT        __builtin_debugtrap()
    #elif defined(__GNUC__)
        #define BREAKPOINT        __builtin_trap()
    #else
        #include <signal.h>
        #define BREAKPOINT        raise(SIGTRAP)
    #endif // __MINGW32__
#endif // CORETEN_COMPILER_MSVC
#endif // CORETEN_UTILS_H