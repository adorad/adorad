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

#ifndef CSTL_DEBUG_CHECK
    #define CSTL_DEBUG_CHECK3(cond, msg)       typedef char static_assertion_##msg[(!!(cond))*2-1]
    // NOTE(jasmcaus): Token pasting madness!!
    #define CSTL_DEBUG_CHECK2(cond, line)      CSTL_DEBUG_CHECK3(cond, static_assertion_at_line_##line)
    #define CSTL_DEBUG_CHECK1(cond, line)      CSTL_DEBUG_CHECK2(cond, line)
    #define CSTL_DEBUG_CHECK(cond)             CSTL_DEBUG_CHECK1(cond, __LINE__)
#endif

#endif // CSTL_DEBUG_H
