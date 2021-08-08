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

#ifndef CSTL_OS_DEFS_H
#define CSTL_OS_DEFS_H

#if defined(_WIN32) || defined(_WIN64)
    #ifndef CSTL_OS_WINDOWS
        #define CSTL_OS_WINDOWS 1
    #endif
#elif defined(__APPLE__) && defined(__MACH__)
    #ifndef CSTL_OS_OSX
        #define CSTL_OS_OSX 1
    #endif
#elif defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
    #ifndef CSTL_OS_UNIX
        #define CSTL_OS_UNIX 1
    #endif

    #if defined(__linux__)
        #ifndef CSTL_OS_LINUX
            #define CSTL_OS_LINUX 1
        #endif
    #elif defined(__FreeBSD__) && defined(__FreeBSD_kernel__)
        #ifndef CSTL_OS_FREEBSD
            #define CSTL_OS_FREEBSD 1
        #endif
    #else 
        #error AdoradError: This Unix Operating System is not supported by Adorad.
    #endif 
#elif defined(_gnu_linux_) || defined(__linux__) && !defined(CSTL_OS_LINUX)
    #define CSTL_OS_LINUX 1
#elif defined(__EMSCRIPTEN__)
    #define CSTL_OS_EMSCRIPTEN
#elif defined(__MACH__)
    #define CSTL_OS_MACH
#elif defined(__NetBSD__)
    #define CSTL_OS_NETBSD
#elif defined(__DragonFly__)
    #define CSTL_OS_DRAGONFLY
#elif defined(__OpenBSD__)
    #define CSTL_OS_OPENBSD
#elif defined(__HAIKU__)
    #define CSTL_OS_HAIKU
#else
    #error AdoradError: This Operating System is not supported by Adorad
#endif

#if !defined(_MSC_VER) && !defined(_WIN64)
    #define CSTL_OS_POSIX 1
#endif

// Architetures
#if defined(__x86_64__)
    #define CSTL_ARCH_X86_64    1
#elif defined(__aarch64__)
    #define CSTL_ARCH_ARM64     1
#elif defined(__ATM_EABI__)
    #define CSTL_ARCH_ARM       1
#else
    #define CSTL_ARCH_UNKNOWN
    // #error This Architecture is not supported by Adorad
#endif // __x86_64

// Generic 32 vs 64 bit
#if defined(_WIN64) || defined(__x86_64__) || defined(_M_X64) || defined(__64BIT__) || defined(__powerpc64__) || defined(__ppc64__)
    #define CSTL_ARCH_64BIT     1
#else 
    #define CSTL_ARCH_32BIT     1
#endif // _WIN64

#ifdef CSTL_OS_WINDOWS
    #define CSTL_OS_SEP         "\\"
    #define CSTL_OS_SEP_CHAR    '\\'
#else
    #define CSTL_OS_SEP         "/"
    #define CSTL_OS_SEP_CHAR    '/'
#endif // CSTL_OS_WINDOWS

#endif // CSTL_OS_DEFS_H