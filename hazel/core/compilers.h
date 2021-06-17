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

#ifndef CSTL_COMPILERS_H
#define CSTL_COMPILERS_H

// General compiler feature support table: https://en.cppreference.com/w/cpp/compiler_support
// MSVC C++11 feature support table: https://msdn.microsoft.com/en-us/library/hh567368.aspx
// GCC C++11 feature support table: https://gcc.gnu.org/projects/cxx-status.html
// 
// MSVC version table:
// https://en.wikipedia.org/wiki/Microsoft_Visual_C%2B%2B#Internal_version_numbering
// MSVC++ 14.2 (16) _MSC_VER == 1920 (Visual Studio 2019)
// MSVC++ 14.1 (15) _MSC_VER == 1910 (Visual Studio 2017)
// MSVC++ 14.0      _MSC_VER == 1900 (Visual Studio 2015)
// MSVC++ 12.0      _MSC_VER == 1800 (Visual Studio 2013)
// MSVC++ 11.0      _MSC_VER == 1700 (Visual Studio 2012)
// MSVC++ 10.0      _MSC_VER == 1600 (Visual Studio 2010)
// MSVC++ 9.0       _MSC_VER == 1500 (Visual Studio 2008)
// MSVC++ 8.0       _MSC_VER == 1400 (Visual Studio 2005)

// Compilers
// Why is Clang here? See https://stackoverflow.com/a/66722270
#if defined(_MSC_VER) && !defined(__clang__)
    #define CSTL_COMPILER_MSVC 1
#elif defined(__clang__)
    #define CSTL_COMPILER_CLANG 1
#elif defined(__GNUC__) && !defined(INTEL_COMPILER) && !defined(llvm)
    #define CSTL_COMPILER_GCC 1
#else 
    #error Unknown Compiler (Hazel currently supports only MSVC, GCC and Clang)
#endif 

// Disable Compiler Warnings
// These are a big nuisance when compiling with external code
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
    #define PRAGMA_TO_STR(x)                    _Pragma(#x)
    #define CSTL_CLANG_SUPPRESS_WARNING_PUSH    _Pragma("clang diagnostic push")
    #define CSTL_CLANG_SUPPRESS_WARNING_POP     _Pragma("clang diagnostic pop")
    #define CSTL_CLANG_SUPPRESS_WARNING(w)      PRAGMA_TO_STR(clang diagnostic ignored w)
#else
    #define CSTL_CLANG_SUPPRESS_WARNING_PUSH
    #define CSTL_CLANG_SUPPRESS_WARNING(w)
    #define CSTL_CLANG_SUPPRESS_WARNING_POP
#endif // CSTL_COMPILER_CLANG

#if defined(CSTL_COMPILER_GCC)
    #define PRAGMA_TO_STR(x)                    _Pragma(#x)
    #define CSTL_GCC_SUPPRESS_WARNING_PUSH      _Pragma("GCC diagnostic push")
    #define CSTL_GCC_SUPPRESS_WARNING_POP       _Pragma("GCC diagnostic pop")
    #define CSTL_GCC_SUPPRESS_WARNING(w)        PRAGMA_TO_STR(GCC diagnostic ignored w)
#else
    #define CSTL_GCC_SUPPRESS_WARNING_PUSH
    #define CSTL_GCC_SUPPRESS_WARNING(w)
    #define CSTL_GCC_SUPPRESS_WARNING_POP
#endif // CSTL_COMPILER_GCC

#endif // CSTL_COMPILERS_H