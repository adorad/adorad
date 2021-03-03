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
        #error HAZEL_ERROR_MESSAGE The C compiler can not find <inttypes.h> . Please install build-essentials
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
#include <stdio.h> // TODO(jasmcaus): Remove these includes - define all function signatures and types manually
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
static inline UInt8 _us32_gt(uint32_t a, int32_t b)    {   return a > INT32_MAX || (int32_t)a > b;      }
static inline UInt8 _us32_ge(uint32_t a, int32_t b)    {   return a >= INT32_MAX || (int32_t)a >= b;    }
static inline UInt8 _us32_eq(uint32_t a, int32_t b)    {   return a <= INT32_MAX && (int32_t)a == b;    }
static inline UInt8 _us32_ne(uint32_t a, int32_t b)    {   return a > INT32_MAX || (int32_t)a != b;     }
static inline UInt8 _us32_le(uint32_t a, int32_t b)    {   return a <= INT32_MAX && (int32_t)a <= b;    }
static inline UInt8 _us32_lt(uint32_t a, int32_t b)    {   return a < INT32_MAX && (int32_t)a < b;      }
static inline UInt8 _us64_gt(uint64_t a, int64_t b)    {   return a > INT64_MAX || (int64_t)a > b;      }
static inline UInt8 _us64_ge(uint64_t a, int64_t b)    {   return a >= INT64_MAX || (int64_t)a >= b;    }
static inline UInt8 _us64_eq(uint64_t a, int64_t b)    {   return a <= INT64_MAX && (int64_t)a == b;    }
static inline UInt8 _us64_ne(uint64_t a, int64_t b)    {   return a > INT64_MAX || (int64_t)a != b;     }
static inline UInt8 _us64_le(uint64_t a, int64_t b)    {   return a <= INT64_MAX && (int64_t)a <= b;    }
static inline UInt8 _us64_lt(uint64_t a, int64_t b)    {   return a < INT64_MAX && (int64_t)a < b;      }

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
//byte g_str_buf[1024];
UInt8* g_str_buf;
int load_so(BytePtr);
void reload_so();
void _vinit(int ___argc, VoidPtr ___argv);
void _vcleanup();
#define sigaction_size   sizeof(sigaction);
#define __TENSOR_LEN(a)  ( (sizeof(a)) / (sizeof(a[0])) )

////////////////////////////////////// GLOBALS  //////////////////////////////////////
// ============== wyhash ==============
//Author: Wang Yi
#ifndef wyhash_version_gamma
	#define wyhash_version_gamma
	#define WYHASH_CONDOM 0
	#include <stdint.h>
	#include <string.h>
	#if defined(_MSC_VER) && defined(_M_X64)
		#include <intrin.h>
		#pragma intrinsic(_umul128)
	#endif

	//const uint64_t _wyp0=0xa0761d6478bd642full, _wyp1=0xe7037ed1a0b428dbull;
	#define _wyp0 ((uint64_t)0xa0761d6478bd642full)
	#define _wyp1 ((uint64_t)0xe7037ed1a0b428dbull)

	#if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__) || defined(__TINYC__)
		#define _likely_(x) __builtin_expect(x, 1)
		#define _unlikely_(x) __builtin_expect((x), 0)
	#else
		#define _likely_(x) (x)
		#define _unlikely_(x) (x)
	#endif

	#if defined(TARGET_ORDER_IS_LITTLE)
		#define WYHASH_LITTLE_ENDIAN 1
	#elif defined(TARGET_ORDER_IS_BIG)
		#define WYHASH_LITTLE_ENDIAN 0
	#endif

	#if (WYHASH_LITTLE_ENDIAN)
		static inline uint64_t _wyr8(const uint8_t *p) { uint64_t v; memcpy(&v, p, 8); return v;}
		static inline uint64_t _wyr4(const uint8_t *p) { unsigned v; memcpy(&v, p, 4); return v;}
	#else
		#if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
			static inline uint64_t _wyr8(const uint8_t *p) { uint64_t v; memcpy(&v, p, 8); return __builtin_bswap64(v);}
			static inline uint64_t _wyr4(const uint8_t *p) { unsigned v; memcpy(&v, p, 4); return __builtin_bswap32(v);}
		#elif defined(_MSC_VER)
			static inline uint64_t _wyr8(const uint8_t *p) { uint64_t v; memcpy(&v, p, 8); return _byteswap_uint64(v);}
			static inline uint64_t _wyr4(const uint8_t *p) { unsigned v; memcpy(&v, p, 4); return _byteswap_ulong(v);}
		#elif defined(__TINYC__)
			static inline uint64_t _wyr8(const uint8_t *p) { uint64_t v; memcpy(&v, p, 8); return bswap_64(v);}
			static inline uint64_t _wyr4(const uint8_t *p) { unsigned v; memcpy(&v, p, 4); return bswap_32(v);}
		#endif
	#endif

	static inline uint64_t _wyr3(const uint8_t *p, unsigned k) { return (((uint64_t)p[0]) << 16) | (((uint64_t)p[k >> 1]) << 8) | p[k - 1];}
	static inline uint64_t _wyrotr(uint64_t v, unsigned k) { return (v >> k) | (v << (64 - k));}
	static inline void _wymix128(uint64_t A, uint64_t B, uint64_t *C, uint64_t *D){
		A^=*C;	B^=*D;
	#ifdef UNOFFICIAL_WYHASH_32BIT
		uint64_t hh=(A>>32)*(B>>32), hl=(A>>32)*(unsigned)B, lh=(unsigned)A*(B>>32), ll=(uint64_t)(unsigned)A*(unsigned)B;
		*C=_wyrotr(hl,32)^hh; *D=_wyrotr(lh,32)^ll;
	#else
		#ifdef __SIZEOF_INT128__
			__uint128_t r=A; r*=B; *C=(uint64_t)r; *D=(uint64_t)(r>>64);
		#elif defined(_MSC_VER) && defined(_M_X64)
			A=_umul128(A,B,&B); *C=A; *D=B;
		#else
			uint64_t ha=A>>32, hb=B>>32, la=(uint32_t)A, lb=(uint32_t)B, hi, lo;
			uint64_t rh=ha*hb, rm0=ha*lb, rm1=hb*la, rl=la*lb, t=rl+(rm0<<32), c=t<rl;
			lo=t+(rm1<<32); c+=lo<t; hi=rh+(rm0>>32)+(rm1>>32)+c;
			*C=lo;	*D=hi;
		#endif
	#endif
	}
	static inline uint64_t wyhash(const void *key, uint64_t len, uint64_t seed){
		const uint8_t *p=(const uint8_t *)key;
		uint64_t i=len, see1=seed;
		start:
		if (_likely_(i<=16)) {
	#ifndef	WYHASH_CONDOM
			uint64_t shift = (i<8)*((8-i)<<3);
			//WARNING: intended reading outside buffer, trading for speed.
			_wymix128((_wyr8(p)<<shift)^_wyp0, (_wyr8(p+i-8)>>shift)^_wyp1, &seed, &see1);
	#else
			if (_likely_(i<=8)) {
				if (_likely_(i>=4)) _wymix128(_wyr4(p)^_wyp0,_wyr4(p+i-4)^_wyp1, &seed, &see1);
				else if (_likely_(i)) _wymix128(_wyr3(p,i)^_wyp0,_wyp1, &seed, &see1);
				else _wymix128(_wyp0,_wyp1, &seed, &see1);
			}
			else _wymix128(_wyr8(p)^_wyp0,_wyr8(p+i-8)^_wyp1, &seed, &see1);
	#endif
			_wymix128(len,_wyp0, &seed, &see1);
			return	seed^see1;
		}
		_wymix128(_wyr8(p)^_wyp0,_wyr8(p+8)^_wyp1, &seed, &see1);
		i-=16;	p+=16;	goto start;
	}
	static inline uint64_t wyhash64(uint64_t A, uint64_t B){
		_wymix128(_wyp0,_wyp1,&A,&B);
		_wymix128(0,0,&A,&B);
		return	A^B;
	}
	static inline uint64_t wyrand(uint64_t *seed){
		*seed+=_wyp0;
		uint64_t	a=0, b=0;
		_wymix128(*seed,*seed^_wyp1,&a,&b);
		return	a^b;
	}
	static inline double wy2u01(uint64_t r) {
		const double _wynorm=1.0/(1ull<<52);
		return (r>>12)*_wynorm;
	}
	static inline double wy2gau(uint64_t r) {
		const double _wynorm=1.0/(1ull<<20);
		return ((r&0x1fffff)+((r>>21)&0x1fffff)+((r>>42)&0x1fffff))*_wynorm-3.0;
	}
#endif

voidptr memdup(voidptr src, int sz);
static voidptr memfreedup(voidptr ptr, voidptr src, int sz) {
	free(ptr);
	return memdup(src, sz);
}


// V includes:

#if defined(__TINYC__) && defined(__has_include)
// tcc does not support has_include properly yet, turn it off completely
#undef __has_include
#endif
// added by module `builtin`:

#if defined(__has_include)

#if __has_include(<float.h>)
#include <float.h>
#else
#error VERROR_MESSAGE Header file <float.h>, needed for module `builtin` was not found. Please install the corresponding development headers.
#endif

#else
#include <float.h>
#endif

// added by module `os`:

#if defined(__has_include)

#if __has_include(<sys/stat.h>)
#include <sys/stat.h>
#else
#error VERROR_MESSAGE Header file <sys/stat.h>, needed for module `os` was not found. Please install the corresponding development headers.
#endif

