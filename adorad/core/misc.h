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

#include <stdlib.h>

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


// Likely and unlikely macros
#ifndef __CORETEN_LIKELY_UNLIKELY_MACROS
    #define __CORETEN_LIKELY_UNLIKELY_MACROS
    #if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
        #define CORETEN_LIKELY(x)    __builtin_expect(x, 1)
        #define CORETEN_UNLIKELY(x)  __builtin_expect(x, 0)
    #else
        #define CORETEN_LIKELY(x) (x)
        #define CORETEN_UNLIKELY(x) (x)
    #endif
#endif // __CORETEN_LIKELY_UNLIKELY_MACROS

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

#ifndef AND_OR_TYPES
    #define AND_OR_TYPES
    #define and     &&
    #define or      ||
#endif // AND_OR_TYPES

#ifndef CORETEN_GLOBAL
    #define CORETEN_GLOBAL       static // Global Variables
    #define CORETEN_INTERNAL     static // Internal Linkage
    #define CORETEN_LOCALPERSIST static // Local Persisting Variables  
#endif 

// Get the type of `val`
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
    #define CORETEN_TYPEOF(val)                                 \
        printf("%s\n",                                          \
            _Generic((val),                                     \
                    Int8 : "Int8",                              \
                    Int16 : "Int16",                            \
                    Int32 : "Int32",                            \
                    Int64 : "Int64",                            \
                    UInt8 : "UInt8",                            \
                    UInt16 : "UInt16",                          \
                    UInt32 : "UInt32",                          \
                    UInt64 : "UInt64",                          \
                    Float32 : "Float32",                        \
                    Float64 : "Float64",                        \
                    default: "<unknown type>"                   \
                ))
#else
    #define CORETEN_TYPEOF(val) printf("[PANIC] CORETEN_TYPEOF not supported by your compiler.\n")
#endif 

#ifdef _WIN32
    #define __func__        __FUNCTION__
#endif

#ifndef __cplusplus
    #define choke_and_die()     exit(1)
#else
    #include <exception>
    #define choke_and_die()     std::abort()
#endif // __cplusplus

// Make __attribute__ annotations (e.g. for checking printf-like functions a no-op for MSVC
// That way, the known semantics of __attribute__(...) remains clear and no wrapper needs to be used.
#if defined(CORETEN_COMPILER_MSVC)
    #define __attribute__(X)
    #define __zu               "%Iu"
    #define strdup              _strdup
#endif // CORETEN_COMPILER_MSVC

// Enable the use of the non-standard keyword __attribute__ to silence warnings under some compilers
#if defined(__GNUC__) || defined(CORETEN_COMPILER_CLANG)
    #define CORETEN_ATTRIBUTE_(attr)    __attribute__((attr))
#else
    #define CORETEN_ATTRIBUTE_(attr)
#endif // __GNUC__

#ifdef CORETEN_COMPILER_MSVC
    #define ATTRIBUTE_COLD        __declspec(noinline)
    #define ATTRIBUTE_PRINTF(a,b)
    #define ATTRIBUTE_NORETURN    __declspec(noreturn)
    #define ATTRIBUTE_WEAK
    #define ATTRIBUTE_UNUSED
    #define BREAKPOINT            __debugbreak())
#else
    #define ATTRIBUTE_COLD        CORETEN_ATTRIBUTE_(cold)
    #define ATTRIBUTE_PRINTF(a,b) CORETEN_ATTRIBUTE_(format(printf, a, b))
    #define ATTRIBUTE_NORETURN    CORETEN_ATTRIBUTE_(noreturn)
    #define ATTRIBUTE_WEAK        CORETEN_ATTRIBUTE_(weak)
    #define ATTRIBUTE_UNUSED      CORETEN_ATTRIBUTE_(unused)

    #if defined(__MINGW32__) || defined(__MINGW64__)
        #define BREAKPOINT        __debugbreak()
    #elif defined(__i386__) || defined(__x86_64__)
        #define BREAKPOINT        __asm__ volatile("int $0x03");
    #elif defined(__clang__)
        #define BREAKPOINT        __builtin_debugtrap()
    #elif defined(__GNUC__)
        #define BREAKPOINT        __builtin_trap()
    #else
        #include <signal.h>
        #define BREAKPOINT        raise(SIGTRAP)
    #endif // __MINGW32__
#endif // CORETEN_COMPILER_MSVC

#endif // CORETEN_MISCELLANEOUS_H
