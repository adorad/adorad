#ifndef _HAZEL_CORE_CSTLIB_H
#define _HAZEL_CORE_CSTLIB_H

/** 
    Jason Dsouza's (jasmcaus) C Helper Library 
    Available in the public domain
    Use at your own risk - no warranty implied

    This is a single header file that replaces (and makes modern) a bunch of useful stuff from 
    the C/C++ Standard Library

LICENSE
	This software is dual-licensed to the public domain and under the following license: you are 
    granted a perpetual, irrevocable license to copy, modify, 	publish, and distribute this file 
    as you see fit.

WARNING
	- This library is **slightly** experimental and features may not work as expected.
	- This also means that many functions are not documented.
CREDITS
	Written Jason Dsouza
*/

#ifndef(__cplusplus)
extern "C" {
#endif


// Operating Systems ==========================================
// HAZEL_SYSTEM_...
#if defined(_WIN32) || defined(_WIN64)
    #ifndef HAZEL_SYSTEM_WINDOWS
        #define HAZEL_SYSTEM_WINDOWS 1
    #endif

#elif defined(__APPLE__) && defined(__MACH__)
    #ifndef HAZEL_SYSTEM_OSX
        #define HAZEL_SYSTEM_OSX 1
    #endif

#elif defined(__unix__)
    #ifndef HAZEL_SYSTEM_UNIX
        #define HAZEL_SYSTEM_UNIX 1
    #endif

    #if defined(__linux__)
        #ifndef HAZEL_SYSTEM_LINUX
            #define HAZEL_SYSTEM_LINUX 1
        #endif
    
    #elif defined(__FreeBSD__) && defined(__FreeBSD_kernel__)
        #ifndef HAZEL_SYSTEM_FREEBSD
            #define HAZEL_SYSTEM_FREEBSD 1
        #endif

    #else 
        #error This Unix Operating System in not supported by Hazel.
    #endif 

#else
    #error This Operating System in not supported by Hazel
#endif


// Compilers ==========================================
// HAZEL_COMPILER_...
#if defined(_MSC_VER)
    #define HAZEL_COMPILER_MSVC 1
#elif defined(__GNUC__)
    #define HAZEL_COMPILER_GCC 1
#elif defined(__clang__)
    #define HAZEL_COMPILER_CLANG 1
#else 
    #error Unknown Compiler (Hazel currently supports only MSVC, GCC and Clang)
#endif 


// Defines ==========================================
#ifndef HAZEL_DEF 
    #ifdef HAZEL_STATIC
        #define HAZEL_DEF static
    #else 
        #define HAZEL_DEF extern
    #endif 
#endif // HAZEL_DEF

// CPUs ==========================================
// HAZEL_CPU_...
#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
    #ifndef HAZEL_CPU_X86
         #define HAZEL_CPU_X86 1
    #endif

    #ifndef HAZEL_CACHE_LINE_SIZE
        #define HAZEL_CACHE_LINE_SIZE 64
    #endif

#elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__)
    #ifndef HAZEL_CPU_PPC
        #define HAZEL_CPU_PPC 1
    #endif

    #ifndef HAZEL_CACHE_LINE_SIZE
        #define HAZEL_CACHE_LINE_SIZE 128
    #endif

#elif defined(__arm__) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64)
    #ifndef HAZEL_CPU_ARM
        #define HAZEL_CPU_ARM 1
    #endif

    #ifndef HAZEL_CACHE_LINE_SIZE
        #define HAZEL_CACHE_LINE_SIZE 64
    #endif

#elif defined(__MIPSEL__) || defined(__mips_isa_rev)
    #ifndef HAZEL_CPU_MIPS
        #define HAZEL_CPU_MIPS 1
    #endif

    #ifndef HAZEL_CACHE_LINE_SIZE
        #define HAZEL_CACHE_LINE_SIZE 64
    #endif

#else
    #error Unknown CPU Type
#endif // HAZEL_CPU_...


#ifndef HAZEL_CHECK
	#define HAZEL_CHECK3(cond, msg) typedef char static_assertion_##msg[(!!(cond))*2-1]
	// NOTE(jasmcaus): Token pasting madness!!
	#define HAZEL_CHECK2(cond, line) HAZEL_CHECK3(cond, static_assertion_at_line_##line)
	#define HAZEL_CHECK1(cond, line) HAZEL_CHECK2(cond, line)
	#define HAZEL_CHECK(cond)        HAZEL_CHECK1(cond, __LINE__)
#endif


// Headers ==========================================

#if defined(_WIN32) && !defined(__MINGW32__)
    #ifndef _CRT_SECURE_NO_WARNINGS
        #define _CRT_SECURE_NO_WARNINGS
    #endif
#endif

#if defined(HAZEL_SYSTEM_UNIX)
    #define _GNU_SOURCE
    #define _LARGEFILE64_SOURCE
#endif


// TODO(jasmcaus): How many of these headers do I really need?
// #include <stdarg.h>
#if !defined(HAZEL_SYSTEM_WINDOWS)
    #include <stddef.h>
    #include <stdarg.h>
#endif

#if defined(HAZEL_SYSTEM_WINDOWS)
    #if !defined(HAZEL_NO_WINDOWS_H)
        #define NOMINMAX 1
        #if !defined(HAZEL_WINDOWS_H_INCLUDED)
            #define WIN32_LEAN_AND_MEAN 1
            #define WIN32_MEAN_AND_LEAN 1
            #define VC_EXTRALEAN        1
        #endif
        #include <windows.h>
        #undef NOMINMAX

        #if !defined(HAZEL_WINDOWS_H_INCLUDED)
            #undef WIN32_LEAN_AND_MEAN
            #undef WIN32_MEAN_AND_LEAN
            #undef VC_EXTRALEAN
        #endif
    #endif

    #include <malloc.h> // NOTE(jasmcaus): _aligned_*()
    #include <intrin.h>

#else
    #include <dlfcn.h>
    #include <errno.h>
    #include <fcntl.h>
    #include <pthread.h>

    #ifndef _IOSC11_SOURCE
        #define _IOSC11_SOURCE
    #endif

    #include <stdlib.h> // NOTE(jasmcaus): malloc on linux
    #include <sys/mman.h>

    #if !defined(HAZEL_SYSTEM_OSX) && !defined(__FreeBSD__)
        #include <sys/sendfile.h>
    #endif

    #include <sys/stat.h>
    #include <sys/time.h>
    #include <sys/types.h>
    #include <time.h>
    #include <unistd.h>

    #if defined(HAZEL_CPU_X86)
        #include <xmmintrin.h>
    #endif
#endif

#if defined(HAZEL_SYSTEM_OSX)
    #include <mach/mach.h>
    #include <mach/mach_init.h>
    #include <mach/mach_time.h>
    #include <mach/thread_act.h>
    #include <mach/thread_policy.h>
    #include <sys/sysctl.h>
    #include <copyfile.h>
    #include <mach/clock.h>
#endif

#if defined(HAZEL_SYSTEM_FREEBSD)
    #include <sys/sysctl.h>
    #include <pthread_np.h>
    #include <sys/cpuset.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/uio.h>

    #define lseek64 lseek
    #define sendfile(out, in, offset, count) sendfile(out, in, offset, count, NULL, NULL, 0)
#endif

#if defined(HAZEL_SYSTEM_UNIX)
    #include <semaphore.h>
#endif

// Base Types (similar to the Types in the Hazel Language) ==========================================
#if defined(HAZEL_COMPILER_MSVC)
    #if _MSVC_VER < 1300 
        typedef unsigned char     UInt8;
        typedef signed char       Int8;
        typedef unsigned short    UInt16;
        typedef signed short      Int16;
        typedef unsigned int      UInt32;
        typedef signed int        Int32;
    #else 
        typedef unsigned __int8   UInt8;
        typedef signed __int8     Int8;
        typedef unsigned __int16  UInt16;
        typedef signed __int16    Int16;
        typedef unsigned __int32  UInt32;
        typedef signed __int32    Int32;
    #endif 

    typedef unsigned __int64  UInt64; 
    typedef signed __int64    Int64;

#else 
    #include <stdint.h>
    typedef uint8_t   UInt8; 
    typedef int8_t    Int8; 
    typedef uint16_t  UInt16; 
    typedef int16_t   Int16; 
    typedef uint32_t  UInt32; 
    typedef int32_t   Int32; 
    typedef uint64_t  UInt64; 
    typedef int64_t   Int64; 
#endif // Hazel Basic Types 

typedef float  Float32; 
typedef double Float64; 

HAZEL_CHECK(sizeof(UInt8)  == sizeof(Int8));
HAZEL_CHECK(sizeof(UInt16) == sizeof(Int16));
HAZEL_CHECK(sizeof(UInt32) == sizeof(Int32));
HAZEL_CHECK(sizeof(UInt64) == sizeof(Int64));

HAZEL_CHECK(sizeof(UInt8)  == 1);
HAZEL_CHECK(sizeof(UInt16) == 2);
HAZEL_CHECK(sizeof(UInt32) == 4);
HAZEL_CHECK(sizeof(UInt64) == 8);

HAZEL_CHECK(sizeof(Float32) == 4);
HAZEL_CHECK(sizeof(Float64) == 8);

// Unicode codepoint
typedef Int32 Rune; 
#define HAZEL_RUNE_INVALID cast(Rune)(0xfffd)
#define HAZEL_RUNE_MAX     cast(Rune)(0x0010ffff)
#define HAZEL_RUNE_BOM     cast(Rune)(0xfeff)
#define HAZEL_RUNE_EOF     cast(Rune)(-1)

// Max and Min 
#ifndef UInt8_MIN
    #define UInt8_MIN 0u
    #define UInt8_MAX 0xffu
    #define Int8_MIN  (-0x7f - 1)
    #define Int8_MAX  0x7f

    #define UInt16_MIN 0u
    #define UInt16_MAX 0xffffu
    #define Int16_MIN  (-0x7fff - 1)
    #define Int16_MAX  0x7fff
 
    #define UInt32_MIN 0u
    #define UInt32_MAX 0xffffffffu
    #define Int32_MIN  (-0x7fffffff - 1)
    #define Int32_MAX  0x7fffffff

    #define UInt64_MIN 0ull
    #define UInt64_MAX 0xffffffffffffffffull
    #define Int64_MIN  (-0x7fffffffffffffffll - 1)
    #define Int64_MAX  0x7fffffffffffffffll

    #if defined(GB_ARCH_32_BIT)
        #define UINTSIZE_MIX UInt32_MIN
        #define UINTSIZE_MAX UInt32_MAX

        #define INTSIZE_MIX Int32_MIN
        #define INTSIZE_MAX Int32_MAX
    
    #elif defined(GB_ARCH_64_BIT)
        #define UINTSIZE_MIX UInt64_MIN
        #define UINTSIZE_MAX UInt64_MAX

        #define INTSIZE_MIX Int64_MIN
        #define INTSIZE_MAX Int64_MAX
#else
        #error Unknown architecture size. JHazel only supports 32-bit and 64-bit architectures.
#endif 

#define Float32_MIN 1.17549435e-38f
#define Float32_MAX 3.40282347e+38f

#define Float64_MIN 2.2250738585072014e-308
#define Float64_MAX 1.7976931348623157e+308


// More Useful Types 
#ifndef null 
    #if defined(__cplusplus)
        #if __cplusplus >= 201103L
            #define null    nullptr 
        #else 
            #define null    0
        #endif 
    
    #else 
        #define null    (void*)0
#endif 

#define nullchar '\0'

// The same thing as size_t 
#ifndef _Ull_DEFINED
    #define _Ull_DEFINED
    #undef Ull
    #ifdef _WIN64
        typedef unsigned __int64 Ull;
    #else
        typedef unsigned int Ull;
    #endif //_WIN64
#endif

// bool is a basic type in C++ and not C
// We could just have used <stdbool.h> but I prefer this as it results in a smaller binary
#ifndef __cplusplus
    typedef unsigned char bool;
    static const bool false = 0;
    static const bool true = 1;
#endif 

// #ifndef __cplusplus
//     #define bool   _Bool
//     #define true   1
//     #define false  0
// #else
    /* Supporting _Bool in C++ is a GCC extension.  */
    // #define _Bool	bool
// #endif // __cplusplus 


// Inline 
#ifndef(__cplusplus) 
    #if defined(_MSC_VER) && _MSC_VER <= 1800 
        #define inline  __inline
    #elif !defined(__STDC_VERSION__)
        #define inline __inline__
    #else 
        #define inline 
    #endif 
#endif 

// Force Inline
#ifndef (force_inline)
    #if defined(_MSC_VER)
        #if _MSC_VER < 1300
            #define force_inline
        #else 
            #define force_inline __forceinline
        #endif 
    #else 
        #define force_inline    __attribute__ ((__always_inline__))
    #endif 
#endif 

// No Inline 
#ifndef (no_inline)
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
#ifndef hazel_sizeof
    #define hazel_sizeof(x)     (ptrdiff_t)(sizeof(x))
#endif 

// Statics!
// static means 3-4 different things in C/C++!!
#ifndef HAZEL_GLOBAL
    #define HAZEL_GLOBAL       static // Global Variables
    #define HAZEL_INTERNAL     static // Internal Linkage
    #define HAZEL_LOCALPERSIST static // Local Persisting Variables  
#endif 


// Some fun with macros ==========================================
#ifndef HAZEL_MAX
    #define HAZEL_MAX(x, y)     ((x) > (y) ? (x) : (y))
#endif 

#ifndef HAZEL_MIN
    #define HAZEL_MIN(x, y)     ((x) < (y) ? (x) : (y))
#endif 

#ifndef HAZEL_CLAMP
    #define HAZEL_CLAMP(x, lower, upper)     HAZEL_MIN(HAZEL_MAX((x), (lower)), (upper))
#endif 

#ifndef HAZEL_IS_BETWEEN
    #define HAZEL_IS_BETWEEN(x, lower, upper)     (((lower) <= (x) ) && ((x) <= (upper)))
#endif 

#ifndef HAZEL_ABS
    #define HAZEL_ABS(x)     ((x) < 0 ? -(x) : (x))
#endif 


// Char Things ==========================================
HAZEL_DEF char gb_char_to_lower       (char c);
HAZEL_DEF char gb_char_to_upper       (char c);
HAZEL_DEF b32  gb_char_is_space       (char c);
HAZEL_DEF b32  gb_char_is_digit       (char c);
HAZEL_DEF b32  gb_char_is_hex_digit   (char c);
HAZEL_DEF b32  gb_char_is_alpha       (char c);
HAZEL_DEF b32  gb_char_is_alphanumeric(char c);
HAZEL_DEF Int32  gb_digit_to_int        (char c);
HAZEL_DEF Int32  gb_hex_digit_to_int    (char c);

// NOTE(jasmcaus): ASCII only
HAZEL_DEF void gb_str_to_lower(char* str);
HAZEL_DEF void gb_str_to_upper(char* str);

HAZEL_DEF ptrdiff_t gb_strlen (char const* str);
HAZEL_DEF ptrdiff_t gb_strnlen(char const* str, ptrdiff_t max_len);
HAZEL_DEF Int32   gb_strcmp (char const* s1, char const* s2);
HAZEL_DEF Int32   gb_strncmp(char const* s1, char const* s2, ptrdiff_t len);
HAZEL_DEF char* gb_strcpy (char* dest, char const* source);
HAZEL_DEF char* gb_strncpy(char* dest, char const* source, ptrdiff_t len);
HAZEL_DEF ptrdiff_t gb_strlcpy(char* dest, char const* source, ptrdiff_t len);
HAZEL_DEF char* gb_strrev (char* str); // NOTE(jasmcaus): ASCII only

// NOTE(jasmcaus): A less fucking crazy strtok!
HAZEL_DEF char const* gb_strtok(char* output, char const* src, char const* delimit);

HAZEL_DEF b32 gb_str_has_prefix(char const* str, char const* prefix);
HAZEL_DEF b32 gb_str_has_suffix(char const* str, char const* suffix);

HAZEL_DEF char const* gb_char_first_occurence(char const* str, char c);
HAZEL_DEF char const* gb_char_last_occurence (char const* str, char c);

HAZEL_DEF void gb_str_concat(char* dest, ptrdiff_t dest_len,
                          char const* src_a, ptrdiff_t src_a_len,
                          char const* src_b, ptrdiff_t src_b_len);

HAZEL_DEF UInt64   gb_str_to_UInt64(char const* str, char* *end_ptr, Int32 base); // TODO(jasmcaus): Support more than just decimal and hexadecimal
HAZEL_DEF Int64   gb_str_to_Int64(char const* str, char* *end_ptr, Int32 base); // TODO(jasmcaus): Support more than just decimal and hexadecimal
HAZEL_DEF Float32   gb_str_to_Float32(char const* str, char* *end_ptr);
HAZEL_DEF Float64   gb_str_to_Float64(char const* str, char* *end_ptr);
HAZEL_DEF void  gb_Int64_to_str(Int64 value, char* string, Int32 base);
HAZEL_DEF void  gb_UInt64_to_str(UInt64 value, char* string, Int32 base);


////////////////////////////////////////////////////////////////
//
// UTF-8 Handling
//
//

// NOTE(jasmcaus): Does not check if utf-8 string is valid
HAZEL_DEF ptrdiff_t gb_utf8_strlen (u8 const* str);
HAZEL_DEF ptrdiff_t gb_utf8_strnlen(u8 const* str, ptrdiff_t max_len);

// NOTE(jasmcaus): Windows doesn't handle 8 bit filenames well ('cause Micro$hit)
HAZEL_DEF u16 *gb_utf8_to_ucs2    (u16 *buffer, ptrdiff_t len, u8 const* str);
HAZEL_DEF u8 * gb_ucs2_to_utf8    (u8 *buffer, ptrdiff_t len, u16 const* str);
HAZEL_DEF u16 *gb_utf8_to_ucs2_buf(u8 const* str);   // NOTE(jasmcaus): Uses locally persisting buffer
HAZEL_DEF u8 * gb_ucs2_to_utf8_buf(u16 const* str); // NOTE(jasmcaus): Uses locally persisting buffer

// NOTE(jasmcaus): Returns size of codepoint in bytes
HAZEL_DEF ptrdiff_t gb_utf8_decode        (u8 const* str, ptrdiff_t str_len, Rune *codepoint);
HAZEL_DEF ptrdiff_t gb_utf8_codepoint_size(u8 const* str, ptrdiff_t str_len);
HAZEL_DEF ptrdiff_t gb_utf8_encode_rune   (u8 buf[4], Rune r);


// Memory ==========================================
#ifndef KB_TO_BYTES
    #define KB_TO_BYTES(x) (           (x) * (Int64)(1024))
    #define MB_TO_BYTES(x) (KB_TO_BYTES(x) * (Int64)(1024))
    #define GB_TO_BYTES(x) (MB_TO_BYTES(x) * (Int64)(1024))
    #define TB_TO_BYTES(x) (GB_TO_BYTES(x) * (Int64)(1024))
#endif 


// Time ==========================================
HAZEL_DEF UInt64  gb_rdtsc       (void);
HAZEL_DEF Float64  gb_time_now    (void); // NOTE(jasmcaus): This is only for relative time e.g. game loops
HAZEL_DEF UInt64  gb_utc_time_now(void); // NOTE(jasmcaus): Number of microseconds since 1601-01-01 UTC
HAZEL_DEF void gb_sleep_ms    (UInt32 ms);


//
// 
//
//
//
//
// Implementations! 
//
// 
//
//
//
//
// ==========================================
#if defined(HAZEL_CORE_CSTDLIB_IMPL) && !defined(HAZEL_CORE_CSTDLIB_IMPL_COMPLETED)
#define HAZEL_CORE_CSTDLIB_IMPL




#endif // HAZEL_CORE_CSTDLIB_IMPL


#ifndef(__cplusplus)
}
#endif

#endif // _HAZEL_CORE_CSTLIB_H