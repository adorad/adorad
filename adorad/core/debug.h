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

#ifndef CSTL_DEBUG_H
#define CSTL_DEBUG_H

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <adorad/core/os_defs.h>
#include <adorad/core/headers.h>
#include <adorad/core/misc.h>


// ========================= Debug + Asserts =========================
// This macro is only for simple assertion checks (that don't require a message to STDOUT).
// Note that this is not recommended. Use CSTL_CHECK instead
// If a condition fails, this raises a compilation error (negative index) --> 0*2 == 0 + (-1) == -1!
#ifndef CSTL_DEBUG_CHECK
    // NOTE(jasmcaus): Token pasting madness!!
    #define CSTL_DEBUG_CHECK1(cond, line)      typedef char static_assertion_at_line_##line[(!!(cond))*2-1]
    #define CSTL_DEBUG_CHECK(cond)             CSTL_DEBUG_CHECK1(cond, __LINE__)
#endif


#if defined(__cplusplus)
    #include <exception>
#endif //__cplusplus

#if defined(__cplusplus)
    #define CSTL_ABORT()     std::abort()
#else
    #define CSTL_ABORT()     exit(1)
#endif //__cplusplus

// Enable the use of the non-standard keyword __attribute__ to silence warnings under some compilers
#if defined(__GNUC__) || defined(CSTL_COMPILER_CLANG)
    #define CSTL_ATTRIBUTE_(attr)    __attribute__((attr))
#else
    #define CSTL_ATTRIBUTE_(attr)
#endif // __GNUC__

#ifdef __cplusplus
    // On C++, default to its polymorphism capabilities
    #define CSTL_OVERLOADABLE
#elif defined(CSTL_COMPILER_CLANG)
    // If we're still in C, use the __attribute__ keyword for Clang
    #define CSTL_OVERLOADABLE   __attribute__((overloadable))
#endif // __cplusplus

#if defined(CSTL_COMPILER_MSVC) || defined(__cplusplus)
    #define CSTL_WEAK     inline
    #define CSTL_UNUSED
#else
    #define CSTL_WEAK     __attribute__((weak))
    #define CSTL_UNUSED   __attribute__((unused))
#endif // CSTL_COMPILER_MSVC


#define CSTL_COLOUR_ERROR     1
#define CSTL_COLOUR_SUCCESS   2
#define CSTL_COLOUR_WARN      3
#define CSTL_COLOUR_CYAN      4
#define CSTL_COLOUR_BOLD      5

