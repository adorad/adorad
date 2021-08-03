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

#ifndef CSTL_OS_H
#define CSTL_OS_H

#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <unistd.h>
#include <adorad/core/buffer.h>
#include <adorad/core/debug.h>

#if defined(_WIN32) || defined(_WIN64)
    #ifndef CSTL_OS_WINDOWS
        #define CSTL_OS_WINDOWS 1
    #endif
#elif defined(__APPLE__) && defined(__MACH__)
    #ifndef CSTL_OS_OSX
        #define CSTL_OS_OSX 1
    #endif
#elif defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
    #ifndef CSTL_OS_UNIX
        #define CSTL_OS_UNIX 1
    #endif

    #if defined(__linux__)
        #ifndef CSTL_OS_LINUX
            #define CSTL_OS_LINUX 1
        #endif
    #elif defined(__FreeBSD__) && defined(__FreeBSD_kernel__)
        #ifndef CSTL_OS_FREEBSD
            #define CSTL_OS_FREEBSD 1
        #endif
    #else 
        #error This Unix Operating System is not supported by Adorad.
    #endif 
#elif defined(_gnu_linux_) || defined(__linux__) && !defined(CSTL_OS_LINUX)
    #define CSTL_OS_LINUX 1
#elif defined(__NetBSD__)
    #define CSTL_OS_NETBSD
#elif defined(__DragonFly__)
    #define CSTL_OS_DRAGONFLY
#elif defined(__OpenBSD__)
    #define CSTL_OS_OPENBSD
#elif defined(__HAIKU__)
    #define CSTL_OS_HAIKU
#else
    #error This Operating System is not supported by Adorad
#endif

// Architetures
#if defined(__x86_64__)
    #define CSTL_ARCH_X86_64    1
#elif defined(__aarch64__)
    #define CSTL_ARCH_ARM64     1
#elif defined(__ATM_EABI__)
    #define CSTL_ARCH_ARM       1
#else
    #define CSTL_ARCH_UNKNOWN
    // #error This Architecture is not supported by Adorad
#endif // __x86_64

// Generic 32 vs 64 bit
#if defined(_WIN64) || defined(__x86_64__) || defined(_M_X64) || defined(__64BIT__) || defined(__powerpc64__) || defined(__ppc64__)
    #define CSTL_ARCH_64BIT     1
#else 
    #define CSTL_ARCH_32BIT     1
#endif // _WIN64

#ifdef CSTL_OS_WINDOWS
    #define CSTL_OS_SEP         "\\"
    #define CSTL_OS_SEP_CHAR    '\\'
#else
    #define CSTL_OS_SEP         "/"
    #define CSTL_OS_SEP_CHAR    '/'
#endif // CSTL_OS_WINDOWS

static Buff* os_get_cwd();
static Buff* os_path_dirname(Buff* path);
static Buff* os_path_extname(Buff* path);
static Buff* os_path_join(Buff* path);
static bool os_is_sep(char ch);

static Buff* os_get_cwd() {
    long n;
    char *buf;

    n = pathconf(".", _PC_PATH_MAX);
    CSTL_CHECK_NE(n, -1);
    buf = (char*)calloc(n, sizeof(*buf));
    CSTL_CHECK_NOT_NULL(buf, "calloc failed. Out of memory");
    getcwd(buf, n);
    
    Buff* buff = buff_new(buf);
    return buff;
}

static Buff* __os_dirname_basename(Buff* path, bool is_basename) {
    Buff* result = buff_new(null);
    UInt64 length = path->length;
    if(!length)
        return null;

    char* end = buff_end(path);
    if(!is_basename) {
        // If the last character is a `sep`, `path` is the dirname
        if(os_is_sep(*end))
            return path;

        // If there is no `sep` in `path`, the dirname is empty
        if(!(strstr(path->data, "/") || strstr(path->data, "\\")))
            return result;
        
        Buff* rev = buff_rev(path);
        char* rev_dir = strchr(rev->data, *(CSTL_OS_SEP));
        buff_set(result, rev_dir);
        result = buff_rev(result);
        buff_free(rev);
    } else {
        // If the last character is a `sep`, `basename` is empty
        if(os_is_sep(*end))
            return buff_new(null);
        
        // If there is no `sep` in `path`, `path` is the basename
        if(!(strstr(path->data, "/") || strstr(path->data, "\\")))
            return path;
        
        Buff* rev = buff_rev(path);
        for(UInt64 i = 0; i<length; i++) {
            if(os_is_sep(*(rev->data + i))) {
                *(rev->data + i) = nullchar;
                break;
            }
        }
        buff_set(result, rev->data);
        result = buff_rev(result);
        free(rev);
    }
    
    return result;
}

static Buff* os_get_dirname(Buff* path) {
    return __os_dirname_basename(path, false);
}

static Buff* os_get_basename(Buff* path) {
    return __os_dirname_basename(path, true);
}

static Buff* os_path_extname(Buff* path) {

}

static Buff* os_path_join(Buff* path) {

}

static bool os_is_sep(char ch) {
#ifdef CSTL_OS_WINDOWS
    return ch == '\\' || ch == '/';
#else
    return ch == '/';
#endif // CSTL_OS_WINDOWS
}


#endif // CSTL_OS_H