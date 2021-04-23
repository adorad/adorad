#ifndef CSTL_STRING_H
#define CSTL_STRING_H

#include <Hazel/Core/memory.h>
#include <Hazel/Core/misc.h>
#include <Hazel/Core/Types.h>

#ifdef __cplusplus
extern "C" {
#endif

// Char Things ==========================================
CSTL_DEF inline char toLower(char c);
CSTL_DEF inline char toUpper(char c);
CSTL_DEF inline bool isWhitespace(char c);
CSTL_DEF inline bool isLower(char c);
CSTL_DEF inline bool isUpper(char c);
CSTL_DEF inline bool isLetter(char c);
CSTL_DEF inline bool isDigit(char c);
CSTL_DEF inline bool isHexDigit(char c);
CSTL_DEF inline bool isAlpha(char c);
CSTL_DEF inline bool isAlphanumeric(char c);
CSTL_DEF inline Int32 digitToInt(char c);
CSTL_DEF inline Int32 hexDigitToInt(char c);

// ASCII only
CSTL_DEF inline void strToLower(char* str);
CSTL_DEF inline void strToUpper(char* str);


// A less fucking crazy strtok!
CSTL_DEF const char* strTok(char* output, char const* src, char const* delimit);

CSTL_DEF bool strHasPrefix(char const* str, char const* prefix);
CSTL_DEF bool strHasSuffix(char const* str, char const* suffix);

CSTL_DEF char const* charFirstOccurence(char const* str, char c);
CSTL_DEF char const* charLastOccurence (char const* str, char c);

CSTL_DEF char* strCat(char* dest, char* source);

CSTL_DEF Ll    strLen(const char* str);
CSTL_DEF Ll    strnLen(const char* str, Ll max_len);
CSTL_DEF Int32 strCmp(const char* str1, const char* str2);
CSTL_DEF Int32 strnCmp(const char* str1, const char* str2, Ll len);
CSTL_DEF char* strCopy(char *dest, const char* source);
CSTL_DEF char* strnCopy(char *dest, const char* source, Ll len);
CSTL_DEF Ll    strlCopy(char *dest, const char* source, Ll len);
CSTL_DEF char* strRev(char *str); //  ASCII only


// ////////////////////////////////////////////////////////////////
// // UTF-8 Handling
// // Does not check if utf-8 string is valid
// CSTL_DEF Ll gb_utf8_strlen (UInt8 const* str);
// CSTL_DEF Ll gb_utf8_strnlen(UInt8 const* str, Ll max_len);

// // NOTE(jasmcaus): Windows doesn't handle 8 bit filenames well ('cause Micro$hit)
// CSTL_DEF UInt16 *gb_utf8_to_ucstr2    (UInt16 *buffer, Ll len, UInt8 const* str);
// CSTL_DEF UInt8 * gb_ucstr2_to_utf8    (UInt8 *buffer, Ll len, UInt16 const* str);
// CSTL_DEF UInt16 *gb_utf8_to_ucstr2_buf(UInt8 const* str);   // NOTE(jasmcaus): Uses locally persisting buffer
// CSTL_DEF UInt8 * gb_ucstr2_to_utf8_buf(UInt16 const* str); // NOTE(jasmcaus): Uses locally persisting buffer

// // NOTE(jasmcaus): Returns size of codepoint in bytes
// CSTL_DEF Ll gb_utf8_decode        (UInt8 const* str, Ll str_len, Rune *codepoint);
// CSTL_DEF Ll gb_utf8_codepoint_size(UInt8 const* str, Ll str_len);
// CSTL_DEF Ll gb_utf8_encode_rune   (UInt8 buf[4], Rune r);


//
//
//
//
//
// Implementation
//
//
//
//
//

static inline char toLower(char c) {
    if(c >= 'A' && c <= 'Z') 
        return 'a' + (c - 'A');
    return c;
}

static inline char toUpper(char c) {
    if(c >= 'a' && c <= 'z') 
        return 'A' + (c - 'a');
    return c;
}

static inline bool isWhitespace(char c) {
    if(c == ' '  || c == '\t' || c == '\n' || c == '\r' || 
       c == '\f' || c == '\v')
       return true; 
    return false;
}

static inline bool isUpper(char c) {
    return c>='A' && c<='Z';
}

static inline bool isLower(char c) {
    return c>='a' && c<='z';
}

static inline bool isLetter(char c) {
    return  (c >= 'a' && c <= 'z') || 
            (c >= 'A' && c <= 'Z') || 
            (c == '_') ;
}

static inline bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static inline bool isHexDigit(char c) {
    return  isDigit(c)                    ||
            CSTL_IS_BETWEEN(c, 'a', 'f') ||
            CSTL_IS_BETWEEN(c, 'A', 'F'); 
}

static inline bool isAlpha(char c) {
    return isUpper(c) || isLower(c);
}

static inline bool isAlphanumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

static inline Int32 digitToInt(char c) {
    return isDigit(c) ? c-'0' : c-'W';
}

static inline Int32 hexDigitToInt(char c) {
    if(isDigit(c))
        return digitToInt(c);

    else if (CSTL_IS_BETWEEN(c, 'a', 'f'))
        return c-'a' + 10; 

    else if (CSTL_IS_BETWEEN(c, 'A', 'F'))
        return c-'A' + 10; 

    return -1; 
}

static inline void strToLower(char* str) {
    if(!str) return; 
    while(*str) {
        *str = toLower(*str);
        str++;
    }
}

static inline void strToUpper(char* str) {
    if(!str) return; 
    while(*str) {
        *str = toUpper(*str);
        str++;
    }
}


CSTL_DEF const char* strTok(char* output, char const* src, char const* delimit) {

}

CSTL_DEF bool strHasPrefix(char const* str, char const* prefix) {

}

CSTL_DEF bool strHasSuffix(char const* str, char const* suffix) {

}

CSTL_DEF char const* charFirstOccurence(char const* str, char c) {

}

CSTL_DEF char const* charLastOccurence (char const* str, char c) {

}


// // The functions below are part of C's actual library. 
// // They're here as part of CSTL so that you don't have to include multiple header files (such as C's <string.h>)
// // 
// // Obtained from here: https://github.com/lattera/glibc/blob/master/string
// // 
// /* Copyright (C) 1991-2018 Free Software Foundation, Inc.
//     This file is part of the GNU C Library.
//     Written by Torbjorn Granlund (tege@sics.se),
//     with help from Dan Sahlin (dan@sics.se);
//     commentary by Jim Blandy (jimb@ai.mit.edu).
//     The GNU C Library is free software; you can redistribute it and/or
//     modify it under the terms of the GNU Lesser General Public
//     License as published by the Free Software Foundation; either
//     version 2.1 of the License, or (at your option) any later version.
//     The GNU C Library is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//     Lesser General Public License for more details.
//     You should have received a copy of the GNU Lesser General Public
//     License along with the GNU C Library; if not, see
//     <http://www.gnu.org/licenses/>.  
// */

// CSTL_DEF Ll strLen(const char* str) {
//     const char* char_ptr;
//     const unsigned long int* longword_ptr;
//     unsigned long int longword, himagic, lomagic;

//     // Handle the first few characters by reading one character at a time.
//     // Do this until CHAR_PTR is aligned on a longword boundary. 
//     for (char_ptr = str; ((unsigned long int) char_ptr & (sizeof (longword) - 1)) != 0; ++char_ptr)
//         if (*char_ptr == nullchar)
//         return char_ptr - str;

//     // All these elucidatory comments refer to 4-byte longwords,
//     // but the theory applies equally well to 8-byte longwords.  */

//     longword_ptr = (unsigned long int *) char_ptr;

//     // Bits 31, 24, 16, and 8 of this number are zero.  Call these bits the "holes."  Note that there is a hole just to the 
//     // left of each byte, with an extra at the end: 
//     //       bits:  01111110 11111110 11111110 11111111
//     //       bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD
//     // The 1-bits make sure that carries propagate to the next 0-bit.
//     // The 0-bits provide holes for carries to fall into. 
//     himagic = 0x80808080L;
//     lomagic = 0x01010101L;

//     if (sizeof(longword) > 4){
//         // 64-bit version of the magic. 
//         // Do the shift in two steps to avoid a warning if long has 32 bits.
//         himagic = ((himagic << 16) << 16) | himagic;
//         lomagic = ((lomagic << 16) << 16) | lomagic;
//     }

//     if (sizeof(longword) > 8)
//         abort();

//     // Instead of the traditional loop which tests each character, we will test a longword at a time. The tricky part is 
//     // testing if *any of the four* bytes in the longword in question are zero.
//     for(;;) {
//         longword = *longword_ptr++;

//         if (((longword - lomagic) & ~longword & himagic) != 0) {
//             // Which of the bytes was the zero?  If none of them were, it was a misfire; continue the search.

//             const char *cp = (const char *) (longword_ptr - 1);

//             if (cp[0] == 0)  return cp - str;
//             if (cp[1] == 0)  return cp - str + 1;
//             if (cp[2] == 0)  return cp - str + 2;
//             if (cp[3] == 0)  return cp - str + 3;

//             if (sizeof(longword) > 4) {
//                 if (cp[4] == 0) return cp - str + 4;
//                 if (cp[5] == 0) return cp - str + 5;
//                 if (cp[6] == 0) return cp - str + 6;
//                 if (cp[7] == 0) return cp - str + 7;
//             }
//         }
//     }
// }

// CSTL_DEF Ll strnLen(const char* str, Ll max_len) {

// }

// CSTL_DEF Int32 strCmp(const char* str1, const char* str2) {
//     // Compare S1 and S2, returning less than, equal to or greater than zero if 'str1' is lexicographically less than,
//     // equal to or greater than 'str2'
//     const unsigned char* str1 = (const unsigned char *)str1;
//     const unsigned char* str2 = (const unsigned char *)str2;
//     unsigned char c1, c2;

//     do {
//         c1 = (unsigned char) *str1++;
//         c2 = (unsigned char) *str2++;
        
//         if (c1 == nullchar)
//             return c1 - c2;
//     } while (c1 == c2);

//     return c1 - c2;
// }

// CSTL_DEF Int32 strnCmp(const char* str1, const char* str2, Ll n) {
//     unsigned char c1 = nullchar;
//     unsigned char c2 = nullchar;

//     if (n >= 4) {
//         Ull n4 = n >> 2;

//         do {
//             c1 = (unsigned char) *str1++;
//             c2 = (unsigned char) *str2++;

//             if (c1 == nullchar || c1 != c2) return c1 - c2;

//             c1 = (unsigned char) *str1++;
//             c2 = (unsigned char) *str2++;
//             if (c1 == nullchar || c1 != c2) return c1 - c2;

//             c1 = (unsigned char) *str1++;
//             c2 = (unsigned char) *str2++;
//             if (c1 == nullchar || c1 != c2) return c1 - c2;

//             c1 = (unsigned char) *str1++;
//             c2 = (unsigned char) *str2++;
//             if (c1 == nullchar || c1 != c2) return c1 - c2;
//         } while (--n4 > 0);

//         n &= 3;
//     }

//     while (n > 0) {
//         c1 = (unsigned char) *str1++;
//         c2 = (unsigned char) *str2++;

//         if (c1 == nullchar || c1 != c2)
//             return c1 - c2;

//         n--;
//     }

//     return c1 - c2;
// }

// CSTL_DEF char* strCopy(char *dest, const char* source) {
//     return memcpy(dest, source, strLen(source) + 1);
// }

// CSTL_DEF char* strnCopy(char *dest, const char* source, Ll n) {
//     size_t size = strnLen(source, n);

//     if (size != n)
//         memset (dest + size, nullchar, n - size);

//     return memcpy(dest, source, size);
// }

// CSTL_DEF Ll strlCopy(char *dest, const char* source, Ll len) {

// }

// CSTL_DEF char* strRev(char *str) {

// }

// CSTL_DEF char* strCat(char* dest, char* source) {
//     // Append 'source' at the end of 'dest'
//     strCopy(dest + strLen(dest), source);
//     return dest;
// }

#if defined(__cplusplus)
}
#endif

#endif // CSTL_STRING_H