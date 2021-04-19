#ifndef CSTL_STRING_H
#define CSTL_STRING_H

#include <hazel/core/misc.h>
#include <hazel/core/types.h>

#ifdef __cplusplus
extern "C" {
#endif

// Char Things ==========================================
CSTL_DEF inline char toLower        (char c);
CSTL_DEF inline char toUpper        (char c);
CSTL_DEF inline bool isWhitespace   (char c);
CSTL_DEF inline bool isLower        (char c);
CSTL_DEF inline bool isUpper        (char c);
CSTL_DEF inline bool isLetter       (char c);
CSTL_DEF inline bool isDigit        (char c);
CSTL_DEF inline bool isHexDigit     (char c);
CSTL_DEF inline bool isAlpha        (char c);
CSTL_DEF inline bool isAlphanumeric (char c);
CSTL_DEF inline Int32  digitToInt        (char c);
CSTL_DEF inline Int32  hexDigitToInt    (char c);

// ASCII only
CSTL_DEF inline void strToLower(char* str);
CSTL_DEF inline void strToUpper(char* str);


// // NOTE(jasmcaus): A less fucking crazy strtok!
// CSTL_DEF char const* gb_strtok(char* output, char const* src, char const* delimit);

// CSTL_DEF bool gb_str_has_prefix(char const* str, char const* prefix);
// CSTL_DEF bool gb_str_has_suffix(char const* str, char const* suffix);

// CSTL_DEF char const* gb_char_first_occurence(char const* str, char c);
// CSTL_DEF char const* gb_char_last_occurence (char const* str, char c);

// CSTL_DEF void gb_str_concat(char* dest, Ll dest_len,
//                           char const* src_a, Ll src_a_len,
//                           char const* src_b, Ll src_b_len);



// ////////////////////////////////////////////////////////////////
// // UTF-8 Handling
// // Does not check if utf-8 string is valid
// CSTL_DEF Ll gb_utf8_strlen (UInt8 const* str);
// CSTL_DEF Ll gb_utf8_strnlen(UInt8 const* str, Ll max_len);

// // NOTE(jasmcaus): Windows doesn't handle 8 bit filenames well ('cause Micro$hit)
// CSTL_DEF UInt16 *gb_utf8_to_ucs2    (UInt16 *buffer, Ll len, UInt8 const* str);
// CSTL_DEF UInt8 * gb_ucs2_to_utf8    (UInt8 *buffer, Ll len, UInt16 const* str);
// CSTL_DEF UInt16 *gb_utf8_to_ucs2_buf(UInt8 const* str);   // NOTE(jasmcaus): Uses locally persisting buffer
// CSTL_DEF UInt8 * gb_ucs2_to_utf8_buf(UInt16 const* str); // NOTE(jasmcaus): Uses locally persisting buffer

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
    if(c == ' '  ||
       c == '\t' ||
       c == '\n' ||
       c == '\r' ||
       c == '\f' ||
       c == '\v')
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

#if defined(__cplusplus)
}
#endif

#endif // CSTL_STRING_H