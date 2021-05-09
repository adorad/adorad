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

#ifndef CSTL_MISCELLANEOUS_H
#define CSTL_MISCELLANEOUS_H

// #ifdef __cplusplus
// namespace Hazel {
// #endif


// Defines 
#ifndef CSTL_DEF 
    #ifdef CSTL_EXTERN
        #define CSTL_DEF extern
    #else 
        #define CSTL_DEF static
    #endif 
#endif // CSTL_DEF


// Inline 
#ifdef __cplusplus
    #if defined(_MSC_VER) && _MSC_VER <= 1800 
        #define inline  __inline
    #elif !defined(__STDC_VERSION__)
        #define inline __inline__
    #else 
        #define inline 
    #endif 
#else
    // We default to C's inline function
    // NOTE: GCC performs inline substitution for `inline` functions. This presents a problem because declared `inline`
    // functions are not visible to the linker and hence will result in ``undefined reference to `function` `` errors
    // A quick fix is to make sure `inline` functions are declared as static. 
    // 
    // We can enforce this here, but I'll wait for sometime. If we decide to go ahead with it, a simple text substitution
    // should work :)
    #define inline  inline
#endif 


// Force Inline
#ifndef force_inline
    #if defined(_MSC_VER)
        #if _MSC_VER < 1300
            #define force_inline
        #else 
            #define force_inline __forceinline
        #endif 
    #else 
        #define force_inline    __attribute__((__always_inline__))
    #endif 
#endif 


// No Inline 
#ifndef no_inline
    #if defined(_MSC_VER)
        #define no_inline   __declspec(noinline)
    #else 
        #define no_inline   __attribute__ ((noinline))
    #endif 
#endif 


// Cast 
#ifndef cast 
    #define cast(Type)  (Type)
#endif 


// A signed sizeof is more useful 
#ifndef CSTL_SIZEOF
    #define CSTL_SIZEOF(x)     (Ll)(sizeof(x))
#endif 


// Statics!
// static means 3-4 different things in C/C++!!
#ifndef CSTL_GLOBAL
    #define CSTL_GLOBAL       static // Global Variables
    #define CSTL_INTERNAL     static // Internal Linkage
    #define CSTL_LOCALPERSIST static // Local Persisting Variables  
#endif 


// Execute power operations
// long power(long x, long y) {
//     int total; 
    
//     if(y == 0) return 1; 
//     else if(y == 1) return x; 
//     else if(y == -1) return 1/x; 
//     else if(y > 0) {
//         total = x; 
//         total *= power(x, y-1); 
//     } 
//     else {
//         total = 1/x; 
//         total *= power(x, y+1); 
//     }

//     return total; 
// }


// Some fun with macros ==========================================
#ifndef CSTL_MAX
    #define CSTL_MAX(x, y)     ((x) > (y) ? (x) : (y))
#endif 

#ifndef CSTL_MIN
    #define CSTL_MIN(x, y)     ((x) < (y) ? (x) : (y))
#endif 

#ifndef CSTL_CLAMP
    #define CSTL_CLAMP(x, lower, upper)     CSTL_MIN(CSTL_MAX((x), (lower)), (upper))
#endif 

#ifndef CSTL_IS_BETWEEN
    #define CSTL_IS_BETWEEN(x, lower, upper)     (((lower) <= (x) ) && ((x) <= (upper)))
#endif 

#ifndef CSTL_ABS
    #define CSTL_ABS(x)     ((x) < 0 ? -(x) : (x))
#endif 


// #ifdef __cplusplus
// } // namespace Hazel
// #endif

#endif // CSTL_MISCELLANEOUS_H