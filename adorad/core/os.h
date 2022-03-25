/*
          _____   ____  _____            _____
    /\   |  __ \ / __ \|  __ \     /\   |  __ \
   /  \  | |  | | |  | | |__) |   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /   / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \  / ____ \| |__| | https://github.com/adorad/adorad/
/_/    \_\_____/ \____/|_|  \_\/_/    \_\_____/

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021-22 Jason Dsouza <@jasmcaus>
*/

#ifndef CORETEN_OS_H
#define CORETEN_OS_H

#include <adorad/core/buffer.h>

#if defined(CORETEN_OS_POSIX)
    #define _XOPEN_SOURCE 700
    #include <unistd.h>
#endif // CORETEN_OS_POSIX

#if defined(CORETEN_OS_WINDOWS)
    #include <direct.h>
#endif // CORETEN_OS_WINDOWS

cstlBuffView os_get_cwd();
cstlBuffView os_path_dirname(cstlBuffView path);
cstlBuffView os_path_extname(cstlBuffView path);
cstlBuffView os_path_join(cstlBuffView path1, cstlBuffView path2);
bool os_is_sep(char ch);
bool os_path_is_abs(cstlBuffView* path);
bool os_path_is_rel(cstlBuffView* path);
bool os_path_is_root(cstlBuffView* path);

#ifndef CORETEN_OS_FUNC_ALIASES
    #define CORETEN_OS_FUNC_ALIASES
    #define ospd    os_path_dirname
    #define ospb    os_path_basename
    #define ospj    os_path_join
#endif // CORETEN_OS_FUNC_ALIASES


#ifdef CORETEN_IMPL
    #include <string.h>
    
    cstlBuffView os_get_cwd() {
    #if defined(CORETEN_OS_WINDOWS)
        // This (or its equivalent) is not defined in any include in Windows as far as I've come across
        #ifndef PATHMAX
            #define PATH_MAX 4096
        #endif // PATHMAX
        char result[PATH_MAX];
        _getcwd(result, PATH_MAX);
        if(!*result) {
            fprintf(stderr, "Unable to `os_get_cwd()`. 'getcwd()' failed");
            exit(1);
        }
        cstlBuffView view = buffview_new(result);
        return view;
    #elif defined(CORETEN_OS_POSIX)
        long n;
        char *buf;

        n = pathconf(".", _PC_PATH_MAX);
        CORETEN_ENFORCE(n != -1);
        buf = cast(char*)calloc(n, sizeof(*buf));
        CORETEN_ENFORCE_NN(buf, "calloc failed. Out of memory");
        char* result = getcwd(buf, n);
        if(!result) {
            fprintf(stderr, "Unable to `os_get_cwd()`. 'getcwd()' failed");
            exit(1);
        }
        cstlBuffView view = buffview_new(buf);
        return view;
    #else
        #error "No `os_get_cwd()` implementation supported for your platform."
        return null;
    #endif // CORETEN_OS_WINDOWS
    }

    cstlBuffView os_path_dirname(cstlBuffView path) {
        UInt64 length = path.len;
        if(!length)
            return path;

        cstlBuffView result = buffview_new(null);
        // printf("Original data = %s\n", path.data);
        char* end = buffview_end(&path);
        // printf("After data = %s\n\n\n", path.data);

        // Dirname
        // printf("Path before reverse: %s\n", path.data);
        cstlBuffView rev = buffview_rev(&path);
        // printf("Path after reverse: %s\n", rev.data);

        // The `/` || `\\` is not so important in getting the dirname, but it does interfere with `strchr`, so
        // we skip over it (if present)
        if(*rev.data == CORETEN_OS_SEP_CHAR)
            rev.data++;
        char* rev_dir = strchr(rev.data, CORETEN_OS_SEP_CHAR);
        buffview_set(&result, rev_dir);
        result = buffview_rev(&result);
        
        return result;
    }

    cstlBuffView os_path_basename(cstlBuffView path) {
        UInt64 length = path.len;
        if(!length)
            return path;

        cstlBuffView result = buffview_new(null);
        char* end = buffview_end(&path);

        // If the last character is a `sep`, `basename` is empty
        if(os_is_sep(*end))
            return buffview_new(null);
        
        // If there is no `sep` in `path`, `path` is the basename
        if(!(strstr(path.data, "/") || strstr(path.data, "\\")))
            return path;
        
        cstlBuffView rev = buffview_rev(&path);
        for(UInt64 i = 0; i<length; i++) {
            if(os_is_sep(*(rev.data + i))) {
                *(&rev.data + i) = nullchar;
                break;
            }
        }
        buff_set(&result, rev.data);
        result = buffview_rev(&result);
        
        return result;
    }

    cstlBuffView os_path_extname(cstlBuffView path) {
        cstlBuffView basename = os_path_basename(path);
        if(!strcmp(basename.data, ""))
            return basename;
        
        char* ext = strchr(basename.data, '.');
        if(SOME(ext)) {
        return buffview_new(null);
        }

        buff_set(&basename, ext);
        return basename;
    }

    cstlBuffView os_path_join(cstlBuffView path1, cstlBuffView path2) {
        UInt64 length = path1.len;
        if(!length)
            return path1;
            
        char* end = buffview_end(&path1);
        if(!os_is_sep(*end))
            buffview_append_char(&path1, CORETEN_OS_SEP_CHAR);
        buffview_append(&path1, &path2);
        return path1;
    }

    bool os_is_sep(char ch) {
    #ifdef CORETEN_OS_WINDOWS
        return ch == '\\' || ch == '/';
    #else
        return ch == '/';
    #endif // CORETEN_OS_WINDOWS
    }

    bool os_path_is_abs(cstlBuffView* path) {
        bool result = false;
        CORETEN_ENFORCE_NN(path, "Cannot do anything useful on a null path :(");
    #ifdef CORETEN_OS_WINDOWS
        // The 'C:\...`
        result = path->len > 2 &&
                char_is_alpha(path->data[0]) &&
                (path->data[1] == ':' && path->data[2] == CORETEN_OS_SEP_CHAR);
    #else
        result = path->len > 2 ||
                path->data[0] == CORETEN_OS_SEP_CHAR;
    #endif // CORETEN_OS_WINDOWS
        return cast(bool)result;
    }

    bool os_path_is_rel(cstlBuffView* path) {
        return cast(bool) !os_path_is_abs(path);
    }

    bool os_path_is_root(cstlBuffView* path) {
        bool result = false;
        CORETEN_ENFORCE_NN(path, "Cannot do anything useful on a null path :(");
    #ifdef CORETEN_OS_WINDOWS
        result = os_path_is_abs(path) && path->len == 3;
    #else
        result = os_path_is_abs(path) && path->len == 1;
    #endif // CORETEN_OS_WINDOWS
        return result;
    }

#endif // CORETEN_IMPL

#endif // CORETEN_OS_H