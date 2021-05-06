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

#ifndef CSTL_COMPILERS_H
#define CSTL_COMPILERS_H

// #ifdef __cplusplus
// namespace Hazel {
// extern "C" {
// #endif


// Compilers ==========================================
// CSTL_COMPILER_...
#if defined(_MSC_VER)
    #define CSTL_COMPILER_MSVC 1
#elif defined(__GNUC__)
    #define CSTL_COMPILER_GCC 1
#elif defined(__clang__)
    #define CSTL_COMPILER_CLANG 1
#else 
    #error Unknown Compiler (Hazel currently supports only MSVC, GCC and Clang)
#endif 

// #ifdef __cplusplus
// } // extern "C"
// } // namespace Hazel
// #endif

#endif // CSTL_COMPILERS_H