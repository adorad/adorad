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

#ifndef CSTL_MISCELLANEOUS_H
#define CSTL_MISCELLANEOUS_H


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
    #define CSTL_CAST(type, x)       static_cast<type>(x)
    #define CSTL_PTRCAST(type, x)    reinterpret_cast<type>(x)
#else
    #define CSTL_CAST(type, x)       ((type)x)
    #define CSTL_PTRCAST(type, x)    ((type)x)
#endif // __cplusplus


// Noexcept
#if defined(__cplusplus) && (__cplusplus >= 201103L)
    #define CSTL_NOEXCEPT    noexcept
#else 
    #define CSTL_NOEXCEPT
#endif // __cplusplus


// Nothrow
#if defined(__cplusplus) && defined(_MSC_VER)
    #define CSTL_NOTHROW   __declspec(nothrow)
#else
    #define CSTL_NOTHROW
#endif // __cplusplus


#define CSTL_CONCATENATE_IMPL(s1, s2)   s1##s2
#define CSTL_CONCATENATE(s1, s2)        CSTL_CONCATENATE_IMPL(s1, s2)

#define CSTL_MACRO_EXPAND(args)         args

#define CSTL_STRINGIZE_IMPL(x)          #x
#define CSTL_STRINGIZE(x)               CSTL_STRINGIZE_IMPL(x)


// printf format-string specifiers for Int64 and UInt64 respectively
#if defined(_MSC_VER) && (_MSC_VER < 1920)
    #define CSTL_PRId64 "I64d"
    #define CSTL_PRIu64 "I64u"
#else
    // Avoid spurious trailing ‘%’ in format error
	// See: https://stackoverflow.com/questions/8132399/how-to-printf-uint64-t-fails-with-spurious-trailing-in-format
	#define __STDC_FORMAT_MACROS
    #include <inttypes.h>

    #define CSTL_PRId64 PRId64
    #define CSTL_PRIu64 PRIu64
#endif


// A signed sizeof is more useful 
#ifndef CSTL_SIZEOF
    #define CSTL_SIZEOF(x)    (Ll)(sizeof(x))
#endif 


// Statics!
// static means 3-4 different things in C/C++!!
#ifndef CSTL_EXTERN
    #define CSTL_EXTERN       extern
#endif 

#ifndef CSTL_STATIC
    #define CSTL_STATIC       static
#endif

#ifndef CSTL_GLOBAL
    #define CSTL_GLOBAL       static // Global Variables
    #define CSTL_INTERNAL     static // Internal Linkage
    #define CSTL_LOCALPERSIST static // Local Persisting Variables  
#endif 

// Get the type of `val`
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
    #define TYPEOF(val)                                         \
        printf("%s\n",                                          \
            _Generic((val),                                     \
                    signed char : "signed char",                \
                    unsigned char : "unsigned char",            \
                    signed short : "signed short",              \
                    unsigned short : "unsigned short",          \
                    signed int : "signed int",                  \
                    unsigned int : "unsigned int",              \
                    signed long long : "signed long long",      \
                    unsigned long long : "unsigned long long",  \
                    float : "float",                            \
                    double : "double",                          \
                    default: "unknown type"                     \
                ))
#else
    #define TYPEOF(val)
#endif 

#endif // CSTL_MISCELLANEOUS_H
