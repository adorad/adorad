#ifndef HAZEL_LANG_COMPILER
#define HAZEL_LANG_COMPILER 

#if defined(__TINYC__) && defined(__has_include)
    // tcc does not support has_include properly yet, turn it off completely
    #undef __has_include
#endif

#if defined(__has_include)
    #if __has_include(<inttypes.h>)
        #include <inttypes.h>
    #else
        #error VERROR_MESSAGE The C compiler can not find <inttypes.h> . Please install build-essentials
    #endif

#else
    #include <inttypes.h>
#endif


////////////////////////////////////// BUILT-IN TYPES //////////////////////////////////////
typedef int8_t   Int8;
typedef int16_t  Int16;
typedef int32_t  Int; // default integer
typedef int64_t  Int64;

typedef uint8_t  UInt8; //also called a byte
typedef uint16_t UInt16;
typedef uint32_t UInt32; 
typedef uint64_t UInt64;

typedef float    Float32;         
typedef double   Float64; // default float  

typedef unsigned char* BytePtr;
typedef char*          CharPtr;
typedef void*          VoidPtr;

typedef UInt8  array_fixed_byte_300[300];

#ifndef __cplusplus
    #ifndef bool
        typedef int Bool;
        #define true 1;
        #define false 0;
    #endif
#endif 

////////////////////////////////////// C HEADERS //////////////////////////////////////
#include <stdio.h> // TODO(jasmcaus): Remove these includes; defined all function signatures and types manually
#include <stdlib.h> 

#ifdef __cplusplus
    #include <utilty>
    #define __MOV std::move
#else  
    #define __MOV
#endif 

#if defined(_WIN32) || defined(__CYGWIN__)
	#define HAZEL_EXPORTED_SYMBOL extern __declspec(dllexport)
	#define HAZEL_LOCAL_SYMBOL static
#else
	// 4 < gcc < 5 is used by some older Ubuntu LTS and Centos versions,
	// and does not support __has_attribute(visibility) ...
	#ifndef __has_attribute
		#define __has_attribute(x) 0  // Compatibility with non-clang compilers.
	#endif

	#if (defined(__GNUC__) && (__GNUC__ >= 4)) || (defined(__clang__) && __has_attribute(visibility))
		#ifdef ARM
			#define HAZEL_EXPORTED_SYMBOL  extern __attribute__((externally_visible,visibility("default")))
		#else
			#define HAZEL_EXPORTED_SYMBOL  extern __attribute__((visibility("default")))
		#endif

		#define HAZEL_LOCAL_SYMBOL  __attribute__ ((visibility ("hidden")))
	#else
		#define HAZEL_EXPORTED_SYMBOL extern
		#define HAZEL_LOCAL_SYMBOL static
	#endif
#endif

#include <signal.h>
#include <stdarg.h> // for va_list
#include <string.h> // memcpy

// Check bitness
#if INTPTR_MAX == INT32_MAX
	#define TARGET_IS_32BIT 1
#elif INTPTR_MAX == INT64_MAX
	#define TARGET_IS_64BIT 1
#else
	#error "The environment is neither 32 nor 64-bit."
#endif

// Check endianness
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ || \
defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN ||  \
defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || \
defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
	#define TARGET_ORDER_IS_BIG

#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ || \
defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || defined(__LITTLE_ENDIAN__) || \
defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(_MIPSEL) || \
defined(__MIPSEL) || defined(__MIPSEL__) || defined(_M_AMD64) || defined(_M_X64) || defined(_M_IX86)
	#define TARGET_ORDER_IS_LITTLE

#else
	#error "Unknown architecture endianness"
#endif

#ifndef _WIN32
	#include <ctype.h>
	#include <locale.h> // tolower
	#include <sys/time.h>
	#include <unistd.h> // sleep
	extern char **environ;
#endif


#if defined(__CYGWIN__) && !defined(_WIN32)
	#error "Cygwin is not supported. Please use MinGW or Visual Studio."
