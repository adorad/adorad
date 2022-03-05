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

#ifndef CORETEN_CPU_H
#define CORETEN_CPU_H

#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__i386) || defined(__x86_64__) ||  \
defined(__i486__) || defined(__i486) || defined(i386) || defined(__ia64__)
    #ifndef CORETEN_CPU_X86
        #define CORETEN_CPU_X86 1
    #endif

    #ifndef CORETEN_CACHE_LINE_SIZE
        #define CORETEN_CACHE_LINE_SIZE 64
    #endif

#elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__)
    #ifndef CORETEN_CPU_PPC
        #define CORETEN_CPU_PPC 1
    #endif

    #ifndef CORETEN_CACHE_LINE_SIZE
        #define CORETEN_CACHE_LINE_SIZE 128
    #endif

#elif defined(__arm__) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64)
    #ifndef CORETEN_CPU_ARM
        #define CORETEN_CPU_ARM 1
    #endif

    #ifndef CORETEN_CACHE_LINE_SIZE
        #define CORETEN_CACHE_LINE_SIZE 64
    #endif

#elif defined(__MIPSEL__) || defined(__mips_isa_rev)
    #ifndef CORETEN_CPU_MIPS
        #define CORETEN_CPU_MIPS 1
    #endif

    #ifndef CORETEN_CACHE_LINE_SIZE
        #define CORETEN_CACHE_LINE_SIZE 64
    #endif

#else
    #error Unknown CPU Type
#endif

// Set MINIZ_HAS_64BIT_REGISTERS to 1 if operations on 64-bit integers are reasonably fast 
// (and don't involve ompiler-generated calls to helper functions
#if defined(_M_X64) || defined(_WIN64) || defined(__MINGW64__) || defined(_LP64) || defined(__LP64__) || \
defined(__ia64__) || defined(__x86_64__)
    #define CORETEN_64BIT    1
#else
    #define CORETEN_64BIT    0
#endif

#endif // CORETEN_CPU_H