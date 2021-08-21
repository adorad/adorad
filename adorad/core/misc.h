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

#ifndef CORETEN_MISCELLANEOUS_H
#define CORETEN_MISCELLANEOUS_H

// Inline 
#ifdef __cplusplus
    #if defined(_MSC_VER) && _MSC_VER <= 1800 
        #define CORETEN_INLINE     __inline
    #elif !defined(__STDC_VERSION__)
        #define CORETEN_INLINE     __inline__
    #else 
        #define CORETEN_INLINE 
    #endif 
#else
    // We default to C's inline function
    // NOTE: GCC performs inline substitution for `inline` functions. This presents a problem because declared `inline`
    // functions are not visible to the linker and hence will result in ``undefined reference to `function` `` errors
    // A quick fix is to make sure `inline` functions are declared as static. 
    // 
    // We can enforce this here, but I'll wait for sometime. If we decide to go ahead with it, a simple text substitution
    // should work :)
    #define CORETEN_INLINE  inline
#endif 


// Force Inline
#ifndef CORETEN_ALWAYS_INLINE
    #if defined(_MSC_VER)
        #if _MSC_VER < 1300
            #define CORETEN_ALWAYS_INLINE
        #else 
            #define CORETEN_ALWAYS_INLINE   __forceinline
        #endif 
    #elif __has_attribute(always_inline) || defined(__GNUC__)
        #define CORETEN_ALWAYS_INLINE       __attribute__((__always_inline__)) inline
    #else 
        #define CORETEN_ALWAYS_INLINE       inline
    #endif 
#endif 


// No Inline 
#ifndef CORETEN_NOINLINE
    #if defined(_MSC_VER)
        #define CORETEN_NOINLINE   __declspec(noinline)
    #else 
        #define CORETEN_NOINLINE   __attribute__((noinline))
    #endif 
#endif 


// Casts
#define cast(type)  (type)
#ifdef __cplusplus
    #define CORETEN_CAST(type, x)       static_cast<type>(x)
    #define CORETEN_PTRCAST(type, x)    reinterpret_cast<type>(x)
#else
    #define CORETEN_CAST(type, x)       ((type)x)
    #define CORETEN_PTRCAST(type, x)    ((type)x)
#endif // __cplusplus

// Noexcept
#if defined(__cplusplus) && (__cplusplus >= 201103L)
    #define CORETEN_NOEXCEPT    noexcept
#else 
    #define CORETEN_NOEXCEPT
#endif // __cplusplus


// Nothrow
#if defined(__cplusplus) && defined(_MSC_VER)
    #define CORETEN_NOTHROW   __declspec(nothrow)
#else
    #define CORETEN_NOTHROW
#endif // __cplusplus

#define CORETEN_CONCATENATE_IMPL(s1, s2)   s1##s2
#define CORETEN_CONCATENATE(s1, s2)        CORETEN_CONCATENATE_IMPL(s1, s2)

#define CORETEN_MACRO_EXPAND(args)         args

#define CORETEN_STRINGIZE_IMPL(x)          #x
#define CORETEN_STRINGIZE(x)               CORETEN_STRINGIZE_IMPL(x)

// printf format-string specifiers for Int64 and UInt64 respectively
#if defined(_MSC_VER) && (_MSC_VER < 1920)
    #define CORETEN_PRId64 "I64d"
    #define CORETEN_PRIu64 "I64u"
#else
    // Avoid spurious trailing ‘%’ in format error
	// See: https://stackoverflow.com/questions/8132399/how-to-printf-uint64-t-fails-with-spurious-trailing-in-format
	#define __STDC_FORMAT_MACROS
    #include <inttypes.h>

    #define CORETEN_PRId64 PRId64
    #define CORETEN_PRIu64 PRIu64
#endif

// A signed sizeof is more useful 
#ifndef CORETEN_SIZEOF
    #define CORETEN_SIZEOF(x)    (Ll)(sizeof(x))
#endif 

// Statics!
// static means 3-4 different things in C/C++!!
#ifdef __cplusplus
    #define CORETEN_EXTERN     extern "C"
#else
    #define CORETEN_EXTERN     extern
#endif // __cplusplus

#ifndef CORETEN_STATIC
    #define CORETEN_STATIC       static
#endif

#ifndef CORETEN_GLOBAL
    #define CORETEN_GLOBAL       static // Global Variables
    #define CORETEN_INTERNAL     static // Internal Linkage
    #define CORETEN_LOCALPERSIST static // Local Persisting Variables  
#endif 

// Get the type of `val`
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
    #define CORETEN_TYPEOF(val)                                    \
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
    #define CORETEN_TYPEOF(val)
#endif 

#endif // CORETEN_MISCELLANEOUS_H
