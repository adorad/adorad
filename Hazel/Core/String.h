/*
_ _    _           ______   _______        
| |  | |    /\    /___  /   |  ____|| |    
| |__| |   /  \      / /    | |__   | |       Hazel - The Fast, Expressive & Elegant Programming Language
|  __  |  / /\ \    / /     |  __|  | |       Languages: C, C++, and Assembly
| |  | | / ____ \  / /___   | |____ | |____   https://github.com/HazelLang/Hazel/
|_|_ |_|/_/    \_\/_______\ |______|_\______|

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/

#ifndef CSTL_STRING_H
#define CSTL_STRING_H

#include <Hazel/Core/Memory.h>
#include <Hazel/Core/Misc.h>
#include <Hazel/Core/Types.h>


namespace Hazel {

// Char Things ==========================================
static inline bool isUpper(char c) { return c>='A' && c<='Z'; }
static inline bool isLower(char c) { return c>='a' && c<='z'; }
static inline bool isDigit(char c) { return c >= '0' && c <= '9'; }
static inline bool isAlpha(char c) { return isUpper(c) || isLower(c); }
static inline bool isAlphanumeric(char c) { return isAlpha(c) || isDigit(c); }

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

static inline bool isLetter(char c) {
    return  (c >= 'a' && c <= 'z') || 
            (c >= 'A' && c <= 'Z') || 
            (c == '_') ;
}

static inline bool isHexDigit(char c) {
    return  isDigit(c)                   ||
            CSTL_IS_BETWEEN(c, 'a', 'f') ||
            CSTL_IS_BETWEEN(c, 'A', 'F'); 
}

static inline Int32 digitToInt(char c) { return isDigit(c) ? c-'0' : c-'W'; }

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

// namespace detail {
//     std::string stripBaseName(const std::string& full_path) {
//         const char kSeparator = '/';
//         size_t pos = full_path.rfind(kSeparator);

//         if (pos != std::string::npos) {
//             return full_path.substr(pos + 1, std::string::npos);
//         } else {
//             return full_path;
//         }
//     }

//     struct CompileTimeEmptyString {
//         operator const std::string&() const {
//             static const std::string empty_string_literal; 
//             return empty_string_literal;
//         }

//         operator const char*() const {
//             return "";
//         }
//     }; // struct CompileTimeEmptyString

//     template <typename T>
//     struct CanonicalizeStrTypes {
//         using type = const T&;
//     };

//     template <size_t N>
//     struct CanonicalizeStrTypes<char[N]> {
//         using type = const char*;
//     };


//     inline std::ostream& _str(std::ostream& ss) {
//         return ss;
//     }

//     template <typename T>
//     inline std::ostream& _str(std::ostream& ss, const T& t) {
//         ss << t;
//         return ss;
//     }

//     template <>
//     inline std::ostream& _str<CompileTimeEmptyString>(std::ostream& ss, const CompileTimeEmptyString&) {
//         return ss;
//     }

//     template <typename T, typename... Args>
//     inline std::ostream& _str(std::ostream& ss, const T& t, const Args&... args) {
//         return _str(_str(ss, t), args...);
//     }

//     template<typename... Args>
//     struct _str_wrapper final {
//     static std::string call(const Args&... args) {
//         std::ostringstream ss;
//         _str(ss, args...);
//         return ss.str();
//     }
//     };

//     // Specializations for already-a-string types.
//     template<>
//     struct _str_wrapper<std::string> final {
//         // return by reference to avoid the binary size of a string copy
//         static const std::string& call(const std::string& str) {
//             return str;
//         }
//     };

//     template<>
//     struct _str_wrapper<const char*> final {
//         static const char* call(const char* str) {
//             return str;
//         }
//     };

//     // For cstl::str() with an empty argument list (which is common in our assert macros), we don't 
//     // want to pay the binary size for constructing and destructing a stringstream or even constructing a string.
//     template<>
//     struct _str_wrapper<> final {
//         static CompileTimeEmptyString call() {
//             return CompileTimeEmptyString();
//         }
//     };

// } // namespace detail

// // Convert a list of string-like arguments into a single string. 
// template<typename... Args>
// inline decltype(auto) str(const Args&... args) {
//     return detail::_str_wrapper<typename 
//         detail::CanonicalizeStrTypes<Args>::type...>::call(args...);
// }

// /// Represents a location in source code (for debugging).
// struct SourceLocation {
//     const char* function;
//     const char* file;
//     UInt32 line;
// };


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

// CSTL_DEF char* strCat(char* dest, char* source);
// CSTL_DEF Ll    strLen(const char* str);
// CSTL_DEF Ll    strnLen(const char* str, Ll max_len);
// CSTL_DEF Int32 strCmp(const char* str1, const char* str2);
// CSTL_DEF Int32 strnCmp(const char* str1, const char* str2, Ll len);
// CSTL_DEF char* strCopy(char *dest, const char* source);
// CSTL_DEF char* strnCopy(char *dest, const char* source, Ll len);
// CSTL_DEF Ll    strlCopy(char *dest, const char* source, Ll len);
// CSTL_DEF char* strRev(char *str); //  ASCII only

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


} // namespace Hazel

#endif // CSTL_STRING_H