#else
#include <sys/stat.h>
#endif

// added by module `os`:
#include <errno.h>
// added by module `os`:

#if defined(__has_include)

#if __has_include(<dirent.h>)
#include <dirent.h>
#else
#error VERROR_MESSAGE Header file <dirent.h>, needed for module `os` was not found. Please install the corresponding development headers.
#endif

#else
#include <dirent.h>
#endif

// added by module `os`:

#if defined(__has_include)

#if __has_include(<unistd.h>)
#include <unistd.h>
#else
#error VERROR_MESSAGE Header file <unistd.h>, needed for module `os` was not found. Please install the corresponding development headers.
#endif

#else
#include <unistd.h>
#endif

// added by module `os`:

#if defined(__has_include)

#if __has_include(<fcntl.h>)
#include <fcntl.h>
#else
#error VERROR_MESSAGE Header file <fcntl.h>, needed for module `os` was not found. Please install the corresponding development headers.
#endif

#else
#include <fcntl.h>
#endif

// added by module `os`:

#if defined(__has_include)

#if __has_include(<sys/utsname.h>)
#include <sys/utsname.h>
#else
#error VERROR_MESSAGE Header file <sys/utsname.h>, needed for module `os` was not found. Please install the corresponding development headers.
#endif

#else
#include <sys/utsname.h>
#endif

// added by module `time`:

#if defined(__has_include)

#if __has_include(<time.h>)
#include <time.h>
#else
#error VERROR_MESSAGE Header file <time.h>, needed for module `time` was not found. Please install the corresponding development headers.
#endif

#else
#include <time.h>
#endif

// added by module `time`:

#if defined(__has_include)

#if __has_include(<time.h>)
#include <time.h>
#else
#error VERROR_MESSAGE Header file <time.h>, needed for module `time` was not found. Please install the corresponding development headers.
#endif

#else
#include <time.h>
#endif

// added by module `term`:

#if defined(__has_include)

#if __has_include(<sys/ioctl.h>)
#include <sys/ioctl.h>
#else
#error VERROR_MESSAGE Header file <sys/ioctl.h>, needed for module `term` was not found. Please install the corresponding development headers.
#endif

#else
#include <sys/ioctl.h>
#endif

// added by module `term`:

#if defined(__has_include)

#if __has_include(<termios.h>)
#include <termios.h>
#else
#error VERROR_MESSAGE Header file <termios.h>, needed for module `term` was not found. Please install the corresponding development headers.
#endif

#else
#include <termios.h>
#endif


// Enum definitions:
typedef enum {
	strconv__Char_parse_state_start, // 
	strconv__Char_parse_state_norm_char, // +1
	strconv__Char_parse_state_field_char, // +2
	strconv__Char_parse_state_pad_ch, // +3
	strconv__Char_parse_state_len_set_start, // +4
	strconv__Char_parse_state_len_set_in, // +5
	strconv__Char_parse_state_check_type, // +6
	strconv__Char_parse_state_check_float, // +7
	strconv__Char_parse_state_check_float_in, // +8
	strconv__Char_parse_state_reset_params, // +9
} strconv__Char_parse_state;

typedef enum {
	strconv__Align_text_right = 0, // 0
	strconv__Align_text_left, // 0+1
	strconv__Align_text_center, // 0+2
} strconv__Align_text;

typedef enum {
	ChanState_success, // 
	ChanState_not_ready, // +1
	ChanState_closed, // +2
} ChanState;

typedef enum {
	os__FileType_regular, // 
	os__FileType_directory, // +1
	os__FileType_character_device, // +2
	os__FileType_block_device, // +3
	os__FileType_fifo, // +4
	os__FileType_symbolic_link, // +5
	os__FileType_socket, // +6
} os__FileType;

typedef enum {
	os__ProcessState_not_started, // 
	os__ProcessState_running, // +1
	os__ProcessState_stopped, // +2
	os__ProcessState_exited, // +3
	os__ProcessState_aborted, // +4
} os__ProcessState;

typedef enum {
	time__FormatTime_hhmm12, // 
	time__FormatTime_hhmm24, // +1
	time__FormatTime_hhmmss12, // +2
	time__FormatTime_hhmmss24, // +3
	time__FormatTime_hhmmss24_milli, // +4
	time__FormatTime_hhmmss24_micro, // +5
	time__FormatTime_no_time, // +6
} time__FormatTime;

typedef enum {
	time__FormatDate_ddmmyy, // 
	time__FormatDate_ddmmyyyy, // +1
	time__FormatDate_mmddyy, // +2
	time__FormatDate_mmddyyyy, // +3
	time__FormatDate_mmmd, // +4
	time__FormatDate_mmmdd, // +5
	time__FormatDate_mmmddyyyy, // +6
	time__FormatDate_no_date, // +7
	time__FormatDate_yyyymmdd, // +8
} time__FormatDate;

typedef enum {
	time__FormatDelimiter_dot, // 
	time__FormatDelimiter_hyphen, // +1
	time__FormatDelimiter_slash, // +2
	time__FormatDelimiter_space, // +3
	time__FormatDelimiter_no_delimiter, // +4
} time__FormatDelimiter;

typedef enum {
	v__token__Kind_unknown, // 
	v__token__Kind_eof, // +1
	v__token__Kind_name, // +2
	v__token__Kind_number, // +3
	v__token__Kind_string, // +4
	v__token__Kind_str_inter, // +5
	v__token__Kind_chartoken, // +6
	v__token__Kind_plus, // +7
	v__token__Kind_minus, // +8
	v__token__Kind_mul, // +9
	v__token__Kind_div, // +10
	v__token__Kind_mod, // +11
	v__token__Kind_xor, // +12
	v__token__Kind_pipe, // +13
	v__token__Kind_inc, // +14
	v__token__Kind_dec, // +15
	v__token__Kind_and, // +16
	v__token__Kind_logical_or, // +17
	v__token__Kind_not, // +18
	v__token__Kind_bit_not, // +19
	v__token__Kind_question, // +20
	v__token__Kind_comma, // +21
	v__token__Kind_semicolon, // +22
	v__token__Kind_colon, // +23
	v__token__Kind_arrow, // +24
	v__token__Kind_amp, // +25
	v__token__Kind_hash, // +26
	v__token__Kind_dollar, // +27
	v__token__Kind_at, // +28
	v__token__Kind_str_dollar, // +29
	v__token__Kind_left_shift, // +30
	v__token__Kind_right_shift, // +31
	v__token__Kind_not_in, // +32
	v__token__Kind_not_is, // +33
	v__token__Kind_assign, // +34
	v__token__Kind_decl_assign, // +35
	v__token__Kind_plus_assign, // +36
	v__token__Kind_minus_assign, // +37
	v__token__Kind_div_assign, // +38
	v__token__Kind_mult_assign, // +39
	v__token__Kind_xor_assign, // +40
	v__token__Kind_mod_assign, // +41
	v__token__Kind_or_assign, // +42
	v__token__Kind_and_assign, // +43
	v__token__Kind_right_shift_assign, // +44
	v__token__Kind_left_shift_assign, // +45
	v__token__Kind_lcbr, // +46
	v__token__Kind_rcbr, // +47
	v__token__Kind_lpar, // +48
	v__token__Kind_rpar, // +49
	v__token__Kind_lsbr, // +50
	v__token__Kind_rsbr, // +51
	v__token__Kind_eq, // +52
	v__token__Kind_ne, // +53
	v__token__Kind_gt, // +54
	v__token__Kind_lt, // +55
	v__token__Kind_ge, // +56
	v__token__Kind_le, // +57
	v__token__Kind_comment, // +58
	v__token__Kind_nl, // +59
	v__token__Kind_dot, // +60
	v__token__Kind_dotdot, // +61
	v__token__Kind_ellipsis, // +62
	v__token__Kind_keyword_beg, // +63
	v__token__Kind_key_as, // +64
	v__token__Kind_key_asm, // +65
	v__token__Kind_key_assert, // +66
	v__token__Kind_key_atomic, // +67
	v__token__Kind_key_break, // +68
	v__token__Kind_key_const, // +69
	v__token__Kind_key_continue, // +70
	v__token__Kind_key_defer, // +71
	v__token__Kind_key_else, // +72
	v__token__Kind_key_enum, // +73
	v__token__Kind_key_false, // +74
	v__token__Kind_key_for, // +75
	v__token__Kind_key_fn, // +76
	v__token__Kind_key_global, // +77
	v__token__Kind_key_go, // +78
	v__token__Kind_key_goto, // +79
	v__token__Kind_key_if, // +80
	v__token__Kind_key_import, // +81
	v__token__Kind_key_in, // +82
	v__token__Kind_key_interface, // +83
	v__token__Kind_key_is, // +84
	v__token__Kind_key_match, // +85
	v__token__Kind_key_module, // +86
	v__token__Kind_key_mut, // +87
	v__token__Kind_key_shared, // +88
	v__token__Kind_key_lock, // +89
	v__token__Kind_key_rlock, // +90
	v__token__Kind_key_none, // +91
	v__token__Kind_key_return, // +92
	v__token__Kind_key_select, // +93
	v__token__Kind_key_sizeof, // +94
	v__token__Kind_key_likely, // +95
	v__token__Kind_key_unlikely, // +96
	v__token__Kind_key_offsetof, // +97
	v__token__Kind_key_struct, // +98
	v__token__Kind_key_true, // +99
	v__token__Kind_key_type, // +100
	v__token__Kind_key_typeof, // +101
	v__token__Kind_key_orelse, // +102
	v__token__Kind_key_union, // +103
	v__token__Kind_key_pub, // +104
	v__token__Kind_key_static, // +105
	v__token__Kind_key_unsafe, // +106
	v__token__Kind_keyword_end, // +107
	v__token__Kind__end_, // +108
} v__token__Kind;

