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

#ifndef CORETEN_BUFFER_H
#define CORETEN_BUFFER_H

#include <adorad/core/debug.h>
#include <adorad/core/types.h>
#include <adorad/core/char.h>
#include <adorad/core/misc.h>

/*
    A `cstlBuffer` is a Fixed-Size Buffer.
    It works like a string, except that the actual type is just a pointer to the first `char` element
    with `len` describing its length.
*/

typedef struct cstlBuffer cstlBuffer;
typedef cstlBuffer Buff;
// These are usually when you want stack objects.
// Isn't any different from `cstlBuffer`, but it makes it easier to read.
typedef cstlBuffer cstlBuffView;
typedef cstlBuffer BuffView;

struct cstlBuffer {
    char* data;    // buffer data
    UInt64 len;    // buffer size
    // bool is_utf8;  // UTF-8 Strings
};

#define BUFF_NEW(buff_data)     buff_new(buff_data)
#define BV(cstr)                buffview_new_from_len(cstr, sizeof(cstr) - 1)
#define BV_FMT                  "%.*s"
#define BV_ARG(bv)              (int)(bv).len, (bv).data

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
cstlBuffer* buff_slice(cstlBuffer* buffer, int begin, int num_bytes);
void buff_free(cstlBuffer* buffer);
cstlBuffer* buff_toupper(cstlBuffer* buffer);
cstlBuffer* buff_tolower(cstlBuffer* buffer);

cstlBuffView buffview_new(char* data); // returns a stack object
cstlBuffView buffview_new_from_len(char* data, UInt64 len); // returns a stack object
char buffview_at(cstlBuffView* view, UInt64 n);
char* buffview_begin(cstlBuffView* view);
char* buffview_end(cstlBuffView* view);
cstlBuffView buffview_rev(cstlBuffView* view);
void buffview_set(cstlBuffView* view, char* data);
bool buffview_cmp(cstlBuffView* view1, cstlBuffView* view2);
bool buffview_cmp_nocase(cstlBuffView* view1, cstlBuffView* view2);


