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

#ifndef CORETEN_COMPILERS_H
#define CORETEN_COMPILERS_H

/*
    General compiler feature support table: https://en.cppreference.com/w/cpp/compiler_support
    MSVC C++11 feature support table: https://msdn.microsoft.com/en-us/library/hh567368.aspx
    GCC C++11 feature support table: https://gcc.gnu.org/projects/cxx-status.html

    MSVC version table:
    https://en.wikipedia.org/wiki/Microsoft_Visual_C%2B%2B#Internal_version_numbering
    MSVC++ 14.2 (16) _MSC_VER == 1920 (Visual Studio 2019)
    MSVC++ 14.1 (15) _MSC_VER == 1910 (Visual Studio 2017)
    MSVC++ 14.0      _MSC_VER == 1900 (Visual Studio 2015)
    MSVC++ 12.0      _MSC_VER == 1800 (Visual Studio 2013)
    MSVC++ 11.0      _MSC_VER == 1700 (Visual Studio 2012)
    MSVC++ 10.0      _MSC_VER == 1600 (Visual Studio 2010)
    MSVC++ 9.0       _MSC_VER == 1500 (Visual Studio 2008)
    MSVC++ 8.0       _MSC_VER == 1400 (Visual Studio 2005)
*/

// Compilers
// Why is Clang here? See https://stackoverflow.com/a/66722270
#if defined(_MSC_VER) && !defined(__clang__)
    #define CORETEN_COMPILER_MSVC 1
#elif defined(__clang__)
    #define CORETEN_COMPILER_CLANG 1
#elif defined(__GNUC__) && !defined(INTEL_COMPILER) && !defined(llvm)
    #define CORETEN_COMPILER_GCC 1
#else 
    #error AdoradError: Unknown Compiler (Adorad currently supports only MSVC, GCC and Clang)
#endif

// Some compilers don't support short-circuiting apparently, yielding ugly syntax errors when things like
// `defined(__clang__) && defined (__has_feature) && __has_feature(...)`. 
// So, we define Clang's `__has_feature` and `__has_extension` macros before referring to them
#ifndef __CORETEN_HAS_FEATURE_EXTENSION_DEFINED
    #if !defined(__has_feature)
        #define __has_feature(feature)   0
    #endif // __has_feature
    #if !defined(__has_extension)
        #define __has_extension(feature)   0
    #endif // __has_extension

    #define CORETEN_HAS_FEATURE(feature)        __has_feature(feature)
    #define CORETEN_HAS_EXTENSION(extension)    __has_extension(extension)
#endif // __CORETEN_HAS_FEATURE_EXTENSION_DEFINED


#endif // CORETEN_COMPILERS_H