typedef enum {
	v__token__AtKind_unknown, // 
	v__token__AtKind_fn_name, // +1
	v__token__AtKind_method_name, // +2
	v__token__AtKind_mod_name, // +3
	v__token__AtKind_struct_name, // +4
	v__token__AtKind_vexe_path, // +5
	v__token__AtKind_file_path, // +6
	v__token__AtKind_line_nr, // +7
	v__token__AtKind_column_nr, // +8
	v__token__AtKind_vhash, // +9
	v__token__AtKind_vmod_file, // +10
} v__token__AtKind;

typedef enum {
	v__token__Precedence_lowest, // 
	v__token__Precedence_cond, // +1
	v__token__Precedence_in_as, // +2
	v__token__Precedence_assign, // +3
	v__token__Precedence_eq, // +4
	v__token__Precedence_sum, // +5
	v__token__Precedence_product, // +6
	v__token__Precedence_prefix, // +7
	v__token__Precedence_postfix, // +8
	v__token__Precedence_call, // +9
	v__token__Precedence_index, // +10
} v__token__Precedence;

typedef enum {
	semver__Operator_gt, // 
	semver__Operator_lt, // +1
	semver__Operator_ge, // +2
	semver__Operator_le, // +3
	semver__Operator_eq, // +4
} semver__Operator;

typedef enum {
	semver__Increment_major, // 
	semver__Increment_minor, // +1
	semver__Increment_patch, // +2
} semver__Increment;

typedef enum {
	v__vmod__TokenKind_module_keyword, // 
	v__vmod__TokenKind_field_key, // +1
	v__vmod__TokenKind_lcbr, // +2
	v__vmod__TokenKind_rcbr, // +3
	v__vmod__TokenKind_labr, // +4
	v__vmod__TokenKind_rabr, // +5
	v__vmod__TokenKind_comma, // +6
	v__vmod__TokenKind_colon, // +7
	v__vmod__TokenKind_eof, // +8
	v__vmod__TokenKind_str, // +9
	v__vmod__TokenKind_ident, // +10
	v__vmod__TokenKind_unknown, // +11
} v__vmod__TokenKind;

typedef enum {
	v__errors__Reporter_scanner, // 
	v__errors__Reporter_parser, // +1
	v__errors__Reporter_checker, // +2
	v__errors__Reporter_gen, // +3
} v__errors__Reporter;

typedef enum {
	vweb__tmpl__State_html, // 
	vweb__tmpl__State_css, // +1
	vweb__tmpl__State_js, // +2
} vweb__tmpl__State;

typedef enum {
	v__vet__ErrorKind_error, // 
	v__vet__ErrorKind_warning, // +1
} v__vet__ErrorKind;

typedef enum {
	v__vet__FixKind_unknown, // 
	v__vet__FixKind_doc, // +1
	v__vet__FixKind_vfmt, // +2
} v__vet__FixKind;

typedef enum {
	v__pref__OS__auto, // 
	v__pref__OS_ios, // +1
	v__pref__OS_macos, // +2
	v__pref__OS_linux, // +3
	v__pref__OS_windows, // +4
	v__pref__OS_freebsd, // +5
	v__pref__OS_openbsd, // +6
	v__pref__OS_netbsd, // +7
	v__pref__OS_dragonfly, // +8
	v__pref__OS_js, // +9
	v__pref__OS_android, // +10
	v__pref__OS_solaris, // +11
	v__pref__OS_haiku, // +12
} v__pref__OS;

typedef enum {
	v__pref__BuildMode_default_mode, // 
	v__pref__BuildMode_build_module, // +1
} v__pref__BuildMode;

typedef enum {
	v__pref__OutputMode_stdout, // 
	v__pref__OutputMode_silent, // +1
} v__pref__OutputMode;

typedef enum {
	v__pref__ColorOutput_auto, // 
	v__pref__ColorOutput_always, // +1
	v__pref__ColorOutput_never, // +2
} v__pref__ColorOutput;

typedef enum {
	v__pref__Backend_c, // 
	v__pref__Backend_js, // +1
	v__pref__Backend_x64, // +2
} v__pref__Backend;

typedef enum {
	v__pref__CompilerType_gcc, // 
	v__pref__CompilerType_tinyc, // +1
	v__pref__CompilerType_clang, // +2
	v__pref__CompilerType_mingw, // +3
	v__pref__CompilerType_msvc, // +4
	v__pref__CompilerType_cplusplus, // +5
} v__pref__CompilerType;

typedef enum {
	v__table__Language_v, // 
	v__table__Language_c, // +1
	v__table__Language_js, // +2
} v__table__Language;

typedef enum {
	v__table__TypeFlag_optional, // 
	v__table__TypeFlag_variadic, // +1
	v__table__TypeFlag_generic, // +2
	v__table__TypeFlag_shared_f, // +3
	v__table__TypeFlag_atomic_f, // +4
} v__table__TypeFlag;

typedef enum {
	v__table__ShareType_mut_t, // 
	v__table__ShareType_shared_t, // +1
	v__table__ShareType_atomic_t, // +2
} v__table__ShareType;

typedef enum {
	v__table__Kind_placeholder, // 
	v__table__Kind_void, // +1
	v__table__Kind_voidptr, // +2
	v__table__Kind_byteptr, // +3
	v__table__Kind_charptr, // +4
	v__table__Kind_i8, // +5
	v__table__Kind_i16, // +6
	v__table__Kind_int, // +7
	v__table__Kind_i64, // +8
	v__table__Kind_byte, // +9
	v__table__Kind_u16, // +10
	v__table__Kind_u32, // +11
	v__table__Kind_u64, // +12
	v__table__Kind_f32, // +13
	v__table__Kind_f64, // +14
	v__table__Kind_char, // +15
	v__table__Kind_size_t, // +16
	v__table__Kind_rune, // +17
	v__table__Kind_bool, // +18
	v__table__Kind_none_, // +19
	v__table__Kind_string, // +20
	v__table__Kind_ustring, // +21
	v__table__Kind_array, // +22
	v__table__Kind_array_fixed, // +23
	v__table__Kind_map, // +24
	v__table__Kind_chan, // +25
	v__table__Kind_any, // +26
	v__table__Kind_struct_, // +27
	v__table__Kind_generic_struct_inst, // +28
	v__table__Kind_multi_return, // +29
	v__table__Kind_sum_type, // +30
	v__table__Kind_alias, // +31
	v__table__Kind_enum_, // +32
	v__table__Kind_function, // +33
	v__table__Kind_interface_, // +34
	v__table__Kind_float_literal, // +35
	v__table__Kind_int_literal, // +36
	v__table__Kind_aggregate, // +37
	v__table__Kind_thread, // +38
} v__table__Kind;

typedef enum {
	v__scanner__CommentsMode_skip_comments, // 
	v__scanner__CommentsMode_parse_comments, // +1
	v__scanner__CommentsMode_toplevel_comments, // +2
} v__scanner__CommentsMode;

typedef enum {
	v__ast__IdentKind_unresolved, // 
	v__ast__IdentKind_blank_ident, // +1
	v__ast__IdentKind_variable, // +2
	v__ast__IdentKind_constant, // +3
	v__ast__IdentKind_global, // +4
	v__ast__IdentKind_function, // +5
} v__ast__IdentKind;

typedef enum {
	v__ast__CompForKind_methods, // 
	v__ast__CompForKind_fields, // +1
} v__ast__CompForKind;

typedef enum {
	v__ast__OrKind_absent, // 
	v__ast__OrKind_block, // +1
	v__ast__OrKind_propagate, // +2
} v__ast__OrKind;

typedef enum {
	v__ast__SqlStmtKind_insert, // 
	v__ast__SqlStmtKind_update, // +1
	v__ast__SqlStmtKind_delete, // +2
} v__ast__SqlStmtKind;

typedef enum {
	v__gen__c__SqlExprSide_left, // 
	v__gen__c__SqlExprSide_right, // +1
} v__gen__c__SqlExprSide;

typedef enum {
	v__gen__c__SqlType_sqlite3, // 
	v__gen__c__SqlType_mysql, // +1
	v__gen__c__SqlType_psql, // +2
	v__gen__c__SqlType_unknown, // +3
} v__gen__c__SqlType;

typedef enum {
	v__gen__x64__SectionType_null = 0, // 0
	v__gen__x64__SectionType_progbits = 1, // 1
	v__gen__x64__SectionType_symtab = 2, // 2
	v__gen__x64__SectionType_strtab = 3, // 3
	v__gen__x64__SectionType_rela = 4, // 4
} v__gen__x64__SectionType;

