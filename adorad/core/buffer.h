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
#include <adorad/core/misc.h>

typedef struct cstlBuffer cstlBuffer;
typedef cstlBuffer Buff;

struct cstlBuffer {
    char* data;    // buffer data
    UInt64 len;    // buffer size
    bool is_utf8;  // UTF-8 Strings
};


static cstlBuffer* buff_new(char* buff_data);
static char buff_at(cstlBuffer* buffer, UInt64 n);
static char* buff_begin(cstlBuffer* buffer);
static char* buff_end(cstlBuffer* buffer);
static bool buff_is_empty(cstlBuffer* buffer);
static void buff_append(cstlBuffer* buffer, cstlBuffer* buff2);
static void buff_set(cstlBuffer* buffer, char* new_buff);
static cstlBuffer* buff_clone(cstlBuffer* buffer);
static UInt64 buff_len(cstlBuffer* buffer);
static void buff_reset(cstlBuffer* buffer);
static cstlBuffer* buff_rev(cstlBuffer* buffer);
static bool buff_cmp(cstlBuffer* buff1, cstlBuffer* buff2);
static bool buff_cmp_nocase(cstlBuffer* buff1, cstlBuffer* buff2);
static cstlBuffer* buff_slice(cstlBuffer* buffer, int begin, int bytes);
static void buff_free(cstlBuffer* buffer);
static inline cstlBuffer* buff_toupper(cstlBuffer* buffer);
static inline cstlBuffer* buff_tolower(cstlBuffer* buffer);

// Create a new `cstlBuffer`
static cstlBuffer* buff_new(char* buff_data) {
    cstlBuffer* buffer = (cstlBuffer*)calloc(1, sizeof(cstlBuffer));
    CSTL_CHECK_NOT_NULL(buffer, "Could not allocate memory. Memory full.");

    buffer->is_utf8 = false;
    buff_set(buffer, buff_data);

    return buffer;
}

// Return the n'th character in the buffer data
static char buff_at(cstlBuffer* buffer, UInt64 n) {
    CSTL_CHECK_NOT_NULL(buffer, "Expected not null");
    CSTL_CHECK_NOT_NULL(buffer->data, "Expected not null");
    
    if(n >= buffer->len)
        return nullchar;

    return (char)buffer->data[n];
}

// Returns a pointer to the beginning of the buffer data
static char* buff_begin(cstlBuffer* buffer) {
    CSTL_CHECK_NOT_NULL(buffer, "Expected not null");
    CSTL_CHECK_NOT_NULL(buffer->data, "Expected not null");

    return cast(char*)buffer->data;
}

// Returns a pointer to the end of the buffer data
static char* buff_end(cstlBuffer* buffer) {
    CSTL_CHECK_NOT_NULL(buffer, "Expected not null");
    CSTL_CHECK_NOT_NULL(buffer->data, "Expected not null");

    return cast(char*)(buffer->data + ((buffer->len - 1) * sizeof(char)));
}

// Is the buffer data empty?
static bool buff_is_empty(cstlBuffer* buffer) {
    CSTL_CHECK_NOT_NULL(buffer, "Expected not null");
    CSTL_CHECK_NOT_NULL(buffer->data, "Expected not null");

    return buffer->len == 0;
}