#endif

#ifdef __linux__
	#include <sys/types.h>
	#include <sys/wait.h> // os__wait uses wait on nix
#endif

#ifdef __FreeBSD__
	#include <sys/types.h>
	#include <sys/wait.h> // os__wait uses wait on nix
#endif

#ifdef __DragonFly__
	#include <sys/types.h>
	#include <sys/wait.h> // os__wait uses wait on nix
#endif

#ifdef __OpenBSD__
	#include <sys/types.h>
	#include <sys/resource.h>
	#include <sys/wait.h> // os__wait uses wait on nix
#endif

#ifdef __NetBSD__
	#include <sys/wait.h> // os__wait uses wait on nix
#endif

#ifdef __sun
	#include <sys/types.h>
	#include <sys/wait.h> // os__wait uses wait on nix
#endif


#define EMPTY_VARG_INITIALIZATION 0
#define EMPTY_STRUCT_DECLARATION
#define EMPTY_STRUCT_INITIALIZATION 0
// Due to a tcc bug, the length of an array needs to be specified, but GCC crashes if it is...
#define EMPTY_ARRAY_OF_ELEMS(x,n) (x[])
#define TCCSKIP(x) x


#define __NOINLINE __attribute__((noinline))
#define __IRQHANDLER __attribute__((interrupt))

#if defined(__x86_64__)
#define __V_amd64  1
#endif
#if defined(__aarch64__) || defined(__arm64__)
#define __V_aarch64  1
#endif

// Using just __GNUC__ for detecting gcc, is not reliable because other compilers define it too:
#ifdef __GNUC__
	#define __V_GCC__
#endif

#ifdef __TINYC__
	#undef __V_GCC__
#endif

#ifdef __cplusplus
	#undef __V_GCC__
#endif

#ifdef __clang__
	#undef __V_GCC__
#endif

#ifdef _MSC_VER
	#undef __V_GCC__
#endif

#ifdef __TINYC__
	#undef EMPTY_STRUCT_DECLARATION
	#undef EMPTY_STRUCT_INITIALIZATION
	#define EMPTY_STRUCT_DECLARATION char _dummy
	#define EMPTY_STRUCT_INITIALIZATION 0
	#undef EMPTY_ARRAY_OF_ELEMS
	#define EMPTY_ARRAY_OF_ELEMS(x,n) (x[n])
	#undef __NOINLINE
	#undef __IRQHANDLER
	// tcc does not support inlining at all
	#define __NOINLINE
	#define __IRQHANDLER
	#undef TCCSKIP
	#define TCCSKIP(x)
	// #include <byteswap.h>
	#ifndef _WIN32
		#include <execinfo.h>
		int tcc_backtrace(const char *fmt, ...);
	#endif
#endif


// for __offset_of
#ifndef __offsetof
	#define __offsetof(s,memb) ((size_t)((char *)&((s *)0)->memb - (char *)0))
#endif

#define OPTION_CAST(x) (x)

#ifndef V64_PRINTFORMAT
	#ifdef PRIx64
		#define V64_PRINTFORMAT "0x%"PRIx64
	#elif defined(__WIN32__)
		#define V64_PRINTFORMAT "0x%I64x"
	#elif defined(__linux__) && defined(__LP64__)
		#define V64_PRINTFORMAT "0x%lx"
	#else
		#define V64_PRINTFORMAT "0x%llx"
	#endif
#endif