typedef enum {
	v__gen__x64__Register_rax, // 
	v__gen__x64__Register_rcx, // +1
	v__gen__x64__Register_rdx, // +2
	v__gen__x64__Register_rbx, // +3
	v__gen__x64__Register_rsp, // +4
	v__gen__x64__Register_rbp, // +5
	v__gen__x64__Register_rsi, // +6
	v__gen__x64__Register_rdi, // +7
	v__gen__x64__Register_eax, // +8
	v__gen__x64__Register_edi, // +9
	v__gen__x64__Register_edx, // +10
	v__gen__x64__Register_r8, // +11
	v__gen__x64__Register_r9, // +12
	v__gen__x64__Register_r10, // +13
	v__gen__x64__Register_r11, // +14
	v__gen__x64__Register_r12, // +15
	v__gen__x64__Register_r13, // +16
	v__gen__x64__Register_r14, // +17
	v__gen__x64__Register_r15, // +18
} v__gen__x64__Register;

typedef enum {
	v__gen__x64__Size__8, // 
	v__gen__x64__Size__16, // +1
	v__gen__x64__Size__32, // +2
	v__gen__x64__Size__64, // +3
} v__gen__x64__Size;


// V type definitions:
struct string {
	byteptr str;
	int len;
	int is_lit;
};

struct array {
	int element_size;
	voidptr data;
	int len;
	int cap;
};

struct DenseArray {
	int key_bytes;
	int value_bytes;
	int slot_bytes;
	int cap;
	int len;
	u32 deletes;
	byte* all_deleted;
	byteptr data;
};

struct map {
	int key_bytes;
	int value_bytes;
	u32 even_index;
	byte cached_hashbits;
	byte shift;
	DenseArray key_values;
	u32* metas;
	u32 extra_metas;
	bool has_string_keys;
	MapHashFn hash_fn;
	MapEqFn key_eq_fn;
	MapCloneFn clone_fn;
	MapFreeFn free_fn;
	int len;
};

struct Option {
	bool ok;
	bool is_none;
	string v_error;
	int ecode;
};

struct Error {
	string msg;
	int code;
};

struct Option2 {
	byte state;
	Error err;
};

typedef array Array_string;
typedef array Array_byte;
typedef array Array_int;
typedef int (*anon_fn_int_int__int)(int,int);
typedef array Array_voidptr;
typedef void (*FnExitCb)();
typedef array Array_VCastTypeIndexName;
typedef array Array_MethodArgs;
typedef array Array_charptr;
typedef array Array_byteptr;
typedef array Array_RepIndex;
typedef map Map_string_int;
typedef struct {
	void* _object;
	int _interface_idx;
} hash__Hasher;
typedef struct {
	void* _object;
	int _interface_idx;
} hash__Hash32er;
typedef struct {
	void* _object;
	int _interface_idx;
} hash__Hash64er;
typedef map Map_string_string;
typedef array Array_ustring;
typedef void (*anon_fn_string)(string);
typedef array Array_T;
typedef map Map_string_Array_string;
typedef array Array_v__util__Possibility;
typedef map Map_string_time__StopWatch;
typedef array Array_v__ast__File;
typedef array Array_v__builder__FunctionRedefinition;
typedef array Array_v__cflag__CFlag;
typedef voidptr v__builder__RegKey;
typedef array Array_u32;
typedef struct {
	void* _object;
	int _interface_idx;
} rand__PRNG;
typedef i64 time__Duration;
typedef voidptr time__time_t;
typedef string (*anon_fn_string__string)(string);
typedef map Map_string_v__token__Kind;
typedef array Array_v__token__Precedence;
typedef array Array_v__vmod__Token;
typedef map Map_string_v__vmod__ModFileAndFolder;
typedef int v__table__Type;
typedef array Array_v__ast__Stmt;
typedef array Array_v__ast__Comment;
typedef array Array_v__ast__Expr;
typedef array Array_bool;
typedef array Array_v__token__Position;
typedef array Array_v__table__Type;
typedef array Array_v__table__Attr;
typedef array Array_v__ast__ConstField;
typedef array Array_v__ast__Embed;
typedef array Array_v__ast__StructField;
typedef array Array_v__ast__FnDecl;
typedef array Array_v__ast__StructInitField;
typedef array Array_v__ast__StructInitEmbed;
typedef array Array_v__ast__ImportSymbol;
typedef array Array_v__table__Param;
typedef array Array_v__ast__GenericParam;
typedef array Array_v__ast__DeferStmt;
typedef array Array_v__ast__CallArg;
typedef array Array_v__ast__GlobalField;
typedef array Array_v__ast__Import;
typedef array Array_v__ast__EmbeddedFile;
typedef array Array_v__errors__Error;
typedef array Array_v__errors__Warning;
typedef array Array_v__ast__FnDecl_ptr;
typedef array Array_v__ast__IfBranch;
typedef array Array_v__ast__Ident;
typedef array Array_v__ast__MatchBranch;
typedef array Array_Array_v__ast__Comment;
typedef array Array_v__ast__SelectBranch;
typedef array Array_v__ast__EnumField;
typedef array Array_v__ast__SumTypeVariant;
typedef array Array_v__table__Field;
typedef map Map_int_v__ast__SqlStmt;
typedef map Map_int_v__ast__SqlExpr;
typedef array Array_v__ast__Node;
typedef map Map_string_v__ast__ScopeObject;
typedef array Array_v__ast__ScopeStructField;
typedef array Array_v__ast__Scope_ptr;
typedef array Array_v__table__TypeSymbol;
typedef map Map_string_v__table__Fn;
typedef array Array_Array_v__table__Type;
typedef map Map_string_Array_Array_v__table__Type;
typedef map Map_string_bool;
typedef array Array_v__table__Fn;
typedef array Array_v__table__Kind;
typedef map Map_string_v__table__Type;
typedef array Array_v__ast__File_ptr;
typedef array Array_i64;
typedef array Array_v__ast__Type;
typedef array Array_v__vet__Error;
typedef array Array_v__depgraph__DepGraphNode;
typedef map Map_string_v__depgraph__DepGraphNode;
typedef map Map_string_strings__Builder;
typedef array Array_v__gen__c__ProfileCounterMeta;
typedef map Map_string_v__ast__FnDecl;
typedef map Map_string_v__ast__ConstField;
typedef map Map_string_Array_v__ast__FnDecl;
typedef map Map_string_v__gen__js__Namespace_ptr;
typedef map Map_string_i64;
typedef array Array_v__token__Token;
typedef array Array_flag__Flag;
typedef array Array_f64;
typedef array Array_semver__Comparator;
typedef array Array_semver__ComparatorSet;
typedef array Array_u64;
typedef array Array_strconv__Uint128;
typedef array Array_rune;
typedef array Array_os__ProcessState;
typedef array Array_v__token__Kind;
typedef array Array_v__vmod__TokenKind;
typedef array Array_v__ast__IdentKind;
typedef array Array_v__table__TypeSymbol_ptr;
typedef array Array_v__gen__x64__Register;
typedef array Array_v__pref__OS;
// builtin types:
//------------------ #endbuiltin
typedef pthread_t __v_thread;
typedef string Array_fixed_string_11 [11];
struct os__Uname {
	string sysname;
	string nodename;
	string release;
	string version;
	string machine;
};

struct v__cflag__CFlag {
	string mod;
	string os;
	string name;
	string value;
	string cached;
};

struct v__builder__WindowsKit {
	string um_lib_path;
	string ucrt_lib_path;
	string um_include_path;
	string ucrt_include_path;
	string shared_include_path;
};

struct v__builder__VsInstallation {
	string include_path;
	string lib_path;
	string exe_path;
};

struct v__vmod__ModFileAndFolder {
	string vmod_file;
	string vmod_folder;
};


// Union sum type v__ast__TypeDecl = 
//          |  190 = v__ast__AliasTypeDecl
//          |  191 = v__ast__FnTypeDecl  
//          |  192 = v__ast__SumTypeDecl 
struct v__ast__TypeDecl {
    union {
        v__ast__AliasTypeDecl* _v__ast__AliasTypeDecl;
        v__ast__FnTypeDecl* _v__ast__FnTypeDecl;
        v__ast__SumTypeDecl* _v__ast__SumTypeDecl;
    };
    int typ;
};


