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

#ifndef CSTL_DEBUG_H
#define CSTL_DEBUG_H

// ========================= Debug + Asserts =========================

// This macro is only for simple assertion checks (that don't require a message to STDOUT).
// Note that this is not recommended. Use CSTL_CHECK instead
// If a condition fails, this raises a compilation error (negative index) --> 0*2 == 0 + (-1) == -1!
#ifndef CSTL_DEBUG_CHECK
    // NOTE(jasmcaus): Token pasting madness!!
    #define CSTL_DEBUG_CHECK1(cond, line)      typedef char static_assertion_at_line_##line[(!!(cond))*2-1]
    #define CSTL_DEBUG_CHECK(cond)             CSTL_DEBUG_CHECK1(cond, __LINE__)
#endif

// This macro is similar to the ones used in Tau, the C/C++ Testing Framework I wrote on a whim!
// See: https://github.com/jasmcaus/tau
#ifdef NDEBUG
    #define CSTL_CHECK(cond, msg)                                   \
        do {                                                        \
            if(!(cond)) {                                           \
                printf("%s:%u:", __FILE__, __LINE__);               \
                printf("\033[1;31m FAILED: %s", msg);               \
                printf("\033[0m\n");                                \
                exit(1);                                            \
            }                                                       \
        }                                                           \
        while(0)
#else 
    #define CSTL_CHECK
#endif // NDEBUG

// Enable the use of the non-standard keyword __attribute__ to silence warnings under some compilers
#if defined(__GNUC__) || defined(__clang__)
    #define CSTL_ATTRIBUTE_(attr)    __attribute__((attr))
#else
    #define CSTL_ATTRIBUTE_(attr)
#endif // __GNUC__

#define CSTL_ERROR      1
#define CSTL_SUCCESS    2
#define CSTL_WARN       3
#define CSTL_CYAN       4
#define CSTL_BOLD       5

static inline int CSTL_ATTRIBUTE_(format (printf, 2, 3))
cstlPrintf(int colour, const char* fmt, ...);
static inline int CSTL_ATTRIBUTE_(format (printf, 2, 3))
cstlPrintf(int colour, const char* fmt, ...) {
    va_list args;
    char buffer[256];
    int n;

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    buffer[sizeof(buffer)-1] = '\0';

#if defined(CSTL_OS_UNIX)
    {
        const char* str;
        switch(colour) {
            case CSTL_ERROR:    str = "\033[1;31m"; break;
            case CSTL_GREEN:    str = "\033[1;32m"; break;
            case CSTL_WARN:     str = "\033[1;33m"; break;
            case CSTL_CYAN:     str = "\033[1;36m"; break;
            case CSTL_BOLD:     str = "\033[1m"; break;
            default:            str = "\033[0m"; break;
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
            case CSTL_ERROR:    attr = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
            case CSTL_SUCCESS:  attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case CSTL_CYAN:     attr = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case CSTL_WARN:     attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case CSTL_BOLD:     attr = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | 
                                       FOREGROUND_INTENSITY; break;
            default:            attr = 0; break;
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

#endif // CSTL_DEBUG_H