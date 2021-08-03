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

#include <stdlib.h>
#include <adorad/core/buffer.h>
#include <adorad/core/os_defs.h>
#include <adorad/core/types.h>
#include <adorad/core/debug.h>

#if defined(CSTL_OS_POSIX)
    #define _XOPEN_SOURCE 700
    #include <unistd.h>
#else
    #include <adorad/core/headers.h>
#endif // CSTL_OS_POSIX

static Buff* os_get_cwd();
static Buff* os_path_dirname(Buff* path);
static Buff* os_path_extname(Buff* path);
static Buff* os_path_join(Buff* path1, Buff* path2);
static bool os_is_sep(char ch);

static Buff* os_get_cwd() {
#if defined(CSTL_OS_WINDOWS)
    Pathspace path_space;
    if (GetCurrentDirectoryW(PATH_MAX_WIDE, &path_space.data.items[0]) == 0) {
        fprintf(stderr, "Unable to `os_get_cwd()`. 'GetCurrentDirectoryW()' failed");
        exit(1);
    }
    Buff* buff = buff_new(path_space.data.items[0]);
    return buff;
#elif defined(CSTL_OS_POSIX)
    long n;
    char *buf;

    n = pathconf(".", _PC_PATH_MAX);
    CSTL_CHECK_NE(n, -1);
    buf = (char*)calloc(n, sizeof(*buf));
    CSTL_CHECK_NOT_NULL(buf, "calloc failed. Out of memory");
    char* result = getcwd(buf, n);
    if(!result) {
        fprintf(stderr, "Unable to `os_get_cwd()`. 'getcwd()' failed");
        exit(1);
    }
    Buff* buff = buff_new(buf);
    return buff;
#else
    #error "No `os_get_cwd()` implementation supported for your platform."
    return null;
#endif // CSTL_OS_WINDOWS
}

static Buff* __os_dirname_basename(Buff* path, bool is_basename) {
    UInt64 length = path->length;
    if(!length)
        return path;

    Buff* result = buff_new(null);
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
    Buff* basename = os_get_basename(path);
    if(!strcmp(basename->data, ""))
        return basename;
    
    char* ext = strchr(basename->data, '.');
    if(!ext) {
       free(basename);
       return buff_new(null);
    }

    buff_set(basename, ext);
    return basename;
}


static Buff* os_path_join(Buff* path1, Buff* path2) {
    UInt64 length = path1->length;
    if(!length)
        return path1;
        
    char* end = buff_end(path1);
    if(!os_is_sep(*end))
        buff_append_char(path1, CSTL_OS_SEP_CHAR);
    buff_append(path1, path2);
    return path1;
}

static bool os_is_sep(char ch) {
#ifdef CSTL_OS_WINDOWS
    return ch == '\\' || ch == '/';
#else
    return ch == '/';
#endif // CSTL_OS_WINDOWS
}


#endif // CSTL_OS_H