// Returns the length of the buffer
// Note: The implementation for this was taken from:
// https://github.com/lattera/glibc/blob/master/string/strlen.c
// All due credit for this goes to the rightful author.
static UInt32 __internal_strlength(const char* str, bool is_utf8) {
    if(!is_utf8) {
        const char* char_ptr;
        const unsigned long int* longword_ptr;
        unsigned long int longword, himagic, lomagic;

        // Handle the first few characters by reading one character at a time.
        // Do this until CHAR_PTR is aligned on a longword boundary.
        for (char_ptr = str; ((unsigned long int) char_ptr & (sizeof (longword) - 1)) != 0; ++char_ptr) {
            if (*char_ptr == nullchar)
                return char_ptr - str;
        }

        // All these elucidatory comments refer to 4-byte longwords, but the theory applies equally 
        // well to 8-byte longwords
        longword_ptr = (unsigned long int* ) char_ptr;

        // Bits 31, 24, 16, and 8 of this number are zero. Call these bits the "holes".
        // Note that there is a hole just to the left of each byte, with an extra at the end:
        //     bits:  01111110 11111110 11111110 11111111
        //     bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD
        // The 1-bits make sure that carries propagate to the next 0-bit.
        // The 0-bits provide holes for carries to fall into.
        himagic = 0x80808080L;
        lomagic = 0x01010101L;

        if(sizeof(longword) > 4) {
            // 64-bit version of the magic.
            // Do the shift in two steps to avoid a warning if long has 32 bits.
            himagic = ((himagic << 16) << 16) | himagic;
            lomagic = ((lomagic << 16) << 16) | lomagic;
        }
        if(sizeof(longword) > 8)
            exit(1);

        // Instead of the traditional loop which tests each character, we will test a longword at a time.
        // The tricky part is testing if *any of the four* bytes in the longword in question are zero.
        for (;;) {
            longword = *longword_ptr++;

            if (((longword - lomagic) & ~longword & himagic) != 0) {
                // Which of the bytes was the zero?  If none of them were, it was a misfire; continue the search.
                const char* cp = (const char* ) (longword_ptr - 1);

                if (cp[0] == 0)
                    return cp - str;
                if (cp[1] == 0)
                    return cp - str + 1;
                if (cp[2] == 0)
                    return cp - str + 2;
                if (cp[3] == 0)
                    return cp - str + 3;

                if (sizeof (longword) > 4) {
                    if (cp[4] == 0)
                        return cp - str + 4;
                    if (cp[5] == 0)
                        return cp - str + 5;
                    if (cp[6] == 0)
                        return cp - str + 6;
                    if (cp[7] == 0)
                        return cp - str + 7;
                }
            }
        }
    } 
    // UTF-8
    else {
        UInt32 count = 0;
        for(; *str; count++) {
            UInt8 c = *str;
            UInt32 increment = 0;
                 if(c < 0x80)               increment = 1;
            else if((c & 0xe0) == 0xc0)     increment = 2;
            else if((c & 0xf0) == 0xe0)     increment = 3;
            else if((c & 0xf8) == 0xf0)     increment = 4;
            else return -1;
            
            str += increment;
        }
        return count;
    }
}

// Append `buff2` to the buffer data
// Returns `buffer`
static void buff_append(cstlBuffer* buffer, cstlBuffer* buff2) {
    CSTL_CHECK_NOT_NULL(buffer, "Expected not null");
    CSTL_CHECK_NOT_NULL(buffer->data, "Expected not null");
    CSTL_CHECK_NOT_NULL(buff2, "Expected not null");
    CSTL_CHECK_NOT_NULL(buff2->data, "Expected not null");

    UInt64 new_len = buffer->len + buff2->len + 1;
    char* newstr = cast(char*)calloc(1, new_len);
    strcpy(newstr, buffer->data);
    strcat(newstr, buff2->data);
    buff_set(buffer, newstr);
}

// Append a character to the buffer data
static void buff_append_char(cstlBuffer* buffer, char ch) {
    CSTL_CHECK_NOT_NULL(buffer, "Expected not null");
    CSTL_CHECK_NOT_NULL(buffer->data, "Expected not null");

    UInt64 len = buffer->len;
    char* newstr = cast(char*)calloc(1, len + 1);
    strcpy(newstr, buffer->data);
    newstr[len] = ch;
    newstr[len + 1] = nullchar;
    buff_set(buffer, newstr);
    buffer->len += 2;
}

// Assign `new_buff` to the buffer data
static void buff_set(cstlBuffer* buffer, char* new_buff) {
    CSTL_CHECK_NOT_NULL(buffer, "Expected not null");

    UInt64 len;
    if(!new_buff) {
        len = 0;
        new_buff = "";
    } else {
        len = (UInt64)__internal_strlength(new_buff, buffer->is_utf8);
    }

    buffer->data = new_buff;
    buffer->len = len;
}

// Returns the buffer length
static UInt64 buff_len(cstlBuffer* buffer) {
    return buffer->len;
}

// Free the buffer from its associated memory
static void buff_reset(cstlBuffer* buffer) {
    if(buffer == null)
        return;

    buffer->data = null;
    buffer->len = 0;
}

// Reverse a buffer (non-destructive)
static cstlBuffer* buff_rev(cstlBuffer* buffer) {
    cstlBuffer* rev = buff_new(null);
    UInt64 length = buffer->len;
    if(!length)
        return rev;
    
    char* temp = cast(char*)calloc(1, length + 1);
    for(UInt64 i=0; i<length; i++)
        *(temp + i) = *(buffer->data + length - i - 1);
    
    buff_set(rev, temp);
    return rev;
}

