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

#ifndef CORETEN_CPU_H
#define CORETEN_CPU_H

#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
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

#endif // CORETEN_CPU_H