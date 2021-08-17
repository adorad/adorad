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

#ifndef CORETEN_DEBUG_H
#define CORETEN_DEBUG_H

#include <stdarg.h>
#include <stdio.h>
#include <adorad/core/os_defs.h>
#include <adorad/core/misc.h>
#include <adorad/core/compilers.h>
#include <adorad/core/utils.h>

// Static asserts (aka compile-time asserts)
#ifndef __CORTEN_STATIC_ASSERT_DEFINED
    #define __CORTEN_STATIC_ASSERT_DEFINED
    #define coreten_assert(cond, msg)      \
        __coreten_assert_warning_push      \
        __coreten_assert_stmt(cond, msg)   \
        __coreten_assert_warning_pop       
#endif // __CORTEN_STATIC_ASSERT_DEFINED

#if __STDC_VERSION__ >= 201112L || CORETEN_HAS_FEATURE(c_static_assert)
    #define __coreten_assert_stmt(cond, msg)    _Static_assert(cond, msg)
#elif (!defined(__clang__) && !defined(__INTEL_COMPILER)) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))
    // If we're dealing with GCC >= 4.6 in C99 mode, we can still use_Static_assert but we need to suppress warnings
    #define __coreten_assert_stmt(cond, msg)    _Static_assert(cond, msg)
    
    #if __GNUC__ >= 6
        #define __coreten_assert_warning_push \
            CORETEN_GCC_SUPPRESS_WARNING_PUSH \
            CORETEN_GCC_SUPPRESS_WARNING("-Wpedantic")
    #else
        #define __coreten_assert_warning_push \
            CORETEN_GCC_SUPPRESS_WARNING_PUSH \
            CORETEN_GCC_SUPPRESS_WARNING("-pedantic")
    #endif // __GNUC__

    #define __coreten_assert_warning_pop \
        CORETEN_GCC_SUPPRESS_WARNING_POP
#elif defined(CORETEN_COMPILER_CLANG) && CORETEN_HAS_EXTENSION(c_static_assert)
    // The same thing for Clang, but we suppress a different warning
    #define __coreten_assert_stmt(cond, msg)    _Static_assert(cond, msg)
    
    #define __coreten_assert_warning_push \
        CORETEN_CLANG_SUPPRESS_WARNING_PUSH \
        CORETEN_CLANG_SUPPRESS_WARNING("-Wc11-extensions")

    #define __coreten_assert_warning_pop \
        CORETEN_CLANG_SUPPRESS_WARNING_POP
#elif _MSC_VER >= 1600
    #define __coreten_assert_stmt(cond, msg)    static_assert(cond, msg)
    #define __coreten_assert_warning_push
    #define __coreten_assert_warning_pop
#else
    // Fallback for compilers that don't support either `_Static_assert` or `static_assert
    // Not as pretty, but it gets the job done.
    // This macro is only for simple assertion checks. Because of implementation limitations, the custom user
    // messages are discarded
    // If a condition fails, this raises a compilation error (negative index) --> 0*2 == 0 + (-1) == -1!
    #define __coreten_assert_stmt__1(cond, line)   typedef char static_assertion_at_line_##line[(!!(cond))*2-1]
    #define __coreten_assert_stmt(cond, msg)            __coreten_assert_stmt__1(cond, __LINE__)
    #define __coreten_assert_warning_push
    #define __coreten_assert_warning_pop
#endif // __STDC_VERSION__


#define CORETEN_COLOUR_ERROR     1
#define CORETEN_COLOUR_SUCCESS   2
#define CORETEN_COLOUR_WARN      3
#define CORETEN_COLOUR_CYAN      4
#define CORETEN_COLOUR_BOLD      5

int ATTRIBUTE_PRINTF(2, 3)
cstlColouredPrintf(int colour, const char* fmt, ...);

typedef enum {
    PanicLevelUnreachable = 0,
    PanicLevelAssertionFailed
} PanicLevel;

ATTRIBUTE_COLD
ATTRIBUTE_NORETURN
ATTRIBUTE_PRINTF(2, 3)
void coreten_panic(PanicLevel pl, const char* format, ...);

#define coreten_unreachable()                                                                      \
    coreten_panic(                                                                                 \
        PanicLevelUnreachable,                                                                     \
        "At %s:%d in %s. This is a bug in Adorad's compiler. %s", __FILE__, __LINE__, __func__,    \
        "Please file an issue on Adorad's Github repository"                                       \
    )

#define __ENFORCE__(cond, ...)                                                           \
    do {                                                                                 \
        if(!(cond)) {                                                                    \
            printf("%s:%u: ", __FILE__, __LINE__);                                       \
            if((sizeof(char[]){__VA_ARGS__}) <= 1)                                       \
                cstlColouredPrintf(CORETEN_COLOUR_ERROR, "FAILED");                         \
            else                                                                         \
                cstlColouredPrintf(CORETEN_COLOUR_ERROR, __VA_ARGS__);                      \
            printf("\n");                                                                \
            printf("The following assertion failed: \n");                                \
            cstlColouredPrintf(CORETEN_COLOUR_CYAN, "    CORETEN_ENFORCE( %s )\n", #cond);          \
            abort();                                                                     \
        }                                                                                \
    }                                                                                    \
    while(0)

// This is a little hack that allows a form of "polymorphism" to a macro - it allows a user to optionally pass
// an extra argument to a macro in CHECK. 
// The first argument is always the condition to check/assert. If this condition fails, by default a `FAILED`
// message is sent to STDOUT. If a second argument is passed to the macro (a string), this will be outputted
// instead.
// 
// The MACRO_CHOOSER uses the list of arguments twice, first to form the name of the helper macro, and then to
// pass the arguments to that helper macro. It uses a standard trick to count the number of arguments to a macro.
//
// Neat hack from:
// https://stackoverflow.com/questions/3046889/optional-parameters-with-c-macros
// and: https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
#define GET_3RD_ARG(arg1, arg2, arg3, ...)   arg3

#define ENFORCE_1_ARGS(cond)              __ENFORCE__(cond, "FAILED")
#define ENFORCE_2_ARGS(cond, ...)         __ENFORCE__(cond, __VA_ARGS__)
#define ENFORCE_MACRO_CHOOSER(...)        GET_3RD_ARG(__VA_ARGS__, ENFORCE_2_ARGS, ENFORCE_1_ARGS, )

#define CORETEN_ENFORCE(...)      ENFORCE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#define ENFORCE_NULL(val,...)     CORETEN_ENFORCE((val) == null, __VA_ARGS__)
#define CORETEN_ENFORCE_NN(val,...)    CORETEN_ENFORCE((val) != null, __VA_ARGS__)

#define WARN(msg)     \
    cstlColouredPrintf(CORETEN_COLOUR_WARN, "%s:%u:\nWARNING: %s\n", __FILE__, __LINE__, #msg)

#endif // CORETEN_DEBUG_H