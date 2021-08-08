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
#endif // CSTL_OS_POSIX

#if defined(CSTL_OS_WINDOWS)
    #include <direct.h>
#endif // CSTL_OS_WINDOWS

static cstlBuffer* os_get_cwd();
static cstlBuffer* os_path_dirname(cstlBuffer* path);
static cstlBuffer* os_path_extname(cstlBuffer* path);
static cstlBuffer* os_path_join(cstlBuffer* path1, cstlBuffer* path2);
static bool os_is_sep(char ch);
static bool os_path_is_abs(cstlBuffer* path);
static bool os_path_is_rel(cstlBuffer* path);
static bool os_path_is_root(cstlBuffer* path);

#ifndef CSTL_OS_FUNC_ALIASES
    #define CSTL_OS_FUNC_ALIASES
    #define ospd    os_path_dirname
    #define ospb    os_path_basename
    #define ospj    os_path_join
#endif // CSTL_OS_FUNC_ALIASES

#endif // CSTL_OS_H