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

#endif // CSTL_DEBUG_H