// Union sum type v__ast__Expr = 
//          |  194 = v__ast__AnonFn      
//          |  195 = v__ast__ArrayDecompose
//          |  196 = v__ast__ArrayInit   
//          |  197 = v__ast__AsCast      
//          |  198 = v__ast__Assoc       
//          |  199 = v__ast__AtExpr      
//          |  200 = v__ast__BoolLiteral 
//          |  201 = v__ast__CTempVar    
//          |  202 = v__ast__CallExpr    
//          |  203 = v__ast__CastExpr    
//          |  204 = v__ast__ChanInit    
//          |  205 = v__ast__CharLiteral 
//          |  206 = v__ast__Comment     
//          |  207 = v__ast__ComptimeCall
//          |  208 = v__ast__ComptimeSelector
//          |  209 = v__ast__ConcatExpr  
//          |  210 = v__ast__EnumVal     
//          |  211 = v__ast__FloatLiteral
//          |  212 = v__ast__GoExpr      
//          |  213 = v__ast__Ident       
//          |  214 = v__ast__IfExpr      
//          |  215 = v__ast__IfGuardExpr 
//          |  216 = v__ast__IndexExpr   
//          |  217 = v__ast__InfixExpr   
//          |  218 = v__ast__IntegerLiteral
//          |  219 = v__ast__Likely      
//          |  220 = v__ast__LockExpr    
//          |  221 = v__ast__MapInit     
//          |  222 = v__ast__MatchExpr   
//          |  223 = v__ast__None        
//          |  224 = v__ast__OffsetOf    
//          |  225 = v__ast__OrExpr      
//          |  226 = v__ast__ParExpr     
//          |  227 = v__ast__PostfixExpr 
//          |  228 = v__ast__PrefixExpr  
//          |  229 = v__ast__RangeExpr   
//          |  230 = v__ast__SelectExpr  
//          |  231 = v__ast__SelectorExpr
//          |  232 = v__ast__SizeOf      
//          |  233 = v__ast__SqlExpr     
//          |  234 = v__ast__StringInterLiteral
//          |  235 = v__ast__StringLiteral
//          |  236 = v__ast__StructInit  
//          |  237 = v__ast__Type        
//          |  238 = v__ast__TypeOf      
//          |  239 = v__ast__UnsafeExpr  
struct v__ast__Expr {
    union {
        v__ast__AnonFn* _v__ast__AnonFn;
        v__ast__ArrayDecompose* _v__ast__ArrayDecompose;
        v__ast__ArrayInit* _v__ast__ArrayInit;
        v__ast__AsCast* _v__ast__AsCast;
        v__ast__Assoc* _v__ast__Assoc;
        v__ast__AtExpr* _v__ast__AtExpr;
        v__ast__BoolLiteral* _v__ast__BoolLiteral;
        v__ast__CTempVar* _v__ast__CTempVar;
        v__ast__CallExpr* _v__ast__CallExpr;
        v__ast__CastExpr* _v__ast__CastExpr;
        v__ast__ChanInit* _v__ast__ChanInit;
        v__ast__CharLiteral* _v__ast__CharLiteral;
        v__ast__Comment* _v__ast__Comment;
        v__ast__ComptimeCall* _v__ast__ComptimeCall;
        v__ast__ComptimeSelector* _v__ast__ComptimeSelector;
        v__ast__ConcatExpr* _v__ast__ConcatExpr;
        v__ast__EnumVal* _v__ast__EnumVal;
        v__ast__FloatLiteral* _v__ast__FloatLiteral;
        v__ast__GoExpr* _v__ast__GoExpr;
        v__ast__Ident* _v__ast__Ident;
        v__ast__IfExpr* _v__ast__IfExpr;
        v__ast__IfGuardExpr* _v__ast__IfGuardExpr;
        v__ast__IndexExpr* _v__ast__IndexExpr;
        v__ast__InfixExpr* _v__ast__InfixExpr;
        v__ast__IntegerLiteral* _v__ast__IntegerLiteral;
        v__ast__Likely* _v__ast__Likely;
        v__ast__LockExpr* _v__ast__LockExpr;
        v__ast__MapInit* _v__ast__MapInit;
        v__ast__MatchExpr* _v__ast__MatchExpr;
        v__ast__None* _v__ast__None;
        v__ast__OffsetOf* _v__ast__OffsetOf;
        v__ast__OrExpr* _v__ast__OrExpr;
        v__ast__ParExpr* _v__ast__ParExpr;
        v__ast__PostfixExpr* _v__ast__PostfixExpr;
        v__ast__PrefixExpr* _v__ast__PrefixExpr;
        v__ast__RangeExpr* _v__ast__RangeExpr;
        v__ast__SelectExpr* _v__ast__SelectExpr;
        v__ast__SelectorExpr* _v__ast__SelectorExpr;
        v__ast__SizeOf* _v__ast__SizeOf;
        v__ast__SqlExpr* _v__ast__SqlExpr;
        v__ast__StringInterLiteral* _v__ast__StringInterLiteral;
        v__ast__StringLiteral* _v__ast__StringLiteral;
        v__ast__StructInit* _v__ast__StructInit;
        v__ast__Type* _v__ast__Type;
        v__ast__TypeOf* _v__ast__TypeOf;
        v__ast__UnsafeExpr* _v__ast__UnsafeExpr;
    };
    int typ;
};


// Union sum type v__ast__Stmt = 
//          |  241 = v__ast__AssertStmt  
//          |  242 = v__ast__AssignStmt  
//          |  243 = v__ast__Block       
//          |  244 = v__ast__BranchStmt  
//          |  245 = v__ast__CompFor     
//          |  246 = v__ast__ConstDecl   
//          |  247 = v__ast__DeferStmt   
//          |  248 = v__ast__EnumDecl    
//          |  249 = v__ast__ExprStmt    
//          |  141 = v__ast__FnDecl      
//          |  250 = v__ast__ForCStmt    
//          |  251 = v__ast__ForInStmt   
//          |  252 = v__ast__ForStmt     
//          |  253 = v__ast__GlobalDecl  
//          |  254 = v__ast__GoStmt      
//          |  255 = v__ast__GotoLabel   
//          |  256 = v__ast__GotoStmt    
//          |  257 = v__ast__HashStmt    
//          |  258 = v__ast__Import      
//          |  259 = v__ast__InterfaceDecl
//          |  260 = v__ast__Module      
//          |  261 = v__ast__Return      
//          |  262 = v__ast__SqlStmt     
//          |  263 = v__ast__StructDecl  
//          |  193 = v__ast__TypeDecl    
struct v__ast__Stmt {
    union {
        v__ast__AssertStmt* _v__ast__AssertStmt;
        v__ast__AssignStmt* _v__ast__AssignStmt;
        v__ast__Block* _v__ast__Block;
        v__ast__BranchStmt* _v__ast__BranchStmt;
        v__ast__CompFor* _v__ast__CompFor;
        v__ast__ConstDecl* _v__ast__ConstDecl;
        v__ast__DeferStmt* _v__ast__DeferStmt;
        v__ast__EnumDecl* _v__ast__EnumDecl;
        v__ast__ExprStmt* _v__ast__ExprStmt;
        v__ast__FnDecl* _v__ast__FnDecl;
        v__ast__ForCStmt* _v__ast__ForCStmt;
        v__ast__ForInStmt* _v__ast__ForInStmt;
        v__ast__ForStmt* _v__ast__ForStmt;
        v__ast__GlobalDecl* _v__ast__GlobalDecl;
        v__ast__GoStmt* _v__ast__GoStmt;
        v__ast__GotoLabel* _v__ast__GotoLabel;
        v__ast__GotoStmt* _v__ast__GotoStmt;
        v__ast__HashStmt* _v__ast__HashStmt;
        v__ast__Import* _v__ast__Import;
        v__ast__InterfaceDecl* _v__ast__InterfaceDecl;
        v__ast__Module* _v__ast__Module;
        v__ast__Return* _v__ast__Return;
        v__ast__SqlStmt* _v__ast__SqlStmt;
        v__ast__StructDecl* _v__ast__StructDecl;
        v__ast__TypeDecl* _v__ast__TypeDecl;
    };
    int typ;
};


// Union sum type v__ast__ScopeObject = 
//          |  265 = v__ast__ConstField  
//          |  266 = v__ast__GlobalField 
//          |  267 = v__ast__Var         
struct v__ast__ScopeObject {
    union {
        v__ast__ConstField* _v__ast__ConstField;
        v__ast__GlobalField* _v__ast__GlobalField;
        v__ast__Var* _v__ast__Var;
    };
    int typ;
};


// Union sum type v__ast__Node = 
//          |  265 = v__ast__ConstField  
//          |  269 = v__ast__EnumField   
//          |  240 = v__ast__Expr        
//          |  270 = v__ast__Field       
//          |  132 = v__ast__File        
//          |  266 = v__ast__GlobalField 
//          |  271 = v__ast__IfBranch    
//          |  272 = v__ast__MatchBranch 
//          |  268 = v__ast__ScopeObject 
//          |  273 = v__ast__SelectBranch
//          |  264 = v__ast__Stmt        
//          |  274 = v__ast__StructField 
//          |  275 = v__ast__StructInitField
//          |  276 = v__table__Param     
struct v__ast__Node {
    union {
        v__ast__ConstField* _v__ast__ConstField;
        v__ast__EnumField* _v__ast__EnumField;
        v__ast__Expr* _v__ast__Expr;
        v__ast__Field* _v__ast__Field;
        v__ast__File* _v__ast__File;
        v__ast__GlobalField* _v__ast__GlobalField;
        v__ast__IfBranch* _v__ast__IfBranch;
        v__ast__MatchBranch* _v__ast__MatchBranch;
        v__ast__ScopeObject* _v__ast__ScopeObject;
        v__ast__SelectBranch* _v__ast__SelectBranch;
        v__ast__Stmt* _v__ast__Stmt;
        v__ast__StructField* _v__ast__StructField;
        v__ast__StructInitField* _v__ast__StructInitField;
        v__table__Param* _v__table__Param;
    };
    int typ;
};

struct v__ast__GenericParam {
	string name;
};

struct v__ast__EmbeddedFile {
	string rpath;
	string apath;
};


// Union sum type v__ast__IdentInfo = 
//          |  316 = v__ast__IdentFn     
//          |  317 = v__ast__IdentVar    
struct v__ast__IdentInfo {
    union {
        v__ast__IdentFn* _v__ast__IdentFn;
        v__ast__IdentVar* _v__ast__IdentVar;
    };
    int typ;
};


// Union sum type v__table__FExpr = 
//          |    3 = byteptr             
//          |    2 = voidptr             
struct v__table__FExpr {
    union {
        byteptr* _byteptr;
        voidptr* _voidptr;
    };
    int typ;
};

struct v__ast__Table {
EMPTY_STRUCT_DECLARATION;
};


