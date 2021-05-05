/*
_ _    _           ______   _______        
| |  | |    /\    /___  /   |  ____|| |    
| |__| |   /  \      / /    | |__   | |       Hazel - The Fast, Expressive & Elegant Programming Language
|  __  |  / /\ \    / /     |  __|  | |       Languages: C, C++, and Assembly
| |  | | / ____ \  / /___   | |____ | |____   https://github.com/HazelLang/Hazel/
|_|_ |_|/_/    \_\/_______\ |______|_\______|

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/

#ifndef CSTL_DEBUG_H
#define CSTL_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

// Debug + Asserts  ==========================================
#ifndef CSTL_DEBUG_ASSERT
    #define CSTL_DEBUG_ASSERT3(cond, msg) typedef char static_assertion_##msg[(!!(cond))*2-1]
    // NOTE(jasmcaus): Token pasting madness!!
    #define CSTL_DEBUG_ASSERT2(cond, line) CSTL_DEBUG_ASSERT3(cond, static_assertion_at_line_##line)
    #define CSTL_DEBUG_ASSERT1(cond, line) CSTL_DEBUG_ASSERT2(cond, line)
    #define CSTL_DEBUG_ASSERT(cond)        CSTL_DEBUG_ASSERT1(cond, __LINE__)
#endif


#if defined(__cplusplus)
}
#endif

#endif // CSTL_DEBUG_H
