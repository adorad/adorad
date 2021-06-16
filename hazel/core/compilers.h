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

#ifndef CSTL_DISABLE_DEBUG_WARNINGS
#define CSTL_DISABLE_DEBUG_WARNINGS                                                                   \
    /* MSVC Warnings */                                                                               \
    CSTL_MSVC_SUPPRESS_WARNING_PUSH                                                                   \
    /* Conditional expression is constant */                                                          \
    CSTL_MSVC_SUPPRESS_WARNING(4127)                                                                  \
    /* Assignment within conditional expression */                                                    \
    CSTL_MSVC_SUPPRESS_WARNING(4706)                                                                  \
    /* Unreferenced inline function has been removed */                                               \
    CSTL_MSVC_SUPPRESS_WARNING(4514)                                                                  \
    /* Function not inlined*/                                                                         \
    CSTL_MSVC_SUPPRESS_WARNING(4710)                                                                  \
    /* function 'x' selected for automatic inline expansion */                                        \
    CSTL_MSVC_SUPPRESS_WARNING(4711)                                                                  \
    /* No function prototype given: converting '()' to '(void)' */                                    \
    CSTL_MSVC_SUPPRESS_WARNING(4255)                                                                  \
    /* '__cplusplus' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif' */    \
    CSTL_MSVC_SUPPRESS_WARNING(4668)                                                                  \
    /* 'fprintf' : format string '' requires an argument of type 'unsigned __int64', but variadic */  \
    /* argument 1 has type 'size_t' */                                                                \
    CSTL_MSVC_SUPPRESS_WARNING(4777)                                                                  \
    /* https://stackoverflow.com/questions/39479163 - what's the difference between 4018 and 4389 */  \
    /* signed/unsigned mismatch */                                                                    \
    CSTL_MSVC_SUPPRESS_WARNING(4388)                                                                  \
    /* expression' : signed/unsigned mismatch */                                                      \
    CSTL_MSVC_SUPPRESS_WARNING(4018)                                                                  \
    /* Conversion from 'int' to 'unsigned long', signed/unsigned mismatch */                          \
    CSTL_MSVC_SUPPRESS_WARNING(4365)                                                                  \
    /* format string expected in argument 'x' is not a string literal */                              \
    CSTL_MSVC_SUPPRESS_WARNING(4774)                                                                  \
    /* 'var' : conversion from 'x' to 'y', possible loss of data */                                   \
    CSTL_MSVC_SUPPRESS_WARNING(4267)                                                                  \
    /* In multi-platform code, we cannot use the non-standard "safe" functions from */                \
    /* Microsoft's C lib like e.g. sprintf_s() instead of standard sprintf(). */                      \
    CSTL_MSVC_SUPPRESS_WARNING(4996)                                                                  \
    /* warning C4090: '=': different 'const' qualifiers */                                            \
    CSTL_MSVC_SUPPRESS_WARNING(4090)                                                                  \
    /* io.h contains definitions for some structures with natural padding. This is uninteresting, */  \
    /* but for some reason, MSVC's behaviour is to warn about including this system header. That  */  \
    /* *is* interesting */                                                                            \
    CSTL_MSVC_SUPPRESS_WARNING(4820)                                                                  \
    /* Miscellaneous */                                                                               \
    CSTL_MSVC_SUPPRESS_WARNING(4061)                                                                  \
    CSTL_MSVC_SUPPRESS_WARNING(4100)                                                                  \
    CSTL_MSVC_SUPPRESS_WARNING(4201)                                                                  \
    CSTL_MSVC_SUPPRESS_WARNING(5045)                                                                  \
    CSTL_MSVC_SUPPRESS_WARNING(5105)                                                                  \
                                                                                                      \
    /* Clang */                                                                                       \
    CSTL_CLANG_SUPPRESS_WARNING_PUSH                                                                  \
    CSTL_CLANG_SUPPRESS_WARNING("-Wdisabled-macro-expansion")                                         \
    CSTL_CLANG_SUPPRESS_WARNING("-Wlanguage-extension-token")                                         \
    CSTL_CLANG_SUPPRESS_WARNING("-Wc++98-compat")                                                     \
    CSTL_CLANG_SUPPRESS_WARNING("-Wc++98-compat-pedantic")                                            \
    CSTL_CLANG_SUPPRESS_WARNING("-Wfloat-equal")                                                      \
    CSTL_CLANG_SUPPRESS_WARNING("-Wmissing-variable-declarations")                                    \
    CSTL_CLANG_SUPPRESS_WARNING("-Wreserved-id-macro")                                                \
    CSTL_CLANG_SUPPRESS_WARNING("-Wunknown-pragmas")                                                  \
    CSTL_CLANG_SUPPRESS_WARNING("-Wstrict-overflow")                                                  \
    CSTL_CLANG_SUPPRESS_WARNING("-Wstrict-aliasing")                                                  \
    CSTL_CLANG_SUPPRESS_WARNING("-Wsign-compare")                                                     \
    CSTL_CLANG_SUPPRESS_WARNING("-Wsign-conversion")                                                  \
    CSTL_CLANG_SUPPRESS_WARNING("-Wunused-macros")                                                    \
    CSTL_CLANG_SUPPRESS_WARNING("-Wunused-function")                                                  \
    CSTL_CLANG_SUPPRESS_WARNING("-Wunused-variable")                                                  \
    CSTL_CLANG_SUPPRESS_WARNING("-Wunused-result")                                                    \
    CSTL_CLANG_SUPPRESS_WARNING("-Wunused-parameter")                                                 \
    CSTL_CLANG_SUPPRESS_WARNING("-Wunused-local-typedefs")                                            \
    CSTL_CLANG_SUPPRESS_WARNING("-Wswitch")                                                           \
    CSTL_CLANG_SUPPRESS_WARNING("-Wswitch-enum")                                                      \
    CSTL_CLANG_SUPPRESS_WARNING("-Wswitch-default")                                                   \
    CSTL_CLANG_SUPPRESS_WARNING("-Wformat")                                                           \
    CSTL_CLANG_SUPPRESS_WARNING("-Wformat-extra-args")                                                \
    CSTL_CLANG_SUPPRESS_WARNING("-Wignored-qualifiers")                                               \
    CSTL_CLANG_SUPPRESS_WARNING("-Wuninitialized")                                                    \
    CSTL_CLANG_SUPPRESS_WARNING("-Wunused-const-variable")                                            \
    CSTL_CLANG_SUPPRESS_WARNING("-Wnewline-eof")                                                      \
    CSTL_CLANG_SUPPRESS_WARNING("-Wdeprecated-declarations")                                          \
                                                                                                      \
    /* GCC */                                                                                         \
    CSTL_GCC_SUPPRESS_WARNING_PUSH                                                                    \
    CSTL_GCC_SUPPRESS_WARNING("-Wunknown-pragmas")                                                    \
    CSTL_GCC_SUPPRESS_WARNING("-Wpragmas")                                                            \
    CSTL_GCC_SUPPRESS_WARNING("-Wmaybe-uninitialized")                                                \
    CSTL_GCC_SUPPRESS_WARNING("-Wmultistatement-macros")                                              \
    CSTL_GCC_SUPPRESS_WARNING("-Wdiscarded-qualifiers")                                               \
    CSTL_GCC_SUPPRESS_WARNING("-Wdisabled-macro-expansion")                                           \
    CSTL_GCC_SUPPRESS_WARNING("-Wlanguage-extension-token")                                           \
    CSTL_GCC_SUPPRESS_WARNING("-Wc++98-compat")                                                       \
    CSTL_GCC_SUPPRESS_WARNING("-Wc++98-compat-pedantic")                                              \
    CSTL_GCC_SUPPRESS_WARNING("-Wfloat-equal")                                                        \
    CSTL_GCC_SUPPRESS_WARNING("-Wmissing-variable-declarations")                                      \
    CSTL_GCC_SUPPRESS_WARNING("-Wreserved-id-macro")                                                  \
    CSTL_GCC_SUPPRESS_WARNING("-Wunknown-pragmas")                                                    \
    CSTL_GCC_SUPPRESS_WARNING("-Wstrict-overflow")                                                    \
    CSTL_GCC_SUPPRESS_WARNING("-Wstrict-aliasing")                                                    \
    CSTL_GCC_SUPPRESS_WARNING("-Wsign-compare")                                                       \
    CSTL_GCC_SUPPRESS_WARNING("-Wsign-conversion")                                                    \
    CSTL_GCC_SUPPRESS_WARNING("-Wunused-macros")                                                      \
    CSTL_GCC_SUPPRESS_WARNING("-Wunused-function")                                                    \
    CSTL_GCC_SUPPRESS_WARNING("-Wunused-variable")                                                    \
    CSTL_GCC_SUPPRESS_WARNING("-Wunused-result")                                                      \
    CSTL_GCC_SUPPRESS_WARNING("-Wunused-parameter")                                                   \
    CSTL_GCC_SUPPRESS_WARNING("-Wunused-local-typedefs")                                              \
    CSTL_GCC_SUPPRESS_WARNING("-Wswitch")                                                             \
    CSTL_GCC_SUPPRESS_WARNING("-Wswitch-enum")                                                        \
    CSTL_GCC_SUPPRESS_WARNING("-Wswitch-default")                                                     \
    CSTL_GCC_SUPPRESS_WARNING("-Wformat")                                                             \
    CSTL_GCC_SUPPRESS_WARNING("-Wformat-extra-args")                                                  \
    CSTL_GCC_SUPPRESS_WARNING("-Wsuggest-attribute=format")                                           \
    CSTL_GCC_SUPPRESS_WARNING("-Wdeprecated-declarations")
#endif // CSTL_DISABLE_DEBUG_WARNINGS

#ifndef CSTL_DISABLE_DEBUG_WARNINGS_POP
#define CSTL_DISABLE_DEBUG_WARNINGS_POP     \
    CSTL_MSVC_SUPPRESS_WARNING_POP          \
    CSTL_CLANG_SUPPRESS_WARNING_POP         \
    CSTL_GCC_SUPPRESS_WARNING_POP      
#endif // CSTL_DISABLE_DEBUG_WARNINGS_POP

#endif // CSTL_COMPILERS_H