// Union sum type v__table__TypeInfo = 
//          |  355 = v__table__Aggregate 
//          |  357 = v__table__Alias     
//          |  340 = v__table__Array     
//          |  360 = v__table__ArrayFixed
//          |  358 = v__table__Chan      
//          |  364 = v__table__Enum      
//          |  362 = v__table__FnType    
//          |  144 = v__table__GenericStructInst
//          |  356 = v__table__Interface 
//          |  341 = v__table__Map       
//          |  361 = v__table__MultiReturn
//          |  145 = v__table__Struct    
//          |  363 = v__table__SumType   
//          |  359 = v__table__Thread    
struct v__table__TypeInfo {
    union {
        v__table__Aggregate* _v__table__Aggregate;
        v__table__Alias* _v__table__Alias;
        v__table__Array* _v__table__Array;
        v__table__ArrayFixed* _v__table__ArrayFixed;
        v__table__Chan* _v__table__Chan;
        v__table__Enum* _v__table__Enum;
        v__table__FnType* _v__table__FnType;
        v__table__GenericStructInst* _v__table__GenericStructInst;
        v__table__Interface* _v__table__Interface;
        v__table__Map* _v__table__Map;
        v__table__MultiReturn* _v__table__MultiReturn;
        v__table__Struct* _v__table__Struct;
        v__table__SumType* _v__table__SumType;
        v__table__Thread* _v__table__Thread;
    };
    int typ;
};

struct v__gen__c__ProfileCounterMeta {
	string fn_name;
	string vpc_name;
	string vpc_calls;
};

struct v__gen__js__JsDoc {
	v__gen__js__JsGen* gen;
};

struct ustring {
	string s;
	Array_int runes;
	int len;
};

struct VCastTypeIndexName {
	int tindex;
	string tname;
};

struct VAssertMetaInfo {
	string fpath;
	int line_nr;
	string fn_name;
	string src;
	string op;
	string llabel;
	string rlabel;
	string lvalue;
	string rvalue;
};

struct MethodArgs {
	int typ;
};

struct FunctionData {
	string name;
	Array_string attrs;
	Array_MethodArgs args;
	int return_type;
	int typ;
};

struct FieldData {
	string name;
	Array_string attrs;
	bool is_pub;
	bool is_mut;
	int typ;
};

struct OptionBase {
	bool ok;
	bool is_none;
	string v_error;
	int ecode;
};

struct SortedMap {
	int value_bytes;
	mapnode* root;
	int len;
};

typedef voidptr Array_fixed_voidptr_11 [11];
struct RepIndex {
	int idx;
	int val_idx;
};

struct strings__Builder {
	Array_byte buf;
	int len;
	int initial_size;
};

struct strconv__PrepNumber {
	bool negative;
	int exponent;
	u64 mantissa;
};

union strconv__Float64u {
	f64 f;
	u64 u;
};

struct strconv__Dec32 {
	u32 m;
	int e;
};

union strconv__Uf32 {
	f32 f;
	u32 u;
};

struct strconv__Dec64 {
	u64 m;
	int e;
};

union strconv__Uf64 {
	f64 f;
	u64 u;
};

struct strconv__BF_param {
	byte pad_ch;
	int len0;
	int len1;
	bool positive;
	bool sign_flag;
	strconv__Align_text allign;
	bool rm_tail_zero;
};

struct strconv__Uint128 {
	u64 lo;
	u64 hi;
};

struct os__File {
	voidptr cfile;
	int fd;
	bool is_opened;
};

struct os__FileInfo {
	string name;
	int size;
};

struct os__FilePermission {
	bool read;
	bool write;
	bool execute;
};

struct os__Result {
	int exit_code;
	string output;
};

typedef char Array_fixed_char_256 [256];
struct os__Command {
	voidptr f;
	bool eof;
	string path;
	bool redirect_stdout;
};

typedef int Array_fixed_int_3 [3];
struct v__vcache__CacheManager {
	string basepath;
	string original_vopts;
	string vopts;
	Map_string_string k2cpath;
};

struct v__util__EManager {
	bool support_color;
};

struct v__token__Position {
	int len;
	int line_nr;
	int pos;
	int last_line;
};

struct v__util__Possibility {
	string value;
	string svalue;
	f32 similarity;
};

struct v__util__Suggestion {
	Array_v__util__Possibility known;
	string wanted;
	string swanted;
};

struct time__StopWatch {
	u64 elapsed;
	u64 start;
	u64 end;
};

struct v__util__Timers {
	Map_string_time__StopWatch swatches;
	bool should_print;
};

struct v__ast__Scope {
	Map_string_v__ast__ScopeObject objects;
	Array_v__ast__ScopeStructField struct_fields;
	v__ast__Scope* parent;
	bool detached_from_parent;
	Array_v__ast__Scope_ptr children;
	int start_pos;
	int end_pos;
};

struct v__builder__MsvcResult {
	string full_cl_exe_path;
	string exe_path;
	string um_lib_path;
	string ucrt_lib_path;
	string vs_lib_path;
	string um_include_path;
	string ucrt_include_path;
	string vs_include_path;
	string shared_include_path;
	bool valid;
};

struct v__table__Table {
	Array_v__table__TypeSymbol types;
	Map_string_int type_idxs;
	Map_string_v__table__Fn fns;
	Array_string imports;
	Array_string modules;
	Array_v__cflag__CFlag cflags;
	Array_string redefined_fns;
	Map_string_Array_Array_v__table__Type fn_gen_types;
	string cmod_prefix;
	bool is_fmt;
	Map_string_bool used_fns;
	Map_string_bool used_consts;
};

struct v__builder__CcompilerOptions {
	string guessed_compiler;
	string shared_postfix;
	bool debug_mode;
	bool is_cc_tcc;
	bool is_cc_gcc;
	bool is_cc_msvc;
	bool is_cc_clang;
	string env_cflags;
	string env_ldflags;
	Array_string args;
	Array_string wargs;
	Array_string pre_args;
	Array_string o_args;
	Array_string source_args;
	Array_string post_args;
	Array_string linker_flags;
};

struct v__depgraph__DepGraph {
	bool acyclic;
	Array_v__depgraph__DepGraphNode nodes;
};

struct v__table__GenericStructInst {
	int parent_idx;
	Array_v__table__Type generic_types;
};

struct v__table__Struct {
	Array_v__table__Attr attrs;
	Array_v__table__Type embeds;
	Array_v__table__Field fields;
	bool is_typedef;
	bool is_union;
	bool is_heap;
	Array_v__table__Type generic_types;
};

struct v__builder__MsvcStringFlags {
	Array_string real_libs;
	Array_string inc_paths;
	Array_string lib_paths;
	Array_string defines;
	Array_string other_flags;
};

struct rand__PRNGConfigStruct {
	Array_u32 seed;
};

struct rand__wyrand__WyRandRNG {
	u64 state;
	bool has_extra;
	u32 extra;
};

struct time__Time {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	int microsecond;
	u64 v_unix;
};

struct time__StopWatchOptions {
	bool auto_start;
};

struct term__Coord {
	int x;
	int y;
};

struct v__token__Token {
	v__token__Kind kind;
	string lit;
	int line_nr;
	int pos;
	int len;
	int tidx;
};

struct v__vmod__Manifest {
	string name;
	string version;
	string description;
	Array_string dependencies;
	string license;
	string repo_url;
	string author;
	Map_string_Array_string unknown;
};

struct v__vmod__Token {
	v__vmod__TokenKind typ;
	string val;
};

struct v__vmod__Scanner {
	int pos;
	string text;
	bool inside_text;
	Array_v__vmod__Token tokens;
};

struct v__vmod__ModFileCacher {
	Map_string_v__vmod__ModFileAndFolder cache;
	Map_string_Array_string folder_files;
};

struct v__ast__CTempVar {
	string name;
	v__ast__Expr orig;
	v__table__Type typ;
	bool is_ptr;
};

struct v__ast__IdentFn {
	v__table__Type typ;
};

struct v__ast__IdentVar {
	v__table__Type typ;
	bool is_mut;
	bool is_static;
	bool is_optional;
	v__table__ShareType share;
};

struct v__table__TypeSymbol {
	int parent_idx;
	v__table__TypeInfo info;
	v__table__Kind kind;
	string name;
	string cname;
	Array_v__table__Fn methods;
	string mod;
	bool is_public;
	v__table__Language language;
};

struct v__table__Field {
	string name;
	v__table__Type typ;
	v__table__FExpr default_expr;
	bool has_default_expr;
	v__table__Type default_expr_typ;
	string default_val;
	Array_v__table__Attr attrs;
	bool is_pub;
	bool is_mut;
	bool is_global;
};

struct v__table__Array {
	int nr_dims;
	v__table__Type elem_type;
};

struct v__table__Map {
	v__table__Type key_type;
	v__table__Type value_type;
};

struct v__table__Fn {
	Array_v__table__Param params;
	v__table__Type return_type;
	bool is_variadic;
	v__table__Language language;
	Array_string generic_names;
	bool is_pub;
	bool is_deprecated;
	bool is_unsafe;
	bool is_placeholder;
	bool no_body;
	string mod;
	string ctdefine;
	Array_v__table__Attr attrs;
	string name;
	voidptr source_fn;
	int usages;
};

struct v__table__Var {
	string name;
	bool is_mut;
	v__table__Type typ;
};

struct v__table__Aggregate {
	Array_v__table__Field fields;
	Array_v__table__Type types;
};

struct v__table__Interface {
	Array_v__table__Type types;
	Array_v__table__Field fields;
	Array_v__table__Fn methods;
};

