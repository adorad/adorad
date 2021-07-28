/*
          _____         _____           _____  
    /\   |  __ \ / __ \|  __ \    /\   |  __ \
   /  \  | |  | | |  | | |__)|   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /  / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \ / ____ \| |__| | https://github.com/adorad/adorad/
/_/_ __\_\_____/_\____/|_|__\_/_/_ __\_\_____/

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <@jasmcaus>
*/

#ifndef CSTL_HEADERS_H
#define CSTL_HEADERS_H

#include <adorad/core/os.h>


// Headers ==========================================
#if defined(_WIN32) && !defined(__MINGW32__)
    #ifndef _CRT_SECURE_NO_WARNINGS
        #define _CRT_SECURE_NO_WARNINGS
    #endif
#endif

#if defined(CSTL_OS_UNIX)
    #define _GNU_SOURCE
    #define _LARGEFILE64_SOURCE
#endif


// TODO(jasmcaus): How many of these headers do I really need?
// #include <stdarg.h>
#if !defined(CSTL_OS_WINDOWS)
    #include <stddef.h>
    #include <stdarg.h>
#endif

#if defined(CSTL_OS_WINDOWS)
    #if !defined(CSTL_NO_WINDOWS_H)
        #define NOMINMAX 1
        #if !defined(CSTL_WINDOWS_H_INCLUDED)
            #define WIN32_LEAN_AND_MEAN 1
            #define WIN32_MEAN_AND_LEAN 1
            #define VC_EXTRALEAN        1
        #endif
        
        #pragma warning(push, 0)
            #include <Windows.h>
            // This conflicts with certain functionalities, especially in the Adorad Language
            #ifdef CONST
                #undef CONST
            #endif // CONST
            #ifdef IN
                #undef IN
            #endif // IN
            // #include <io.h>
        #pragma warning(pop)
        #undef NOMINMAX

        #if !defined(CSTL_WINDOWS_H_INCLUDED)
            #undef WIN32_LEAN_AND_MEAN
            #undef WIN32_MEAN_AND_LEAN
            #undef VC_EXTRALEAN
        #endif
    #endif

    // #include <malloc.h> // NOTE(jasmcaus): _aligned_*()
    // #include <intrin.h>

#else
    // #include <dlfcn.h>
    // #include <errno.h>
    // #include <fcntl.h>
    // #include <pthread.h>

    #ifndef _IOSC11_SOURCE
        #define _IOSC11_SOURCE
    #endif

    // #include <stdlib.h> // NOTE(jasmcaus): malloc on linux
    // #include <sys/mman.h>

    #if !defined(CSTL_OS_OSX) && !defined(__FreeBSD__)
        // #include <sys/sendfile.h>
    #endif

    // #include <sys/stat.h>
    // #include <sys/time.h>
    // #include <sys/types.h>
    // #include <time.h>
    // #include <unistd.h>

    #if defined(CSTL_CPU_X86)
        // #include <xmmintrin.h>
    #endif
#endif

#if defined(CSTL_OS_OSX)
    // #include <mach/mach.h>
    // #include <mach/mach_init.h>
    // #include <mach/mach_time.h>
    // #include <mach/thread_act.h>
    // #include <mach/thread_policy.h>
    // #include <sys/sysctl.h>
    // #include <copyfile.h>
    // #include <mach/clock.h>
#endif

#if defined(CSTL_OS_FREEBSD)
    // #include <sys/sysctl.h>
    // #include <pthread_np.h>
    // #include <sys/cpuset.h>
    // #include <sys/types.h>
    // #include <sys/socket.h>
    // #include <sys/uio.h>

    #define lseek64 lseek
    #define sendfile(out, in, offset, count) sendfile(out, in, offset, count, NULL, NULL, 0)
#endif

#if defined(CSTL_OS_UNIX)
    // #include <semaphore.h>
#endif


#endif // CSTL_HEADERS_H