// Compare two buffers (case-sensitive)
// Returns true if `buff1` is lexicographically equal to `buff2`
static bool buff_cmp(cstlBuffer* buff1, cstlBuffer* buff2) {
    if(buff1->len != buff2->len)
        return false;
    
    const unsigned char* s1 = cast(const unsigned char*) buff1->data;
    const unsigned char* s2 = cast(const unsigned char*) buff2->data;
    unsigned char ch1, ch2;

    do {
        ch1 = cast(unsigned char) *s1++;
        ch2 = cast(unsigned char) *s2++;
        if(ch1 == nullchar)
            return (ch1 - ch2) == 0 ? true : false;
    } while(ch1 == ch2);

    return (ch1 - ch2) == 0 ? true : false;
}

// Compare two buffers (ignoring case)
// Returns true if `buff1` is lexicographically equal to `buff2`
static bool buff_cmp_nocase(cstlBuffer* buff1, cstlBuffer* buff2) {
    if(buff1->len != buff2->len)
        return false;
    
    const unsigned char* s1 = cast(const unsigned char*) buff1->data;
    const unsigned char* s2 = cast(const unsigned char*) buff2->data;
    int result;

    if(s1 == s2)
        return true;
    
    while((result = toLower(*s1) - toLower(*s2++)) == 0) {
        if(*s1++ == nullchar)
            break;
    }
    return result == 0 ? true : false;
}

// Get a slice of a buffer
static cstlBuffer* buff_slice(cstlBuffer* buffer, int begin, int bytes) {
    CSTL_CHECK_NOT_NULL(buffer, "`buffer` cannot be null");
    CSTL_CHECK_GE(begin, 0);
    CSTL_CHECK_GT(bytes, 0);

    cstlBuffer* slice = buff_new(null);
    CSTL_CHECK_NOT_NULL(slice, "`slice` cannot be null");
    char* temp = cast(char*)calloc(1, bytes);
    strncpy(temp, &(buffer->data[begin]), bytes);
    buff_set(slice, temp);
    CSTL_CHECK_NOT_NULL(slice, "`slice source` cannot be null");
    return slice;
}

// Clone a buffer
static cstlBuffer* buff_clone(cstlBuffer* buffer) {
    CSTL_CHECK_NOT_NULL(buffer, "Cannot clone a null buffer :(");
    cstlBuffer* clone = buff_new(null);
    char* dest = cast(char*)calloc(1, buff_len(buffer));
    char* source = buffer->data;

    if(source) {
        char* str = dest;
        while(*source)
            *str++ = *source++;
    }
    buff_set(clone, dest);
    return clone;
}

// Clone a buffer (upto `n` chars)
static cstlBuffer* buff_clone_n(cstlBuffer* buffer, int n) {
    CSTL_CHECK_NOT_NULL(buffer, "Cannot clone a null buffer :(");
    CSTL_CHECK_GT(n, 0);
    CSTL_CHECK_LT(n, buffer->len);
    cstlBuffer* clone = buff_new(null);
    char* dest = cast(char*)calloc(1, buff_len(buffer));
    char* source = buffer->data;

    if(source) {
        char* str = dest;
        while(n > 0 && *source) {
            *str++ = *source++;
            n--;
        }
    }
    buff_set(clone, dest);
    return clone;
}


// Free the buffer from its associated memory
static void buff_free(cstlBuffer* buffer) {
    if(buffer)
        free(buffer);
}

// Convert a buffer to lowercase
static inline cstlBuffer* buff_tolower(cstlBuffer* buffer) {
    cstlBuffer* lower = buff_new(null);
    if(!buffer->data) 
        return lower;

    char* temp = cast(char*)calloc(1, buffer->len);    
    strcpy(temp, buffer->data);
    int i = 0;
    while(*temp) {
        *temp = toLower(*temp);
        temp++;
        i++;
    }
    // Backtrack `i` times
    temp -= i;
    buff_set(lower, temp);
    return lower;
}

// Convert a buffer to uppercase
static inline cstlBuffer* buff_toupper(cstlBuffer* buffer) {
    cstlBuffer* upper = buff_new(null);
    if(!buffer->data) 
        return upper;

    char* temp = cast(char*)calloc(1, buffer->len);    
    strcpy(temp, buffer->data);
    int i = 0;
    while(*temp) {
        *temp = toUpper(*temp);
        temp++;
        i++;
    }
    // Backtrack `i` times
    temp -= i;
    buff_set(upper, temp);
    return upper;
}

#endif // CSTL_BUFFER_H