struct v__table__Alias {
	v__table__Type parent_type;
	v__table__Language language;
	bool is_import;
};

struct v__table__Chan {
	v__table__Type elem_type;
	bool is_mut;
};

struct v__table__Thread {
	v__table__Type return_type;
};

struct v__table__ArrayFixed {
	int size;
	v__table__Type elem_type;
};

struct v__table__MultiReturn {
	Array_v__table__Type types;
};

struct v__table__SumType {
	Array_v__table__Type variants;
};

struct v__table__Enum {
	Array_string vals;
	bool is_flag;
	bool is_multi_allowed;
};

struct v__table__FnSignatureOpts {
	bool skip_receiver;
	bool type_only;
};

struct v__scanner__Scanner {
	string file_path;
	string file_base;
	string text;
	int pos;
	int line_nr;
	int last_nl_pos;
	bool is_inside_string;
	bool is_inter_start;
	bool is_inter_end;
	bool is_enclosed_inter;
	string line_comment;
	bool is_started;
	bool is_print_line_on_error;
	bool is_print_colored_error;
	bool is_print_rel_paths_on_error;
	byte quote;
	byte inter_quote;
	Array_int line_ends;
	int nr_lines;
	bool is_vh;
	bool is_fmt;
	v__scanner__CommentsMode comments_mode;
	bool is_inside_toplvl_statement;
	Array_v__token__Token all_tokens;
	int tidx;
	int eofs;
	v__pref__Preferences* pref;
	Array_v__errors__Error errors;
	Array_v__errors__Warning warnings;
	Array_v__vet__Error vet_errors;
};

struct v__parser__EatCommentsConfig {
	bool same_line;
	bool follow_up;
};

struct v__depgraph__DepGraphNode {
	string name;
	Array_string deps;
};

struct v__depgraph__OrderedDepMap {
	Array_string keys;
	Map_string_Array_string data;
};

struct v__markused__Walker {
	v__table__Table* table;
	Map_string_bool used_fns;
	Map_string_bool used_consts;
	int n_maps;
	int n_asserts;
	Array_v__ast__File files;
	Map_string_v__ast__FnDecl all_fns;
	Map_string_v__ast__ConstField all_consts;
};

struct v__gen__x64__Gen {
	string out_name;
	v__pref__Preferences* pref;
	v__table__Table* table;
	Array_byte buf;
	int sect_header_name_pos;
	i64 offset;
	Array_i64 str_pos;
	Array_string strings;
	i64 file_size_pos;
	i64 main_fn_addr;
	i64 code_start_pos;
	Map_string_i64 fn_addr;
	Map_string_int var_offset;
	int stack_var_pos;
	int debug_pos;
	Array_v__errors__Error errors;
	Array_v__errors__Warning warnings;
};

struct v__gen__x64__SectionConfig {
	string name;
	v__gen__x64__SectionType typ;
	i64 flags;
	voidptr data;
	bool is_saa;
	i64 datalen;
	int v_link;
	int info;
	i64 align;
	i64 entsize;
};

struct v__pkgconfig__MainOptions {
	bool modversion;
	bool description;
	bool help;
	bool debug;
	bool listall;
	bool exists;
	bool variables;
	bool requires;
	string atleast;
	string atleastpc;
	string exactversion;
	bool version;
	bool cflags;
	bool cflags_only_path;
	bool cflags_only_other;
	bool stat1c;
	bool libs;
	bool libs_only_link;
	bool libs_only_path;
	bool libs_only_other;
	Array_string args;
};

struct v__pkgconfig__Main {
	v__pkgconfig__MainOptions* opt;
	string res;
	bool has_actions;
};

struct v__pkgconfig__Options {
	string path;
	bool debug;
	bool norecurse;
	bool only_description;
	bool use_default_paths;
};

struct flag__FlagParser {
	Array_string args;
	int max_free_args;
	Array_flag__Flag flags;
	string application_name;
	string application_version;
	string application_description;
	int min_free_args;
	string args_description;
};

struct flag__Flag {
	string name;
	byte abbr;
	string usage;
	string val_desc;
};

struct semver__Version {
	int major;
	int minor;
	int patch;
	string prerelease;
	string metadata;
};

struct semver__RawVersion {
	string prerelease;
	string metadata;
	Array_string raw_ints;
};

struct semver__ComparatorSet {
	Array_semver__Comparator comparators;
};

struct semver__Range {
	Array_semver__ComparatorSet comparator_sets;
};

typedef byte Array_fixed_byte_26 [26];
typedef voidptr Array_fixed_voidptr_100 [100];
typedef byte Array_fixed_byte_1000 [1000];
typedef byte Array_fixed_byte_256 [256];
typedef byte Array_fixed_byte_1024 [1024];
typedef byte Array_fixed_byte_4096 [4096];
typedef int Array_fixed_int_6 [6];
typedef byte Array_fixed_byte_50 [50];
struct mapnode {
	voidptr* children;
	int len;
	Array_fixed_string_11 keys;
	Array_fixed_voidptr_11 values;
};

struct os__FileMode {
	os__FileType typ;
	os__FilePermission owner;
	os__FilePermission group;
	os__FilePermission others;
};

struct os__Process {
	string filename;
	int pid;
	int code;
	os__ProcessState status;
	string err;
	Array_string args;
	bool env_is_custom;
	Array_string env;
	bool use_stdio_ctl;
	Array_fixed_int_3 stdio_fd;
};

struct v__pref__Preferences {
	v__pref__OS os;
	v__pref__Backend backend;
	v__pref__BuildMode build_mode;
	v__pref__OutputMode output_mode;
	bool is_verbose;
	bool is_test;
	bool is_script;
	bool is_vsh;
	bool is_livemain;
	bool is_liveshared;
	bool is_shared;
	bool is_prof;
	string profile_file;
	bool profile_no_inline;
	bool translated;
	bool is_prod;
	bool obfuscate;
	bool is_repl;
	bool is_run;
	bool sanitize;
	bool is_debug;
	bool is_vlines;
	bool show_cc;
	bool show_c_output;
	string dump_c_flags;
	bool use_cache;
	bool retry_compilation;
	bool is_stats;
	string cflags;
	bool m64;
	string ccompiler;
	v__pref__CompilerType ccompiler_type;
	string third_party_option;
	bool building_v;
	bool autofree;
	bool compress;
	bool enable_globals;
	bool is_fmt;
	bool is_vet;
	bool is_bare;
	bool no_preludes;
	string custom_prelude;
	Array_string lookup_path;
	bool output_cross_c;
	bool prealloc;
	string vroot;
	string out_name_c;
	string out_name;
	string display_name;
	string bundle_id;
	string path;
	Array_string compile_defines;
	Array_string compile_defines_all;
	Array_string run_args;
	Array_string printfn_list;
	bool print_v_files;
	bool skip_running;
	bool skip_warnings;
	bool warn_impure_v;
	bool warns_are_errors;
	bool fatal_errors;
	bool reuse_tmpc;
	v__pref__ColorOutput use_color;
	bool is_parallel;
	int error_limit;
	bool is_vweb;
	bool only_check_syntax;
	bool experimental;
	bool skip_unused;
	bool show_timings;
	bool is_ios_simulator;
	bool is_apk;
	Array_string cleanup_files;
	Array_string build_options;
	v__vcache__CacheManager cache_manager;
	bool is_help;
};

struct v__vmod__Parser {
	string file_path;
	v__vmod__Scanner scanner;
};

struct v__ast__AliasTypeDecl {
	string name;
	bool is_pub;
	v__table__Type parent_type;
	v__token__Position pos;
	Array_v__ast__Comment comments;
};

struct v__ast__FnTypeDecl {
	string name;
	bool is_pub;
	v__table__Type typ;
	v__token__Position pos;
	Array_v__ast__Comment comments;
};

struct v__ast__SumTypeDecl {
	string name;
	bool is_pub;
	v__token__Position pos;
	Array_v__ast__Comment comments;
	Array_v__ast__SumTypeVariant variants;
};

struct v__ast__ArrayDecompose {
	v__ast__Expr expr;
	v__token__Position pos;
	v__table__Type expr_type;
	v__table__Type arg_type;
};

struct v__ast__ArrayInit {
	v__token__Position pos;
	v__token__Position elem_type_pos;
	Array_v__ast__Expr exprs;
	Array_Array_v__ast__Comment ecmnts;
	Array_v__ast__Comment pre_cmnts;
	bool is_fixed;
	bool has_val;
	string mod;
	v__ast__Expr len_expr;
	v__ast__Expr cap_expr;
	v__ast__Expr default_expr;
	bool has_len;
	bool has_cap;
	bool has_default;
	Array_v__table__Type expr_types;
	v__table__Type elem_type;
	v__table__Type typ;
};

struct v__ast__AsCast {
	v__ast__Expr expr;
	v__table__Type typ;
	v__token__Position pos;
	v__table__Type expr_type;
};

struct v__ast__Assoc {
	string var_name;
	Array_string fields;
	Array_v__ast__Expr exprs;
	v__token__Position pos;
	v__table__Type typ;
	v__ast__Scope* scope;
};

struct v__ast__AtExpr {
	string name;
	v__token__Position pos;
	v__token__AtKind kind;
	string val;
};

struct v__ast__BoolLiteral {
	bool val;
	v__token__Position pos;
};

