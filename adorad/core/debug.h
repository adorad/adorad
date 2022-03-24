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

#ifndef CORETEN_DEBUG_H
#define CORETEN_DEBUG_H

#include <stdarg.h>
#include <stdio.h>
#include <adorad/core/os_defs.h>
#include <adorad/core/misc.h>
#include <adorad/core/compilers.h>


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


typedef enum CoretenColor {
    CORETEN_COLOR_ERROR = 1,
    CORETEN_COLOR_SUCCESS,
    CORETEN_COLOR_WARN,
    CORETEN_COLOR_CYAN,
    CORETEN_COLOR_BOLD
} CoretenColor;

// Rust-style Handling
// Change this to Adorad-style handling when it gets its own
#define OK(x)       ((x)->error == ErrorNone)
#define SOME(x)     ((x) != (null))
#define NONE(x)     ((x) == (null))

int ATTRIBUTE_PRINTF(2, 3)
cstl_colored_printf(CoretenColor color, const char* fmt, ...);

typedef enum {
    DreadLevelUnreachable = 0,
    DreadLevelAssertionFailed
} DreadLevel;

ATTRIBUTE_COLD
ATTRIBUTE_NORETURN
ATTRIBUTE_PRINTF(2, 3)
void coreten_dread(DreadLevel pl, const char* format, ...);

#define coreten_unreachable()                                                                      \
    coreten_dread(                                                                                 \
        DreadLevelUnreachable,                                                                     \
        "Unreachable: At %s:%d in %s. %s", __FILE__, __LINE__, __func__,                           \
        "Please file an issue on Adorad's Github repository"                                       \
    )

#define __ENFORCE__(cond, ...)                                                              \
    do {                                                                                    \
        if(!(cond)) {                                                                       \
            printf("%s:%u: ", __FILE__, __LINE__);                                          \
            if((sizeof(char[]){__VA_ARGS__}) <= 1)                                          \
                cstl_colored_printf(CORETEN_COLOR_ERROR, "FAILED");                        \
            else                                                                            \
                cstl_colored_printf(CORETEN_COLOR_ERROR, __VA_ARGS__);                     \
            printf("\n");                                                                   \
            printf("The following assertion failed: \n");                                   \
            cstl_colored_printf(CORETEN_COLOR_CYAN, "    CORETEN_ENFORCE( %s )\n", #cond); \
            choke_and_die();                                                                \
        }                                                                                   \
    }                                                                                       \
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
#define __GET_3RD_ARG(arg1, arg2, arg3, ...)   arg3

#define __ENFORCE_1_ARGS__(cond)              __ENFORCE__(cond, "FAILED")
#define __ENFORCE_2_ARGS__(cond, ...)         __ENFORCE__(cond, __VA_ARGS__)
#define __ENFORCE_MACRO_CHOOSER(...)          __GET_3RD_ARG(__VA_ARGS__, __ENFORCE_2_ARGS__, __ENFORCE_1_ARGS__, )

#define CORETEN_ENFORCE(...)                  __ENFORCE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#define ENFORCE_NULL(val,...)                 CORETEN_ENFORCE((val) == null, __VA_ARGS__)
#define CORETEN_ENFORCE_NN(val,...)           CORETEN_ENFORCE((val) != null, __VA_ARGS__)

#define WARN(...)     \
    cstl_colored_printf(CORETEN_COLOR_WARN, "%s:%u:\nWARNING: %s\n", __FILE__, __LINE__, __VA_ARGS__)

#define LOG(...)     \
    cstl_colored_printf(CORETEN_COLOR_WARN, "%s:%u: LOG: %s\n", __FILE__, __LINE__, __VA_ARGS__)

#ifdef CORETEN_IMPL
    int CORETEN_ATTRIBUTE_(format (printf, 2, 3))
    cstl_colored_printf(CoretenColor color, const char* fmt, ...) {
        va_list args;
        char buffer[256];
        int n;

        va_start(args, fmt);
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);
        buffer[sizeof(buffer)-1] = '\0';

    #if defined(CORETEN_OS_UNIX)
        {
            const char* str;
            switch(color) {
                case CORETEN_COLOR_ERROR:    str = "\033[1;31m"; break;
                case CORETEN_COLOR_SUCCESS:  str = "\033[1;32m"; break;
                case CORETEN_COLOR_WARN:     str = "\033[1;33m"; break;
                case CORETEN_COLOR_CYAN:     str = "\033[1;36m"; break;
                case CORETEN_COLOR_BOLD:     str = "\033[1m"; break;
                default:                      str = "\033[0m"; break;
            }
            printf("%s", str);
            n = printf("%s", buffer);
            printf("\033[0m"); // Reset the color
            return n;
        }
    #elif defined(CORETEN_OS_WINDOWS)
        {
            HANDLE h;
            CONSOLE_SCREEN_BUFFER_INFO info;
            WORD attr;

            h = GetStdHandle(STD_OUTPUT_HANDLE);
            GetConsoleScreenBufferInfo(h, &info);

            switch(color) {
                case CORETEN_COLOR_ERROR:      attr = FOREGROUND_RED   | FOREGROUND_INTENSITY; break;
                case CORETEN_COLOR_SUCCESS:    attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
                case CORETEN_COLOR_CYAN:       attr = FOREGROUND_BLUE  | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
                case CORETEN_COLOR_WARN:       attr = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
                case CORETEN_COLOR_BOLD:       attr = FOREGROUND_BLUE  | FOREGROUND_GREEN | FOREGROUND_INTENSITY | 
                                                       FOREGROUND_RED; break;
                default:                        attr = 0; break;
            }
            if(attr != 0)
                SetConsoleTextAttribute(h, attr);
            n = printf("%s", buffer);
            SetConsoleTextAttribute(h, info.wAttributes);
            return n;
        }
    #else
        n = printf("%s", buffer);
        return n;
    #endif // CORETEN_UNIX_
    }

    void coreten_dread(DreadLevel pl, const char* format, ...) {
        va_list args;
        char buffer[256];

        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        buffer[sizeof(buffer)-1] = '\0';
        const char* str;

        switch(pl) {
            case DreadLevelUnreachable: str = "CoretenUnreachable: "; break;
            default: str = "Dread: "; break;
        }
        cstl_colored_printf(CORETEN_COLOR_ERROR, "%s", str);
        printf("%s\n", buffer);
        choke_and_die();
    }

#endif // CORETEN_IMPL

#endif // CORETEN_DEBUG_H