static inline int CSTL_ATTRIBUTE_(format (printf, 2, 3))
cstlColouredPrintf(int colour, const char* fmt, ...);
static inline int CSTL_ATTRIBUTE_(format (printf, 2, 3))
cstlColouredPrintf(int colour, const char* fmt, ...) {
    va_list args;
    char buffer[256];
    int n;

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    buffer[sizeof(buffer)-1] = '\0';

#ifdef CSTL_OS_UNIX
    {
        const char* str;
        switch(colour) {
            case CSTL_COLOUR_ERROR:    str = "\033[1;31m"; break;
            case CSTL_COLOUR_SUCCESS:  str = "\033[1;32m"; break;
            case CSTL_COLOUR_WARN:     str = "\033[1;33m"; break;
            case CSTL_COLOUR_CYAN:     str = "\033[1;36m"; break;
            case CSTL_COLOUR_BOLD:     str = "\033[1m"; break;
            default:                   str = "\033[0m"; break;
        }
        printf("%s", str);
        n = printf("%s", buffer);
        printf("\033[0m"); // Reset the colour
        return n;
    }
#elif defined(CSTL_OS_WINDOWS)
    {
        HANDLE h;
        CONSOLE_SCREEN_BUFFER_INFO info;
        WORD attr;

        h = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(h, &info);

        switch(colour) {
            case CSTL_COLOUR_ERROR:      attr = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
            case CSTL_COLOUR_SUCCESS:    attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case CSTL_COLOUR_CYAN:       attr = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case CSTL_COLOUR_WARN:       attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case CSTL_COLOUR_BOLD:       attr = FOREGROUND_BLUE | FOREGROUND_GREEN |FOREGROUND_RED | 
                                         FOREGROUND_INTENSITY; break;
            default:                     attr = 0; break;
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
#endif // CSTL_UNIX_
}


static inline int CSTL_isDigit(char c) { return c >= '0' && c <= '9'; }

static inline int cstlShouldDecomposeMacro(char const* actual, char const* expected, int isStringCmp) {
    // Signal that the macro can be further decomposed if either of the following symbols are present
    int dots = 0;
    int numActualDigits = 0;
    int numExpectedDigits = 0;

    // If not inside a string comparison, we will return `1` only if we determine that `actual` is a variable 
    // name/expression (i.e for a value, we search through each character verifying that each is a digit
    // - for floats, we allow a maximum of 1 '.' char)
    if(!isStringCmp) {
        for(int i=0; i < strlen(actual); i++) {
            if(CSTL_isDigit(actual[i])) { numActualDigits++; }
            else if(actual[i] == '.') { 
                dots++; 
                if(dots > 1) { return 1; }
            }
            else { return 1; }
        }
        // Do the same for `expected`
        dots = 0;
        for(int i=0; i < strlen(expected); i++) {
            if(CSTL_isDigit(expected[i])) { numExpectedDigits++; }
            else if(expected[i] == '.') { 
                dots++; 
                if(dots > 1) { return 1; }
            }
            else { return 1; }
        }
    } 
    // Inside a string comparison, we search for common expression tokens like the following:
    // '(', ')', '-'
    else {
        if(strchr(actual, '(') != NULL || strchr(expected, '(') != NULL || 
           actual[0] != '"' || expected[0] != '"' ) {
            return 1;
        }
    }
    return 0;
}

#ifdef CSTL_OVERLOADABLE
    #ifndef CSTL_CAN_USE_OVERLOADABLES
        #define CSTL_CAN_USE_OVERLOADABLES
    #endif // CSTL_CAN_USE_OVERLOADABLES

    CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(float f);
    CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(double d);
    CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(long double d);
    CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(int i);
    CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(unsigned int i);
    CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(long int i);
    CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(long unsigned int i);
    CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(const void* p);

    CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(float f) { printf("%f", CSTL_CAST(double, f)); }
    CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(double d) { printf("%f", d); }
    CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(long double d) { printf("%Lf", d); }
    CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(int i) { printf("%d", i); }
    CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(unsigned int i) { printf("%u", i); }
    CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(long int i) { printf("%ld", i); }
    CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(long unsigned int i) { printf("%lu", i); }
    CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(const void* p) { printf("%p", p); }

    // long long is in C++ only
    #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) || defined(__cplusplus) && (__cplusplus >= 201103L)
        CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(long long int i);
        CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(long long unsigned int i);

        CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(long long int i) { printf("%lld", i); }
        CSTL_WEAK CSTL_OVERLOADABLE void CSTL_OVERLOAD_PRINTER(long long unsigned int i) { printf("%llu", i); }
    #endif // __STDC_VERSION__

#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
    #ifndef CSTL_CAN_USE_OVERLOADABLES
        #define CSTL_CAN_USE_OVERLOADABLES
    #endif // CSTL_CAN_USE_OVERLOADABLES

    #define CSTL_OVERLOAD_PRINTER(val)                            \
        printf(_Generic((val),                                    \
                            char : "'%c'",                        \
                            char* : "%s",                         \
                            unsigned char : "%hhu",               \
                            short : "%hd",                        \
                            unsigned short : "%hu",               \
                            int : "%d",                           \
                            unsigned int : "%u",                  \
                            long : "%ld",                         \
                            long long : "%lld",                   \
                            unsigned long : "%lu",                \
                            unsigned long long : "%"CSTL_PRIu64,  \
                            float : "%f",                         \
                            double : "%f",                        \
                            long double : "%Lf",                  \
                            void* : "%p"),                        \
                    (val))
    
#else
    // If we're here, this means that the Compiler does not support overloadable methods
    #define CSTL_OVERLOAD_PRINTER(...)                                                             \
        printf("Error: Your compiler does not support overloadable methods.");                     \
        printf("If you think this was an error, please file an issue on Adorad's Github repo."   )
#endif // CSTL_OVERLOADABLE

// ifCondFailsThenPrint is the string representation of the opposite of the truthy value of `cond`
// For example, if `cond` is "!=", then `ifCondFailsThenPrint` will be `==`
#if defined(CSTL_CAN_USE_OVERLOADABLES)
    #define __CSTLCMP__(actual, expected, cond, space, macroName)                        \
        do {                                                                             \
            if(!((actual)cond(expected))) {                                              \
                printf("%s:%u: ", __FILE__, __LINE__);                                   \
                cstlColouredPrintf(CSTL_COLOUR_ERROR, "FAILED\n");                       \
                if(cstlShouldDecomposeMacro(#actual, #expected, 0)) {                    \
                    cstlColouredPrintf(CSTL_COLOUR_CYAN, "  In macro : ");               \
                    cstlColouredPrintf(CSTL_COLOUR_CYAN, "%s( %s, %s )\n",               \
                                                                #macroName,              \
                                                                #actual, #expected);     \
                }                                                                        \
                printf("  Expected : %s", #actual);                                      \
                printf(" %s ", #cond space);                                             \
                CSTL_OVERLOAD_PRINTER(expected);                                         \
                printf("\n");                                                            \
                                                                                         \
                printf("    Actual : %s", #actual);                                      \
                printf(" == ");                                                          \
                CSTL_OVERLOAD_PRINTER(actual);                                           \
                printf("\n");                                                            \
                CSTL_ABORT();                                                            \
            }                                                                            \
        }                                                                                \
        while(0)

// CSTL_OVERLOAD_PRINTER does not work on some compilers
#else
    #define __CSTLCMP__(actual, expected, cond, space, macroName)                        \
        do {                                                                             \
            if(!((actual)cond(expected))) {                                              \
                printf("%s:%u: ", __FILE__, __LINE__);                                   \
                cstlColouredPrintf(CSTL_COLOUR_ERROR, "FAILED\n");                       \
                if(cstlShouldDecomposeMacro(#actual, #expected, 0)) {                    \
                    cstlColouredPrintf(CSTL_COLOUR_CYAN, "  In macro : ");               \
                    cstlColouredPrintf(CSTL_COLOUR_CYAN, "%s( %s, %s )\n",               \
                                                                #macroName,              \
                                                                #actual, #expected);     \
                }                                                                        \
                printf("  Expected : %s", #actual);                                      \
                printf(" %s ", #cond space);                                             \
                printf(#expected);                                                       \
                printf("\n");                                                            \
                                                                                         \
                printf("    Actual : %s", #actual);                                      \
                printf(" == ");                                                          \
                printf(#actual);                                                         \
                printf("\n");                                                            \
                CSTL_ABORT();                                                            \
            }                                                                            \
        }                                                                                \
        while(0)
#endif // CSTL_CAN_USE_OVERLOADABLES

#define __CSTLCMP_STR__(actual, expected, cond, ifCondFailsThenPrint, actualPrint, macroName)       \
    do {                                                                                            \
        if(strcmp(actual, expected) cond 0) {                                                       \
            printf("%s:%u: ", __FILE__, __LINE__);                                                  \
            cstlColouredPrintf(CSTL_COLOUR_ERROR, "FAILED\n");                                      \
            if(cstlShouldDecomposeMacro(#actual, #expected, 1)) {                                   \
                    cstlColouredPrintf(CSTL_COLOUR_CYAN, "  In macro : ");                          \
                    cstlColouredPrintf(CSTL_COLOUR_CYAN, "%s( %s, %s )\n",                          \
                                                                #macroName,                         \
                                                                #actual, #expected);                \
                }                                                                                   \
            printf("  Expected : \"%s\" %s \"%s\"\n", actual, #ifCondFailsThenPrint, expected);     \
            printf("    Actual : %s\n", #actualPrint);                                              \
            CSTL_ABORT();                                                                           \
            return;                                                                                 \
        }                                                                                           \
    }                                                                                               \
    while(0)  

#define __CSTLCMP_STRN__(actual, expected, n, cond, ifCondFailsThenPrint, actualPrint, macroName)    \
    do {                                                                                             \
        if(CSTL_CAST(int, n) < 0) {                                                                  \
            cstlColouredPrintf(CSTL_COLOUR_ERROR, "`n` cannot be negative\n");                       \
            CSTL_ABORT;                                                                              \
        }                                                                                            \
        if(strncmp(actual, expected, n) cond 0) {                                                    \
            printf("%s:%u: ", __FILE__, __LINE__);                                                   \
            cstlColouredPrintf(CSTL_COLOUR_ERROR, "FAILED\n");                                       \
            if(cstlShouldDecomposeMacro(#actual, #expected, 1)) {                                    \
                    cstlColouredPrintf(CSTL_COLOUR_CYAN, "  In macro : ");                           \
                    cstlColouredPrintf(CSTL_COLOUR_CYAN, "%s( %s, %s, %s)\n",                        \
                                                                #macroName,                          \
                                                                #actual, #expected, #n);             \
                }                                                                                    \
            printf("  Expected : \"%.*s\" %s \"%.*s\"\n", CSTL_CAST(int, n), actual,                 \
                                                              #ifCondFailsThenPrint,                 \
                                                              CSTL_CAST(int, n), expected);          \
            printf("    Actual : %s\n", #actualPrint);                                               \
            CSTL_ABORT();                                                                            \
            return;                                                                                  \
        }                                                                                            \
    }                                                                                                \
    while(0)  


#define __CSTLCMP_TF(cond, actual, expected, negateSign, macroName)                 \
    do {                                                                            \
        if(negateSign(cond)) {                                                      \
            printf("%s:%u: ", __FILE__, __LINE__);                                  \
            cstlColouredPrintf(CSTL_COLOUR_ERROR, "FAILED\n");                      \
            if(cstlShouldDecomposeMacro(#actual, null, 0)) {                        \
                    cstlColouredPrintf(CSTL_COLOUR_CYAN, "  In macro : ");          \
                    cstlColouredPrintf(CSTL_COLOUR_CYAN, "%s( %s )\n",              \
                                                                #macroName,         \
                                                                #cond);             \
                }                                                                   \
            printf("  Expected : %s\n", #expected);                                 \
            printf("    Actual : %s\n", #actual);                                   \
            CSTL_ABORT();                                                           \
        }                                                                           \
    } while(0)

#define CSTL_CHECK_EQ(actual, expected)     __CSTLCMP__(actual, expected, ==, "", CSTL_CHECK_EQ)
#define CSTL_CHECK_NE(actual, expected)     __CSTLCMP__(actual, expected, !=, "", CSTL_CHECK_NE)
#define CSTL_CHECK_LT(actual, expected)     __CSTLCMP__(actual, expected, < , " ", CSTL_CHECK_LT)
#define CSTL_CHECK_LE(actual, expected)     __CSTLCMP__(actual, expected, <=, "", CSTL_CHECK_LE)
#define CSTL_CHECK_GT(actual, expected)     __CSTLCMP__(actual, expected, > , " ", CSTL_CHECK_GT)
#define CSTL_CHECK_GE(actual, expected)     __CSTLCMP__(actual, expected, >=, "", CSTL_CHECK_GE)

// Whole-string checks
#define CSTL_CHECK_STREQ(actual, expected)     __CSTLCMP_STR__(actual, expected, !=, ==, not equal, CSTL_CHECK_STREQ)
#define CSTL_CHECK_STRNEQ(actual, expected)    __CSTLCMP_STR__(actual, expected, ==, !=, equal, CSTL_CHECK_STRNEQ)

// Substring Checks
#define CSTL_CHECK_STRNE(actual, expected, n)     __CSTLCMP_STRN__(actual, expected, n, !=, ==, unequal substrings, CSTL_CHECK_STRNE)
#define CSTL_CHECK_STRNNE(actual, expected, n)    __CSTLCMP_STRN__(actual, expected, n, ==, !=, equal substrings, CSTL_CHECK_STRNNE) 

// Note: The negate sign `!` must be there for CSTL_CHECK_TRUE
// Do not remove it
#define CSTL_CHECK_TRUE(cond)      __CSTLCMP_TF(cond, false, true, !, CSTL_CHECK_TRUE)
#define CSTL_CHECK_FALSE(cond)     __CSTLCMP_TF(cond, true, false, , CSTL_CHECK_FALSE)

#define __CSTL_CHECK__(cond, macroName, ...)                                             \
    do {                                                                                 \
        if(!(cond)) {                                                                    \
            printf("%s:%u: ", __FILE__, __LINE__);                                       \
            if((sizeof(char[]){__VA_ARGS__}) <= 1)                                       \
                cstlColouredPrintf(CSTL_COLOUR_ERROR, "FAILED");                         \
            else                                                                         \
                cstlColouredPrintf(CSTL_COLOUR_ERROR, __VA_ARGS__);                      \
            printf("\n");                                                                \
            printf("The following assertion failed: \n");                                \
            cstlColouredPrintf(CSTL_COLOUR_CYAN, "    %s( %s )\n", #macroName, #cond);   \
            CSTL_ABORT();                                                                \
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

#define CSTL_CHECK_1_ARGS(cond)              __CSTL_CHECK__(cond, CHECK, "FAILED")
#define CSTL_CHECK_2_ARGS(cond, ...)         __CSTL_CHECK__(cond, CHECK, __VA_ARGS__)
#define CSTL_CHECK_MACRO_CHOOSER(...)        GET_3RD_ARG(__VA_ARGS__, CSTL_CHECK_2_ARGS, CSTL_CHECK_1_ARGS, )

#define CSTL_CHECK(...)      CSTL_CHECK_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#define CSTL_CHECK_NULL(val,...)       CSTL_CHECK((val) == null, __VA_ARGS__)
#define CSTL_CHECK_NOT_NULL(val,...)   CSTL_CHECK((val) != null, __VA_ARGS__)

#define CSTL_WARN(msg)     \
    cstlColouredPrintf(CSTL_COLOUR_WARN, "%s:%u:\nWARNING: %s\n", __FILE__, __LINE__, #msg)


#endif // CSTL_DEBUG_H