struct v__ast__CastExpr {
	v__ast__Expr expr;
	v__ast__Expr arg;
	v__table__Type typ;
	v__token__Position pos;
	string typname;
	v__table__Type expr_type;
	bool has_arg;
	bool in_prexpr;
};

struct v__ast__ChanInit {
	v__token__Position pos;
	v__ast__Expr cap_expr;
	bool has_cap;
	v__table__Type typ;
	v__table__Type elem_type;
};

struct v__ast__CharLiteral {
	string val;
	v__token__Position pos;
};

struct v__ast__Comment {
	string text;
	bool is_multi;
	int line_nr;
	v__token__Position pos;
};

struct v__ast__ComptimeSelector {
	bool has_parens;
	v__ast__Expr left;
	v__ast__Expr field_expr;
	v__token__Position pos;
	v__table__Type left_type;
	v__table__Type typ;
};

struct v__ast__ConcatExpr {
	Array_v__ast__Expr vals;
	v__token__Position pos;
	v__table__Type return_type;
};

struct v__ast__EnumVal {
	string enum_name;
	string val;
	string mod;
	v__token__Position pos;
	v__table__Type typ;
};

struct v__ast__FloatLiteral {
	string val;
	v__token__Position pos;
};

struct v__ast__Ident {
	v__table__Language language;
	v__token__Kind tok_kind;
	v__token__Position pos;
	v__token__Position mut_pos;
	v__ast__Scope* scope;
	v__ast__ScopeObject obj;
	string mod;
	string name;
	v__ast__IdentKind kind;
	v__ast__IdentInfo info;
	bool is_mut;
};

struct v__ast__IfExpr {
	bool is_comptime;
	v__token__Kind tok_kind;
	v__ast__Expr left;
	v__token__Position pos;
	Array_v__ast__Comment post_comments;
	Array_v__ast__IfBranch branches;
	bool is_expr;
	v__table__Type typ;
	bool has_else;
};

struct v__ast__IfGuardExpr {
	string var_name;
	v__token__Position pos;
	v__ast__Expr expr;
	v__table__Type expr_type;
};

struct v__ast__IntegerLiteral {
	string val;
	v__token__Position pos;
};

struct v__ast__Likely {
	v__ast__Expr expr;
	v__token__Position pos;
	bool is_likely;
};

struct v__ast__LockExpr {
	Array_v__ast__Stmt stmts;
	Array_bool is_rlock;
	v__token__Position pos;
	Array_v__ast__Ident lockeds;
	bool is_expr;
	v__table__Type typ;
};

struct v__ast__MapInit {
	v__token__Position pos;
	Array_v__ast__Expr keys;
	Array_v__ast__Expr vals;
	Array_Array_v__ast__Comment comments;
	Array_v__ast__Comment pre_cmnts;
	v__table__Type typ;
	v__table__Type key_type;
	v__table__Type value_type;
};

struct v__ast__MatchExpr {
	v__token__Kind tok_kind;
	v__ast__Expr cond;
	Array_v__ast__MatchBranch branches;
	v__token__Position pos;
	Array_v__ast__Comment comments;
	bool is_expr;
	v__table__Type return_type;
	v__table__Type cond_type;
	v__table__Type expected_type;
	bool is_sum_type;
};

struct v__ast__None {
	v__token__Position pos;
	int foo;
};

struct v__ast__OffsetOf {
	v__table__Type struct_type;
	string field;
	v__token__Position pos;
};

struct v__ast__OrExpr {
	Array_v__ast__Stmt stmts;
	v__ast__OrKind kind;
	v__token__Position pos;
};

struct v__ast__ParExpr {
	v__ast__Expr expr;
	v__token__Position pos;
};

struct v__ast__PostfixExpr {
	v__token__Kind op;
	v__ast__Expr expr;
	v__token__Position pos;
	string auto_locked;
};

struct v__ast__RangeExpr {
	v__ast__Expr low;
	v__ast__Expr high;
	bool has_high;
	bool has_low;
	v__token__Position pos;
};

struct v__ast__SelectExpr {
	Array_v__ast__SelectBranch branches;
	v__token__Position pos;
	bool has_exception;
	bool is_expr;
	v__table__Type expected_type;
};

struct v__ast__SelectorExpr {
	v__token__Position pos;
	string field_name;
	bool is_mut;
	v__token__Position mut_pos;
	v__token__Kind next_token;
	v__ast__Expr expr;
	v__table__Type expr_type;
	v__table__Type typ;
	v__table__Type name_type;
	v__ast__Scope* scope;
	v__table__Type from_embed_type;
};

struct v__ast__SizeOf {
	bool is_type;
	v__ast__Expr expr;
	v__token__Position pos;
	v__table__Type typ;
};

struct v__ast__StringInterLiteral {
	Array_string vals;
	Array_v__ast__Expr exprs;
	Array_int fwidths;
	Array_int precisions;
	Array_bool pluss;
	Array_bool fills;
	Array_v__token__Position fmt_poss;
	v__token__Position pos;
	Array_v__table__Type expr_types;
	Array_byte fmts;
	Array_bool need_fmts;
};

struct v__ast__StringLiteral {
	string val;
	bool is_raw;
	v__table__Language language;
	v__token__Position pos;
};

struct v__ast__StructInit {
	v__token__Position pos;
	bool is_short;
	bool unresolved;
	Array_v__ast__Comment pre_comments;
	v__table__Type typ;
	v__ast__Expr update_expr;
	v__table__Type update_expr_type;
	Array_v__ast__Comment update_expr_comments;
	bool has_update_expr;
	Array_v__ast__StructInitField fields;
	Array_v__ast__StructInitEmbed embeds;
};

struct v__ast__Type {
	v__table__Type typ;
	v__token__Position pos;
};

struct v__ast__TypeOf {
	v__ast__Expr expr;
	v__token__Position pos;
	v__table__Type expr_type;
};

struct v__ast__UnsafeExpr {
	v__ast__Expr expr;
	v__token__Position pos;
};

struct v__ast__AssertStmt {
	v__token__Position pos;
	v__ast__Expr expr;
};

struct v__ast__AssignStmt {
	v__token__Kind op;
	v__token__Position pos;
	Array_v__ast__Comment comments;
	Array_v__ast__Comment end_comments;
	Array_v__ast__Expr right;
	Array_v__ast__Expr left;
	Array_v__table__Type left_types;
	Array_v__table__Type right_types;
	bool is_static;
	bool is_simple;
	bool has_cross_var;
};

struct v__ast__Block {
	Array_v__ast__Stmt stmts;
	bool is_unsafe;
	v__token__Position pos;
};

struct v__ast__BranchStmt {
	v__token__Kind kind;
	string label;
	v__token__Position pos;
};

struct v__ast__CompFor {
	string val_var;
	Array_v__ast__Stmt stmts;
	v__ast__CompForKind kind;
	v__token__Position pos;
	v__token__Position typ_pos;
	v__table__Type typ;
};

struct v__ast__ConstDecl {
	bool is_pub;
	v__token__Position pos;
	Array_v__ast__ConstField fields;
	Array_v__ast__Comment end_comments;
	bool is_block;
};

struct v__ast__DeferStmt {
	Array_v__ast__Stmt stmts;
	v__token__Position pos;
	string ifdef;
	int idx_in_fn;
};

struct v__ast__EnumDecl {
	string name;
	bool is_pub;
	bool is_flag;
	bool is_multi_allowed;
	Array_v__ast__Comment comments;
	Array_v__ast__EnumField fields;
	Array_v__table__Attr attrs;
	v__token__Position pos;
};

struct v__ast__ExprStmt {
	v__ast__Expr expr;
	v__token__Position pos;
	Array_v__ast__Comment comments;
	bool is_expr;
	v__table__Type typ;
};

struct v__ast__ForCStmt {
	v__ast__Stmt init;
	bool has_init;
	v__ast__Expr cond;
	bool has_cond;
	v__ast__Stmt inc;
	bool has_inc;
	bool is_multi;
	Array_v__ast__Stmt stmts;
	v__token__Position pos;
	string label;
	v__ast__Scope* scope;
};

struct v__ast__ForInStmt {
	string key_var;
	string val_var;
	v__ast__Expr cond;
	bool is_range;
	v__ast__Expr high;
	Array_v__ast__Stmt stmts;
	v__token__Position pos;
	bool val_is_mut;
	v__table__Type key_type;
	v__table__Type val_type;
	v__table__Type cond_type;
	v__table__Kind kind;
	string label;
	v__ast__Scope* scope;
};

struct v__ast__ForStmt {
	v__ast__Expr cond;
	Array_v__ast__Stmt stmts;
	bool is_inf;
	v__token__Position pos;
	string label;
	v__ast__Scope* scope;
};

struct v__ast__GlobalDecl {
	v__token__Position pos;
	Array_v__ast__GlobalField fields;
	Array_v__ast__Comment end_comments;
};

struct v__ast__GotoLabel {
	string name;
	v__token__Position pos;
};

struct v__ast__GotoStmt {
	string name;
	v__token__Position pos;
};

struct v__ast__HashStmt {
	string mod;
	v__token__Position pos;
	string source_file;
	string val;
	string kind;
	string main;
	string msg;
};

struct v__ast__Import {
	string mod;
	string alias;
	v__token__Position pos;
	v__token__Position mod_pos;
	v__token__Position alias_pos;
	Array_v__ast__ImportSymbol syms;
	Array_v__ast__Comment comments;
	Array_v__ast__Comment next_comments;
};


#endif // HAZEL_LANG_COMPILER 