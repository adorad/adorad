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

#ifdef __cplusplus
namespace Hazel {
#endif


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
        #define CSTL_INLINE     __inline
    #elif !defined(__STDC_VERSION__)
        #define CSTL_INLINE     __inline__
    #else 
        #define CSTL_INLINE 
    #endif 
#else
    // We default to C's inline function
    // NOTE: GCC performs inline substitution for `inline` functions. This presents a problem because declared `inline`
    // functions are not visible to the linker and hence will result in ``undefined reference to `function` `` errors
    // A quick fix is to make sure `inline` functions are declared as static. 
    // 
    // We can enforce this here, but I'll wait for sometime. If we decide to go ahead with it, a simple text substitution
    // should work :)
    #define CSTL_INLINE  inline
#endif 


// Force Inline
#ifndef CSTL_ALWAYS_INLINE
    #if defined(_MSC_VER)
        #if _MSC_VER < 1300
            #define CSTL_ALWAYS_INLINE
        #else 
            #define CSTL_ALWAYS_INLINE   __forceinline
        #endif 
    #elif __has_attribute(always_inline) || defined(__GNUC__)
        #define CSTL_ALWAYS_INLINE       __attribute__((__always_inline__)) inline
    #else 
        #define CSTL_ALWAYS_INLINE       inline
    #endif 
#endif 


// No Inline 
#ifndef CSTL_NOINLINE
    #if defined(_MSC_VER)
        #define CSTL_NOINLINE   __declspec(noinline)
    #else 
        #define CSTL_NOINLINE   __attribute__((noinline))
    #endif 
#endif 


// Casts
#ifdef __cplusplus
    #define cast(type, x)       static_cast<type>(x)
    #define ptrcast(type, x)    reinterpret_cast<type>(x)
#else
    #define cast(type, x)       ((type)x)
    #define ptrcast(type, x)    ((type)x)
#endif // __cplusplus


// Noexcept
#if defined(__cplusplus) && (__cplusplus >= 201103L)
    #define noexcept    noexcept
#endif // __cplusplus


// Nothrow
#if defined(__cplusplus) && defined(_MSC_VER)
    #define nothrow   __declspec(nothrow)
#else
    #define nothrow
#endif // __cplusplus


// printf format-string specifiers for Int64 and UInt64 respectively
#ifdef __clang__
    #define CSTL_PRId64     "lld"
    #define CSTL_PRIu64     "llu"
#else 
    #define CSTL_PRId64     "I64d"
    #define CSTL_PRIu64     "I64u"
#endif  // __clang__


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

#ifdef __cplusplus
} // namespace Hazel
#endif

#endif // CSTL_MISCELLANEOUS_H