#ifdef CORETEN_IMPL
    #include <string.h>
    
    // Create a new `cstlBuffer`
    cstlBuffer* buff_new(char* buff_data) {
        cstlBuffer* buffer = cast(cstlBuffer*)calloc(1, sizeof(cstlBuffer));
        CORETEN_ENFORCE_NN(buffer, "Could not allocate memory. Memory full.");

        buff_set(buffer, buff_data);

        return buffer;
    }

    // Return the n'th character in the buffer data
    char buff_at(cstlBuffer* buffer, UInt64 n) {
        CORETEN_ENFORCE_NN(buffer, "Expected not null");
        CORETEN_ENFORCE_NN(buffer->data, "Expected not null");
        
        if(n >= buffer->len)
            return nullchar;

        return cast(char)buffer->data[n];
    }

    // Returns a pointer to the beginning of the buffer data
    char* buff_begin(cstlBuffer* buffer) {
        CORETEN_ENFORCE_NN(buffer, "Expected not null");
        CORETEN_ENFORCE_NN(buffer->data, "Expected not null");

        return cast(char*)buffer->data;
    }

    // Returns a pointer to the end of the buffer data
    char* buff_end(cstlBuffer* buffer) {
        CORETEN_ENFORCE_NN(buffer, "Expected not null");
        CORETEN_ENFORCE_NN(buffer->data, "Expected not null");

        return cast(char*)(buffer->data + ((buffer->len - 1) * sizeof(char)));
    }

    // Is the buffer data empty?
    bool buff_is_empty(cstlBuffer* buffer) {
        CORETEN_ENFORCE_NN(buffer, "Expected not null");
        CORETEN_ENFORCE_NN(buffer->data, "Expected not null");

        return buffer->len == 0;
    }

    // Returns the length of the buffer
    // Note: The implementation for this was taken from:
    // https://github.com/lattera/glibc/blob/master/string/strlen.c
    // All due credit for this goes to the rightful author.
    // UInt32 __internal_strlength(const char* str, bool is_utf8) {
    static UInt32 __internal_strlength(const char* str) {
        // if(!is_utf8) {
            const char* char_ptr;
            const unsigned long int* longword_ptr;
            unsigned long int longword, himagic, lomagic;

            // Handle the first few characters by reading one character at a time.
            // Do this until CHAR_PTR is aligned on a longword boundary.
            for(char_ptr = str; (cast(unsigned long int)char_ptr & (sizeof(longword) - 1)) != 0; ++char_ptr) {
                if(*char_ptr == nullchar)
                    return char_ptr - str;
            }

            // All these elucidatory comments refer to 4-byte longwords, but the theory applies equally 
            // well to 8-byte longwords
            longword_ptr = cast(unsigned long int*)char_ptr;

            // Bits 31, 24, 16, && 8 of this number are zero. Call these bits the "holes".
            // Note that there is a hole just to the left of each byte, with an extra at the end:
            //     bits:  01111110 11111110 11111110 11111111
            //     bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD
            // The 1-bits make sure that carries propagate to the next 0-bit.
            // The 0-bits provide holes forcarries to fall into.
            himagic = 0x80808080L;
            lomagic = 0x01010101L;

            if(sizeof(longword) > 4) {
                // 64-bit version of the magic.
                // Do the shift in two steps to avoid a warning if long has 32 bits.
                himagic = ((himagic << 16) << 16) | himagic;
                lomagic = ((lomagic << 16) << 16) | lomagic;
            }
            if(sizeof(longword) > 8)
                abort();

            // Instead of the traditional loop which tests each character, we will test a longword at a time.
            // The tricky part is testing if *any of the four* bytes in the longword in question are zero.
            for(;;) {
                longword = *longword_ptr++;

                if(((longword - lomagic) & ~longword & himagic) != 0) {
                    // Which of the bytes was the zero?  If none of them were, it was a misfire; continue the search.
                    const char* cp = cast(const char*)(longword_ptr - 1);

                    if(cp[0] == 0)
                        return cp - str;
                    if(cp[1] == 0)
                        return cp - str + 1;
                    if(cp[2] == 0)
                        return cp - str + 2;
                    if(cp[3] == 0)
                        return cp - str + 3;

                    if(sizeof(longword) > 4) {
                        if(cp[4] == 0)
                            return cp - str + 4;
                        if(cp[5] == 0)
                            return cp - str + 5;
                        if(cp[6] == 0)
                            return cp - str + 6;
                        if(cp[7] == 0)
                            return cp - str + 7;
                    }
                }
            }
        // } 
        // // UTF-8
        // else {
        //     UInt32 count = 0;
        //     for(; *str; count++) {
        //         UInt8 c = *str;
        //         UInt32 increment = 0;
        //              if(c < 0x80)               increment = 1;
        //         else if((c & 0xe0) == 0xc0)     increment = 2;
        //         else if((c & 0xf0) == 0xe0)     increment = 3;
        //         else if((c & 0xf8) == 0xf0)     increment = 4;
        //         else return -1;
                
        //         str += increment;
        //     }
        //     return count;
        // }
    }

    // Append `buff2` to the buffer data
    // Returns `buffer`
    void buff_append(cstlBuffer* buffer, cstlBuffer* buff2) {
        CORETEN_ENFORCE_NN(buffer, "Expected not null");
        CORETEN_ENFORCE_NN(buffer->data, "Expected not null");
        CORETEN_ENFORCE_NN(buff2, "Expected not null");
        CORETEN_ENFORCE_NN(buff2->data, "Expected not null");

        UInt64 new_len = buffer->len + buff2->len + 1;
        char* newstr = cast(char*)calloc(1, new_len);
        strcpy(newstr, buffer->data);
        strcat(newstr, buff2->data);
        buff_set(buffer, newstr);
    }

    // Append a character to the buffer data
    void buff_append_char(cstlBuffer* buffer, char ch) {
        CORETEN_ENFORCE_NN(buffer, "Expected not null");
        CORETEN_ENFORCE_NN(buffer->data, "Expected not null");

        UInt64 len = buffer->len;
        char* newstr = cast(char*)calloc(1, len + 1);
        strcpy(newstr, buffer->data);
        newstr[len] = ch;
        newstr[len + 1] = nullchar;
        buff_set(buffer, newstr);
        buffer->len += 2;
    }

    // Assign `new_buff` to the buffer data
    void buff_set(cstlBuffer* buffer, char* new_buff) {
        CORETEN_ENFORCE_NN(buffer, "Expected not null");

        UInt64 len;
        if(NONE(new_buff)) {
            len = 0;
            new_buff = "";
        } else {
            // len = (UInt64)__internal_strlength(new_buff, buffer->is_utf8);
            len = cast(UInt64)__internal_strlength(new_buff);
        }

        buffer->data = new_buff;
        buffer->len = len;
    }

    // Returns the buffer length
    UInt64 buff_len(cstlBuffer* buffer) {
        return buffer->len;
    }

    // Free the buffer from its associated memory
    void buff_reset(cstlBuffer* buffer) {
        if(NONE(buffer))
            return;

        buffer->data = null;
        buffer->len = 0;
    }

    // Reverse a buffer (non-destructive)
    cstlBuffer* buff_rev(cstlBuffer* buffer) {
        cstlBuffer* rev = buff_new(null);
        UInt64 length = buffer->len;
        if(!length)
            return rev;
        
        char* temp = cast(char*)calloc(1, length + 1);
        for(UInt64 i=0; i<length; i++)
            *(temp + i) = *(buffer->data + length - i - 1);
        
        temp[length] = nullchar;
        buff_set(rev, temp);
        return rev;
    }

    // Compare two buffers (case-sensitive)
    // Returns true if `buff1` is lexicographically equal to `buff2`
    bool buff_cmp(cstlBuffer* buff1, cstlBuffer* buff2) {
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
    bool buff_cmp_nocase(cstlBuffer* buff1, cstlBuffer* buff2) {
        if(buff1->len != buff2->len)
            return false;
        
        const unsigned char* s1 = cast(const unsigned char*) buff1->data;
        const unsigned char* s2 = cast(const unsigned char*) buff2->data;
        int result;

        if(s1 == s2)
            return true;
        
        while((result = char_to_lower(*s1) - char_to_lower(*s2++)) == 0) {
            if(*s1++ == nullchar)
                break;
        }
        return result == 0 ? true : false;
    }

    // Get a slice of a buffer
    cstlBuffer* buff_slice(cstlBuffer* buffer, int begin, int num_bytes) {
        CORETEN_ENFORCE_NN(buffer, "`buffer` cannot be null");
        CORETEN_ENFORCE(begin >= 0);
        CORETEN_ENFORCE(num_bytes >  0);

        cstlBuffer* slice = buff_new(null);
        CORETEN_ENFORCE_NN(slice, "`slice` cannot be null");
        char* temp = cast(char*)calloc(1, num_bytes);
        strncpy(temp, &(buffer->data[begin]), num_bytes);
        buff_set(slice, temp);
        CORETEN_ENFORCE_NN(slice, "`slice source` cannot be null");
        return slice;
    }

    // Clone a buffer
    cstlBuffer* buff_clone(cstlBuffer* buffer) {
        CORETEN_ENFORCE_NN(buffer, "Cannot clone a null buffer :(");
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
    cstlBuffer* buff_clone_n(cstlBuffer* buffer, int n) {
        CORETEN_ENFORCE_NN(buffer, "Cannot clone a null buffer :(");
        CORETEN_ENFORCE(n > 0);
        CORETEN_ENFORCE(n > buffer->len);
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
    void buff_free(cstlBuffer* buffer) {
        if(SOME(buffer))
            free(buffer);
    }

    // Convert a buffer to lowercase
    cstlBuffer* buff_tolower(cstlBuffer* buffer) {
        cstlBuffer* lower = buff_new(null);
        if(NONE(buffer->data))
            return lower;

        char* temp = cast(char*)calloc(1, buffer->len);    
        strcpy(temp, buffer->data);
        int i = 0;
        while(*temp) {
            *temp = char_to_lower(*temp);
            temp++;
            i++;
        }
        // Backtrack `i` times
        temp -= i;
        buff_set(lower, temp);
        return lower;
    }

    // Convert a buffer to uppercase
    cstlBuffer* buff_toupper(cstlBuffer* buffer) {
        cstlBuffer* upper = buff_new(null);
        if(NONE(buffer->data))
            return upper;

        char* temp = cast(char*)calloc(1, buffer->len);    
        strcpy(temp, buffer->data);
        int i = 0;
        while(*temp) {
            *temp = char_to_upper(*temp);
            temp++;
            i++;
        }
        // Backtrack `i` times
        temp -= i;
        buff_set(upper, temp);
        return upper;
    }

    /*
        cstlBuffView
    */
    // Returns a Stack object as opposed to a pointer
    cstlBuffView buffview_new(char* buff_data) {
        cstlBuffView view;
        buffview_set(&view, buff_data);

        return view;
    }

    // Returns a Stack object as opposed to a pointer
    cstlBuffView buffview_new_from_len(char* buff_data, UInt64 len) {
        cstlBuffView view;
        view.data = buff_data;
        view.len = len;
        return view;
    }

    // Non-destructive reverse of a buffer view
    cstlBuffView buffview_rev(cstlBuffView* view) {
        cstlBuffView rev = buffview_new(null);
        UInt64 length = view->len;
        if(!length)
            return rev;
        
        char* temp = cast(char*)calloc(1, length + 1);
        for(UInt64 i=0; i<length; i++)
            *(temp + i) = *(view->data + length - i - 1);
        
        buffview_set(&rev, temp);
        
        return rev;
    }

    // Assign `new_data` to the view data
    void buffview_set(cstlBuffView* view, char* new_data) {
        CORETEN_ENFORCE_NN(view, "Expected not null");

        UInt64 len;
        if(!new_data) {
            len = 0;
            new_data = "";
        } else {
            len = cast(UInt64)__internal_strlength(new_data);
        }

        view->data = new_data;
        view->len = len;
    }

    char buffview_at(cstlBuffView* view, UInt64 n) {
        CORETEN_ENFORCE_NN(view, "Expected not null");
        CORETEN_ENFORCE_NN(view->data, "Expected not null");

        if(n >= view->len)
            return nullchar;
        
        return (char)view->data[n];
    }

    // Returns a pointer to the beginning of the view data
    char* buffview_begin(cstlBuffView* view) {
        CORETEN_ENFORCE_NN(view, "Expected not null");
        CORETEN_ENFORCE_NN(view->data, "Expected not null");

        return cast(char*)view->data;
    }

    // Returns a pointer to the end of the view data
    char* buffview_end(cstlBuffView* view) {
        CORETEN_ENFORCE_NN(view, "Expected not null");
        CORETEN_ENFORCE_NN(view->data, "Expected not null");

        return cast(char*)(view->data + ((view->len - 1) * sizeof(char)));
    }

    // Compare two BuffViews (case-sensitive)
    // Returns true if `view1` is lexicographically equal to `view2`
    bool buffview_cmp(cstlBuffView* view1, cstlBuffView* view2) {
        if(view1->len != view2->len)
            return false;
        
        const unsigned char* s1 = cast(const unsigned char*) view1->data;
        const unsigned char* s2 = cast(const unsigned char*) view2->data;
        unsigned char ch1, ch2;

        do {
            ch1 = cast(unsigned char) *s1++;
            ch2 = cast(unsigned char) *s2++;
            if(ch1 == nullchar)
                return (ch1 - ch2) == 0 ? true : false;
        } while(ch1 == ch2);

        return (ch1 - ch2) == 0 ? true : false;
    }

    // Compare two BuffViews (ignoring case)
    // Returns true if `view1` is lexicographically equal to `view2`
    bool buffview_cmp_nocase(cstlBuffView* view1, cstlBuffView* view2) {
        if(view1->len != view2->len)
            return false;
        
        const unsigned char* s1 = cast(const unsigned char*) view1->data;
        const unsigned char* s2 = cast(const unsigned char*) view2->data;
        int result;

        if(s1 == s2)
            return true;
        
        while((result = char_to_lower(*s1) - char_to_lower(*s2++)) == 0) {
            if(*s1++ == nullchar)
                break;
        }
        return result == 0 ? true : false;
    }

    // Append `view2` to the end of `view`.
    // Returns `view`
    void buffview_append(cstlBuffView* view, cstlBuffView* view2) {
        CORETEN_ENFORCE_NN(view, "Expected not null");
        CORETEN_ENFORCE_NN(view->data, "Expected not null");
        CORETEN_ENFORCE_NN(view2, "Expected not null");
        CORETEN_ENFORCE_NN(view2->data, "Expected not null");

        UInt64 new_len = view->len + view2->len + 1;
        char* newstr = cast(char*)calloc(1, new_len);
        strcpy(newstr, view->data);
        strcat(newstr, view2->data);
        buffview_set(view, newstr);
    }

    // Append a character to the view data
    void buffview_append_char(cstlBuffView* view, char ch) {
        CORETEN_ENFORCE_NN(view, "Expected not null");
        CORETEN_ENFORCE_NN(view->data, "Expected not null");

        UInt64 len = view->len;
        char* newstr = cast(char*)calloc(1, len + 1);
        strcpy(newstr, view->data);
        newstr[len] = ch;
        newstr[len + 1] = nullchar;
        buffview_set(view, newstr);
        view->len += 2;
    }

#endif // CORETEN_IMPL

#endif // CORETEN_BUFFER_H