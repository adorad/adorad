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

#ifndef CSTL_BUFFER_H
#define CSTL_BUFFER_H

#include <string.h>
#include <adorad/core/types.h>
#include <adorad/core/debug.h>

typedef struct cstlBuffer cstlBuffer;
typedef cstlBuffer Buff;

struct cstlBuffer {
    char* data;    // buffer data
    UInt64 length; // buffer size
};

// Create a new `cstlBuffer`
static cstlBuffer* buff_new(char* buff_data);
// Return the n'th character in the buffer data
static char buff_at(cstlBuffer* buffer, UInt64 n);
// Return a pointer to the beginning of the buffer data
static char* buff_begin(cstlBuffer* buffer);
// Return a pointer to the end of the buffer data
static char* buff_end(cstlBuffer* buffer);
// Is the buffer data empty?
static bool buff_is_empty(cstlBuffer* buffer);
// Assign `new` to the buffer data
static void buff_set(cstlBuffer* buffer, char* new_buff);
// Free the cstlBuffer from it's associated memory
static void buff_free(cstlBuffer* buffer);

// Create a new `cstlBuffer`
static cstlBuffer* buff_new(char* buff_data) {
    cstlBuffer* buffer = (cstlBuffer*)calloc(1, sizeof(cstlBuffer));
    CSTL_CHECK_NOT_NULL(buffer, "Could not allocate memory. Memory full.");

    UInt64 len = buff_data == null ? 0 : (UInt64)strlen(buff_data);

    buffer->data = buff_data;
    buffer->length = len;

    return buffer;
}

// Return the n'th character in the buffer data
static char buff_at(cstlBuffer* buffer, UInt64 n) {
    CSTL_CHECK_NOT_NULL(buffer, "Expected not null");
    CSTL_CHECK_NOT_NULL(buffer->data, "Expected not null");
    
    if(n >= buffer->length)
        return nullchar;

    return (char)buffer->data[n];
}

// Return a pointer to the beginning of the buffer data
static char* buff_begin(cstlBuffer* buffer) {
    CSTL_CHECK_NOT_NULL(buffer, "Expected not null");
    CSTL_CHECK_NOT_NULL(buffer->data, "Expected not null");

    return (char*)buffer->data;
}

// Return a pointer to the end of the buffer data
static char* buff_end(cstlBuffer* buffer) {
    CSTL_CHECK_NOT_NULL(buffer, "Expected not null");
    CSTL_CHECK_NOT_NULL(buffer->data, "Expected not null");

    return (char*)(buffer->data + ((buffer->length - 1) * sizeof(char)));
}

// Is the buffer data empty?
static bool buff_is_empty(cstlBuffer* buffer) {
    CSTL_CHECK_NOT_NULL(buffer, "Expected not null");
    CSTL_CHECK_NOT_NULL(buffer->data, "Expected not null");

    return buffer->length == 0;
}

// Assign `new` to the buffer data
static void buff_set(cstlBuffer* buffer, char* new_buff) {
    CSTL_CHECK_NOT_NULL(buffer, "Expected not null");

    buffer->data = new_buff;
    buffer->length = (UInt64)strlen(new_buff);
}

// Free the cstlBuffer from it's associated memory
static void buff_free(cstlBuffer* buffer) {
    if(buffer == null)
        return;

    buffer->data = null;
    buffer->length = 0;
}

#endif // CSTL_BUFFER_H