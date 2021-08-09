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

#ifdef CORETEN_COMPILER_MSVC
    #define ATTRIBUTE_COLD        __declspec(noinline)
    #define ATTRIBUTE_PRINTF(a,b)
    #define ATTRIBUTE_NORETURN    __declspec(noreturn
    #define ATTRIBUTE_WEAK
    #define ATTRIBUTE_UNUSED
    #define BREAKPOINT            __debugbreak())
#else
    #define ATTRIBUTE_COLD        __attribute((cold))
    #define ATTRIBUTE_PRINTF(a,b) __attribute((format(printf, a, b)))
    #define ATTRIBUTE_NORETURN    __attribute((noreturn))
    #define ATTRIBUTE_WEAK        __attribute__((weak))
    #define ATTRIBUTE_UNUSED      __attribute__((unused))

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