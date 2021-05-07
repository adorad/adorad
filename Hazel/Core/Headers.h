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

#ifndef CSTL_HEADERS_H
#define CSTL_HEADERS_H

#include <Hazel/Core/OS.h>

// #ifdef __cplusplus
// namespace Hazel {
// #endif


// Headers ==========================================
#if defined(_WIN32) && !defined(__MINGW32__)
    #ifndef _CRT_SECURE_NO_WARNINGS
        #define _CRT_SECURE_NO_WARNINGS
    #endif
#endif

#if defined(CSTL_SYSTEM_UNIX)
    #define _GNU_SOURCE
    #define _LARGEFILE64_SOURCE
#endif


// TODO(jasmcaus): How many of these headers do I really need?
// #include <stdarg.h>
#if !defined(CSTL_SYSTEM_WINDOWS)
    #include <stddef.h>
    #include <stdarg.h>
#endif

#if defined(CSTL_SYSTEM_WINDOWS)
    #if !defined(CSTL_NO_WINDOWS_H)
        #define NOMINMAX 1
        #if !defined(CSTL_WINDOWS_H_INCLUDED)
            #define WIN32_LEAN_AND_MEAN 1
            #define WIN32_MEAN_AND_LEAN 1
            #define VC_EXTRALEAN        1
        #endif
        // #include <windows.h>
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

    #if !defined(CSTL_SYSTEM_OSX) && !defined(__FreeBSD__)
        // #include <sys/sendfile.h>
    #endif

    // #include <sys/stat.h>
    // #include <sys/time.h>
    // #include <sys/Types.h>
    // #include <time.h>
    // #include <unistd.h>

    #if defined(CSTL_CPU_X86)
        // #include <xmmintrin.h>
    #endif
#endif

#if defined(CSTL_SYSTEM_OSX)
    // #include <mach/mach.h>
    // #include <mach/mach_init.h>
    // #include <mach/mach_time.h>
    // #include <mach/thread_act.h>
    // #include <mach/thread_policy.h>
    // #include <sys/sysctl.h>
    // #include <copyfile.h>
    // #include <mach/clock.h>
#endif

#if defined(CSTL_SYSTEM_FREEBSD)
    // #include <sys/sysctl.h>
    // #include <pthread_np.h>
    // #include <sys/cpuset.h>
    // #include <sys/Types.h>
    // #include <sys/socket.h>
    // #include <sys/uio.h>

    #define lseek64 lseek
    #define sendfile(out, in, offset, count) sendfile(out, in, offset, count, NULL, NULL, 0)
#endif

#if defined(CSTL_SYSTEM_UNIX)
    // #include <semaphore.h>
#endif


// #ifdef __cplusplus
// } // namespace Hazel
// #endif

#endif // CSTL_HEADERS_H