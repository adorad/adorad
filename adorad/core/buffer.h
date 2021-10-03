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

#ifndef CORETEN_BUFFER_H
#define CORETEN_BUFFER_H

#include <adorad/core/debug.h>
#include <adorad/core/types.h>
#include <adorad/core/char.h>
#include <adorad/core/misc.h>

/*
    A `cstlBuffer` is a Fixed-Size Buffer.
    It works like a string, except that the actual type is just a pointer to the first `char` element
*/

typedef struct cstlBuffer cstlBuffer;
typedef cstlBuffer Buff;

struct cstlBuffer {
    char* data;    // buffer data
    UInt64 len;    // buffer size
    // bool is_utf8;  // UTF-8 Strings
};

cstlBuffer* buff_new(char* buff_data);
char buff_at(cstlBuffer* buffer, UInt64 n);
char* buff_begin(cstlBuffer* buffer);
char* buff_end(cstlBuffer* buffer);
bool buff_is_empty(cstlBuffer* buffer);
void buff_append(cstlBuffer* buffer, cstlBuffer* buff2);
void buff_append_char(cstlBuffer* buffer, char ch);
void buff_set(cstlBuffer* buffer, char* new_buff);
cstlBuffer* buff_clone(cstlBuffer* buffer);
UInt64 buff_len(cstlBuffer* buffer);
void buff_reset(cstlBuffer* buffer);
cstlBuffer* buff_rev(cstlBuffer* buffer);
bool buff_cmp(cstlBuffer* buff1, cstlBuffer* buff2);
bool buff_cmp_nocase(cstlBuffer* buff1, cstlBuffer* buff2);
cstlBuffer* buff_slice(cstlBuffer* buffer, int begin, int bytes);
void buff_free(cstlBuffer* buffer);
cstlBuffer* buff_toupper(cstlBuffer* buffer);
cstlBuffer* buff_tolower(cstlBuffer* buffer);

#endif // CORETEN_BUFFER_H