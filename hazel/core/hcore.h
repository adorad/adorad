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

#if defined(__cplusplus)
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
	    #define NOMINMAX            1
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


#if defined(__cplusplus)
}
#endif

#endif // _HAZEL_CORE_CSTLIB_H