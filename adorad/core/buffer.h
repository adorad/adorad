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
#include <adorad/core/debug.h>
#include <adorad/core/types.h>
#include <adorad/core/string.h>

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
// Append `buff2` to the buffer data
static void buff_append(cstlBuffer* buffer, cstlBuffer* buff2);
// Assign `new_buff` to the buffer data
static void buff_set(cstlBuffer* buffer, char* new_buff);
// Reset a buffer
static void buff_reset(cstlBuffer* buffer);
// Reverse a buffer (non-destructive)
static cstlBuffer* buff_rev(cstlBuffer* buffer);
// Compare two buffers (case-sensitive)
// Returns true if `buff1` is lexicographically equal to `buff2`
static bool buff_cmp(cstlBuffer* buff1, cstlBuffer* buff2);
// Compare two buffers (ignoring case)
// Returns true if `buff1` is lexicographically equal to `buff2`
static bool buff_cmp_nocase(cstlBuffer* buff1, cstlBuffer* buff2);
// Free the buffer from its associated memory
static void buff_free(cstlBuffer* buffer);

// Create a new `cstlBuffer`
static cstlBuffer* buff_new(char* buff_data) {
    cstlBuffer* buffer = (cstlBuffer*)calloc(1, sizeof(cstlBuffer));
    CSTL_CHECK_NOT_NULL(buffer, "Could not allocate memory. Memory full.");

    buff_set(buffer, buff_data);

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

// Append `buff2` to the buffer data
// Returns `buffer`
static void buff_append(cstlBuffer* buffer, cstlBuffer* buff2) {
    CSTL_CHECK_NOT_NULL(buffer, "Expected not null");
    CSTL_CHECK_NOT_NULL(buffer->data, "Expected not null");
    CSTL_CHECK_NOT_NULL(buff2, "Expected not null");
    CSTL_CHECK_NOT_NULL(buff2->data, "Expected not null");

    UInt64 new_len = buffer->length + buff2->length + 1;
    char* newstr = (char*)calloc(1, new_len);
    strcpy(newstr, buffer->data);
    strcat(newstr, buff2->data);
    buff_set(buffer, newstr);
}

// Append a character to the buffer data
static void buff_append_char(cstlBuffer* buffer, char ch) {
    CSTL_CHECK_NOT_NULL(buffer, "Expected not null");
    CSTL_CHECK_NOT_NULL(buffer->data, "Expected not null");

    UInt64 len = buffer->length;
    char* newstr = (char*)calloc(1, len + 1);
    strcpy(newstr, buffer->data);
    newstr[len] = ch;
    newstr[len + 1] = nullchar;
    buff_set(buffer, newstr);
    buffer->length += 2;
}

// Assign `new_buff` to the buffer data
static void buff_set(cstlBuffer* buffer, char* new_buff) {
    CSTL_CHECK_NOT_NULL(buffer, "Expected not null");

    UInt64 len;
    if(!new_buff) {
        len = 0;
        new_buff = "";
    } else {
        len = (UInt64)strlen(new_buff);
    }

    buffer->data = new_buff;
    buffer->length = len;
}

// Free the buffer from its associated memory
static void buff_reset(cstlBuffer* buffer) {
    if(buffer == null)
        return;

    buffer->data = null;
    buffer->length = 0;
}

// Reverse a buffer (non-destructive)
static Buff* buff_rev(Buff* buffer) {
    Buff* rev = buff_new(null);
    UInt64 length = buffer->length;
    if(!length)
        return rev;
    
    char* temp = (char*)calloc(1, length + 1);
    for(UInt64 i=0; i<length; i++)
        *(temp + i) = *(buffer->data + length - i - 1);
    
    buff_set(rev, temp);
    return rev;
}

// Compare two buffers (case-sensitive)
// Returns true if `buff1` is lexicographically equal to `buff2`
static bool buff_cmp(cstlBuffer* buff1, cstlBuffer* buff2) {
    if(buff1->length != buff2->length)
        return false;
    
    const unsigned char* s1 = (const unsigned char*) buff1->data;
    const unsigned char* s2 = (const unsigned char*) buff2->data;
    unsigned char ch1, ch2;

    do {
        ch1 = (unsigned char) *s1++;
        ch2 = (unsigned char) *s2++;
        if(ch1 == nullchar)
            return (ch1 - ch2) == 0 ? true : false;
    } while(ch1 == ch2);

    return (ch1 - ch2) == 0 ? true : false;
}

// Compare two buffers (ignoring case)
// Returns true if `buff1` is lexicographically equal to `buff2`
static bool buff_cmp_nocase(cstlBuffer* buff1, cstlBuffer* buff2) {
    if(buff1->length != buff2->length)
        return false;
    
    const unsigned char* s1 = (const unsigned char*) buff1->data;
    const unsigned char* s2 = (const unsigned char*) buff2->data;
    unsigned char ch1, ch2;
    int result;

    if(s1 == s2)
        return true;
    
    while((result = toLower(*s1) - toLower(*s2++)) == 0) {
        if(*s1++ == nullchar)
            break;
    }
    return result == 0 ? true : false;

}

// Free the buffer from its associated memory
static void buff_free(cstlBuffer* buffer) {
    free(buffer);
}

#endif // CSTL_BUFFER_H