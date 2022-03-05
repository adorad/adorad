/*
          _____   ____  _____            _____
    /\   |  __ \ / __ \|  __ \     /\   |  ___\
   /  \  | |  | | |  | | |__) |   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /   / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \  / ____ \| |__| | https://github.com/adorad/adorad/
/_/    \_\_____/ \____/|_|  \_\/_/    \_\_____/

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021-22 Jason Dsouza <@jasmcaus>
*/

#ifndef CORETEN_OS_DEFS_H
#define CORETEN_OS_DEFS_H

#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WINDOWS__)
    #define CORETEN_OS_WINDOWS     1
#elif defined(__APPLE__) && defined(__MACH__)
    #define CORETEN_OS_OSX         1
#elif defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
    #define CORETEN_OS_UNIX        1

    #if defined(__linux__)
        #define CORETEN_OS_LINUX   1
    #elif defined(__FreeBSD__) && defined(__FreeBSD_kernel__)
        #define CORETEN_OS_FREEBSD 1
    #else 
        #error AdoradError: This Unix Operating System is not supported by Adorad.
    #endif 
#elif defined(_gnu_linux_) || defined(__linux__) && !defined(CORETEN_OS_LINUX)
    #define CORETEN_OS_LINUX 1
#elif defined(__EMSCRIPTEN__)
    #define CORETEN_OS_EMSCRIPTEN
#elif defined(__MACH__)
    #define CORETEN_OS_MACH
#elif defined(__NetBSD__)
    #define CORETEN_OS_NETBSD
#elif defined(__DragonFly__)
    #define CORETEN_OS_DRAGONFLY
#elif defined(__OpenBSD__)
    #define CORETEN_OS_OPENBSD
#elif defined(__HAIKU__)
    #define CORETEN_OS_HAIKU
#else
    #error AdoradError: This Operating System is not supported by Adorad
#endif

#if !defined(_MSC_VER) && !defined(_WIN64)
    #define CORETEN_OS_POSIX 1
#endif

// Architetures
#if defined(__i386__) || defined(_M_IX86) || defined(_X86_) || defined(__amd64__) || defined(__x86_64__) || \
defined(_M_X64) || defined(_M_AMD64)
    #define CORETEN_ARCH_X86           1
    #if defined(__x86_64__)
        #define CORETEN_ARCH_X86_64    1
    #endif // __x86_64__
#elif defined(__aarch64__)
    #define CORETEN_ARCH_ARM64     1
#elif defined(__ATM_EABI__)
    #define CORETEN_ARCH_ARM       1
#else
    #define CORETEN_ARCH_UNKNOWN
    // #error This Architecture is not supported by Adorad
#endif // __x86_64

// Generic 32 vs 64 bit
#if defined(_WIN64) || defined(__x86_64__) || defined(_M_X64) || defined(__64BIT__) || defined(__powerpc64__) || \
defined(__ppc64__)
    #define CORETEN_ARCH_64BIT     1
#else 
    #define CORETEN_ARCH_32BIT     1
#endif // _WIN64

#ifdef CORETEN_OS_WINDOWS
    #define CORETEN_OS_SEP         "\\"
    #define CORETEN_OS_SEP_CHAR    '\\'
#else
    #define CORETEN_OS_SEP         "/"
    #define CORETEN_OS_SEP_CHAR    '/'
#endif // CORETEN_OS_WINDOWS
#endif // CORETEN_OS_DEFS_H