#ifdef _WIN32
	#define WINVER 0x0600
	#ifdef _WIN32_WINNT
		#undef _WIN32_WINNT
	#endif
	#define _WIN32_WINNT 0x0600
	#ifndef WIN32_FULL
	#define WIN32_LEAN_AND_MEAN
	#endif
	#ifndef _UNICODE
	#define _UNICODE
	#endif
	#ifndef UNICODE
	#define UNICODE
	#endif
	#include <windows.h>

	#include <io.h> // _waccess
	#include <direct.h> // _wgetcwd

	#ifdef _MSC_VER
		// On MSVC these are the same (as long as /volatile:ms is passed)
		#define _Atomic volatile

		// MSVC cannot parse some things properly
		#undef EMPTY_STRUCT_DECLARATION
		#undef OPTION_CAST

		#define EMPTY_STRUCT_DECLARATION int ____dummy_variable
		#define OPTION_CAST(x)
		#undef __NOINLINE
		#undef __IRQHANDLER
		#define __NOINLINE __declspec(noinline)
		#define __IRQHANDLER __declspec(naked)

		#include <dbghelp.h>
		#pragma comment(lib, "Dbghelp")
	#endif
#else
	#include <pthread.h>
	#ifndef PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP
		// musl does not have that
		#define pthread_rwlockattr_setkind_np(a, b)
	#endif
#endif

// g_live_info is used by live.info()
static void* g_live_info = NULL;

////////////////////////////////////// USEFUL C MACROS //////////////////////////////////////
//#define tos4(s, slen) ((string){.str=(s), .len=(slen)})
// `"" s` is used to enforce a string literal argument
#define _SLIT(s) ((string){.str=(byteptr)("" s), .len=(sizeof(s)-1), .is_lit=1})
// take the address of an rvalue
#define ADDR(type, expr) (&((type[]){expr}[0]))
#define _PUSH_MANY(arr, val, tmp, tmp_typ) {tmp_typ tmp = (val); array_push_many(arr, tmp.data, tmp.len);}
#define _IN_MAP(val, m) map_exists_1(m, val)

// unsigned/signed comparisons
static inline UInt8 _us32_gt(uint32_t a, int32_t b) { return a > INT32_MAX || (int32_t)a > b; }
static inline UInt8 _us32_ge(uint32_t a, int32_t b) { return a >= INT32_MAX || (int32_t)a >= b; }
static inline UInt8 _us32_eq(uint32_t a, int32_t b) { return a <= INT32_MAX && (int32_t)a == b; }
static inline UInt8 _us32_ne(uint32_t a, int32_t b) { return a > INT32_MAX || (int32_t)a != b; }
static inline UInt8 _us32_le(uint32_t a, int32_t b) { return a <= INT32_MAX && (int32_t)a <= b; }
static inline UInt8 _us32_lt(uint32_t a, int32_t b) { return a < INT32_MAX && (int32_t)a < b; }
static inline UInt8 _us64_gt(uint64_t a, int64_t b) { return a > INT64_MAX || (int64_t)a > b; }
static inline UInt8 _us64_ge(uint64_t a, int64_t b) { return a >= INT64_MAX || (int64_t)a >= b; }
static inline UInt8 _us64_eq(uint64_t a, int64_t b) { return a <= INT64_MAX && (int64_t)a == b; }
static inline UInt8 _us64_ne(uint64_t a, int64_t b) { return a > INT64_MAX || (int64_t)a != b; }
static inline UInt8 _us64_le(uint64_t a, int64_t b) { return a <= INT64_MAX && (int64_t)a <= b; }
static inline UInt8 _us64_lt(uint64_t a, int64_t b) { return a < INT64_MAX && (int64_t)a < b; }

#if defined(__MINGW32__) || defined(__MINGW64__) || (defined(_WIN32) && defined(__TINYC__))
	#undef PRId64
	#undef PRIi64
	#undef PRIo64
	#undef PRIu64
	#undef PRIx64
	#undef PRIX64
	#define PRId64 "lld"
	#define PRIi64 "lli"
	#define PRIo64 "llo"
	#define PRIu64 "llu"
	#define PRIx64 "llx"
	#define PRIX64 "llX"
#endif

////////////////////////////////////// GLOBALS  //////////////////////////////////////


#endif // HAZEL_LANG_COMPILER 