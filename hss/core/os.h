/*
_ _    _           ______   _______        
| |  | |    /\    /___  /   |  ____|| |    
| |__| |   /  \      / /    | |__   | |       Hazel - The Fast, Expressive & Elegant Programming Language
|  __  |  / /\ \    / /     |  __|  | |       Languages: C, C++, and Assembly
| |  | | / ____ \  / /___   | |____ | |____   https://github.com/HazelLang/hazel/
|_|_ |_|/_/    \_\/_______\ |______|_\______|

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/


#ifndef CSTL_OS_H
#define CSTL_OS_H


// Operating Systems ==========================================
// CSTL_OS_...
#if defined(_WIN32) || defined(_WIN64)
    #ifndef CSTL_OS_WINDOWS
        #define CSTL_OS_WINDOWS 1
    #endif

#elif defined(__APPLE__) && defined(__MACH__)
    #ifndef CSTL_OS_OSX
        #define CSTL_OS_OSX 1
    #endif

#elif defined(__unix__)
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
        #error This Unix Operating System in not supported by Hazel.
    #endif 

#elif defined(__NetBSD__)
    #define CSTL_OS_NETBSD
    
#elif defined(__DragonFly__)
    #define CSTL_OS_DRAGONFLY
    
#elif defined(__OpenBSD__)
    #define CSTL_OS_OPENBSD
    
#elif defined(__HAIKU__)
    #define CSTL_OS_HAIKU

#else
    #error This Operating System in not supported by Hazel
#endif


#if defined(_WIN32)
	#define CSTL_API GB_EXTERN __declspec(dllexport)
	#define GB_DLL_IMPORT GB_EXTERN __declspec(dllimport)
#else
	#define CSTL_API GB_EXTERN __attribute__((visibility("default")))
	#define GB_DLL_IMPORT GB_EXTERN
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
    // #error This Architecture is not supported by Hazel
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


#endif // CSTL_OS_H