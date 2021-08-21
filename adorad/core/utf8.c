/*
          _____   ____  _____            _____
    /\   |  __ \ / __ \|  __ \     /\   |  __ \
   /  \  | |  | | |  | | |__) |   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /   / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \  / ____ \| |__| | https://github.com/adorad/AdoradInternalTests/
/_/    \_\_____/ \____/|_|  \_\/_/    \_\_____/

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <@jasmcaus>
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <adorad/core/utf8.h>

/*
    The following macros define the last codepoint part of a UTF-8 category
*/
// Basic Latin (U+0000 - U+007F)
#define UTF8_MAX_BASIC_LATIN	0x007F
// Latin-1 Supplement (U+0080 - U+00FF)
#define UTF8_MAX_LATIN_1		0x00FF
// Basic Multilingual Plane (BMP)
#define UTF8_MAX_BMP			0xFFFF
// Last legal character in Unicode
#define UTF8_MAX_UNICODE		0x10FFFF

/*
    Replacement character
*/
// The codepoint used to replace illegal codepoints
#define UTF8_REPLACEMENT_CHAR	0xFFFD
// Replacement character as a UTF8-encoded string
#define UTF8_REPLACEMENT_STRING	"\xEF\xBF\xBD"
// Length of the replacement character
#define UTF8_REPLACEMENT_STRLEN	3

/*
    Surrogates
*/
// The minimum codepoint for the high member of the surrogate pair
#define UTF8_SURROGATE_HIGH_START	0xD800
// The maximum codepoint for the high member of the surrogate pair
#define UTF8_SURROGATE_HIGH_END		0xDBFF
// The minimum codepoint for the low member of the surrogate pair
#define UTF8_SURROGATE_LOW_START	0xDC00
// The maximum codepoint for the low member of the surrogate pair
#define UTF8_SURROGATE_LOW_END		0xDFFF

/*
    Hangul Jamo
*/
// The first & last codepoint part of the HJ block
#define UTF8_HANGUL_JAMO_FIRST      0x1100
#define UTF8_HANGUL_JAMO_LAST       0x11FF
// The first & last codepoint part of the HJ L section used for normalization
#define UTF8_HANGUL_L_FIRST         0x1100
#define UTF8_HANGUL_L_LAST          0x1112
// The first & last codepoint part of the HJ V section used for normalization
#define UTF8_HANGUL_V_FIRST         0x1161
#define UTF8_HANGUL_V_LAST          0x1175
// The first & last codepoint part of the HJ T section used for normalization
#define UTF8_HANGUL_T_FIRST         0x11A7
#define UTF8_HANGUL_T_LAST          0x11C2
// The first & last codepoint in the Hangul Syllables block
#define UTF8_HANGUL_S_FIRST         0xAC00
#define UTF8_HANGUL_S_LAST          0xD7A3

// The number of codepoints in the HJ ... sections (where `...` is either L,V,T,N,S) 
#define UTF8_HANGUL_LCOUNT         19
#define UTF8_HANGUL_VCOUNT         21
#define UTF8_HANGUL_TCOUNT         28
#define UTF8_HANGUL_NCOUNT         588
#define UTF8_HANGUL_SCOUNT         11172


#define UTF8_CP_LATIN_CAPITAL_LETTER_I                 0x0049
#define UTF8_CP_LATIN_CAPITAL_LETTER_J                 0x004A
#define UTF8_CP_LATIN_SMALL_LETTER_I                   0x0069
#define UTF8_CP_LATIN_SMALL_LETTER_J                   0x006A
#define UTF8_CP_LATIN_CAPITAL_LETTER_I_WITH_GRAVE      0x00CC
#define UTF8_CP_LATIN_CAPITAL_LETTER_I_WITH_ACUTE      0x00CD
#define UTF8_CP_LATIN_CAPITAL_LETTER_I_WITH_TILDE      0x0128
#define UTF8_CP_LATIN_CAPITAL_LETTER_I_WITH_OGONEK     0x012E
#define UTF8_CP_LATIN_SMALL_LETTER_I_WITH_OGONEK       0x012F
#define UTF8_CP_LATIN_CAPITAL_LETTER_I_WITH_DOT_ABOVE  0x0130
#define UTF8_CP_LATIN_SMALL_LETTER_DOTLESS_I           0x0131
#define UTF8_CP_COMBINING_GRAVE_ACCENT                 0x0300
#define UTF8_CP_COMBINING_ACUTE_ACCENT                 0x0301
#define UTF8_CP_COMBINING_TILDE_ACCENT                 0x0303
#define UTF8_CP_COMBINING_DOT_ABOVE                    0x0307
#define UTF8_CP_COMBINING_GREEK_YPOGEGRAMMENI          0x0345
#define UTF8_CP_COMBINING_GRAPHEME_JOINER              0x034F
#define UTF8_CP_GREEK_CAPITAL_LETTER_SIGMA             0x03A3

#ifndef UTF8_UINT16_MAX
    #define UTF8_UINT16_MAX     65535U
#endif // UTF8_UINT16_MAX

#include <adorad/core/utf8_data.h>
// #include <adorad/core/utf8_properties.h>

const Rune codepoint_decoded_length[256] = {
    // Basic Latin
    1, 1, 1, 1, 1, 1, 1, 1, // 0x00 - 0x07
    1, 1, 1, 1, 1, 1, 1, 1, // 0x08 - 0x0F
    1, 1, 1, 1, 1, 1, 1, 1, // 0x10 - 0x17
    1, 1, 1, 1, 1, 1, 1, 1, // 0x18 - 0x1F
    1, 1, 1, 1, 1, 1, 1, 1, // 0x20 - 0x27
    1, 1, 1, 1, 1, 1, 1, 1, // 0x28 - 0x2F
    1, 1, 1, 1, 1, 1, 1, 1, // 0x30 - 0x37
    1, 1, 1, 1, 1, 1, 1, 1, // 0x38 - 0x3F
    1, 1, 1, 1, 1, 1, 1, 1, // 0x40 - 0x47
    1, 1, 1, 1, 1, 1, 1, 1, // 0x48 - 0x4F
    1, 1, 1, 1, 1, 1, 1, 1, // 0x50 - 0x57
    1, 1, 1, 1, 1, 1, 1, 1, // 0x58 - 0x5F
    1, 1, 1, 1, 1, 1, 1, 1, // 0x60 - 0x67
    1, 1, 1, 1, 1, 1, 1, 1, // 0x68 - 0x6F
    1, 1, 1, 1, 1, 1, 1, 1, // 0x70 - 0x77
    1, 1, 1, 1, 1, 1, 1, 1, // 0x78 - 0x7F

    // Malformed continuation byte
    0, 0, 0, 0, 0, 0, 0, 0, // 0x80 - 0x87
    0, 0, 0, 0, 0, 0, 0, 0, // 0x88 - 0x8F
    0, 0, 0, 0, 0, 0, 0, 0, // 0x90 - 0x97
    0, 0, 0, 0, 0, 0, 0, 0, // 0x98 - 0x9F
    0, 0, 0, 0, 0, 0, 0, 0, // 0xA0 - 0xA7
    0, 0, 0, 0, 0, 0, 0, 0, // 0xA8 - 0xAF
    0, 0, 0, 0, 0, 0, 0, 0, // 0xB0 - 0xB7
    0, 0, 0, 0, 0, 0, 0, 0, // 0xB8 - 0xBF

    // Two bytes
    2, 2, 2, 2, 2, 2, 2, 2, // 0xC0 - 0xC7
    2, 2, 2, 2, 2, 2, 2, 2, // 0xC8 - 0xCF
    2, 2, 2, 2, 2, 2, 2, 2, // 0xD0 - 0xD7
    2, 2, 2, 2, 2, 2, 2, 2, // 0xD8 - 0xDF

    // Three bytes
    3, 3, 3, 3, 3, 3, 3, 3, // 0xE0 - 0xE7
    3, 3, 3, 3, 3, 3, 3, 3, // 0xE8 - 0xEF

    // Four bytes
    4, 4, 4, 4, 4, 4, 4, 4, // 0xF0 - 0xF7

    // Five bytes
    5, 5, 5, 5,             // 0xF8 - 0xFB

    // Six bytes
    6, 6,                   // 0xFC - 0xFD

    // Invalid
    7, 7                    // 0xFE - 0xFF
};

const Byte utf8class[256] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0 
};

static inline bool utf8_is_codepoint_valid(Rune uc) {
    // if(uc < 0 || uc >= 0x110000 || ((uc & 0xFFFF) >= 0xFFFE) || (uc >= 0xD800 && uc < 0xE000) || 
    //   (uc >= 0xFDD0 && uc < 0xFDF0))
    //     return false;
    // return true;
    return ((cast(Rune)uc) - 0xd800 > 0x07ff) && (cast(Rune)uc < 0x110000);
}

// Determine the number of bytes needed to store the UTF-8 character
// This can be one in <1, 2, 3, 4>
// Theoretically, this number can extend to 6, and even 7, bytes, but this is rare
static inline Ll utf8_encode_nbytes(Rune value) {
    Ll nbytes = 0;
    CORETEN_ENFORCE(value > 0, "Cannot encode a negative value :(");

    if(value <= 0x7f) nbytes = 1;     // 127
    if(value <= 0x7ff) nbytes = 2;    // 2047
    if(value <= 0xffff) nbytes = 3;   // 65535
    if(value <= 0x10ffff) nbytes = 4; // 1114111
    else nbytes = 0;
    CORETEN_ENFORCE(nbytes > 0, "Invalid code point");
    return nbytes;
}

// Determine the number of bytes used by the UTF-8 character
// This can be one in <1, 2, 3, 4>
// Theoretically, this number can extend to 6, and even 7, bytes, but this is rare
static inline Ll utf8_decode_nbytes(Rune byte) {
    CORETEN_ENFORCE(byte > 0, "Cannot decode  a negative value :(");
    Ll nbytes;

    // If the byte starts with 10xxxxx, it's the middle of a UTF-8 sequence, so don't count it at all.
    if((byte & 0xc0) == 0x80) nbytes = 0;
    
    // The first byte's high bits tell us how many bytes are in the UTF-8 sequence.
    else if((byte & 0xf8) == 0xf0) nbytes = 4;
    else if((byte & 0xf0) == 0xe0) nbytes = 3;
    else if((byte & 0xe0) == 0xc0) nbytes = 2;
    else nbytes = 0;
    CORETEN_ENFORCE(nbytes > 0, "Invalid code point");
    return nbytes;
}

static inline char* utf8_encode(Rune value) {
    Byte mask = 0x3f; // 63
    char* dst = cast(char*)calloc(4, sizeof(char));

    if(value <= (value << 7) - 1) {
        dst[0] = cast(char)value;
        return dst;
    } else if(value <= (value << 11) - 1) {
        dst[0] = (0xc0) | (cast(char)(value >> 6));
        dst[1] = (0x80) | (cast(char)(value) & mask);
        return dst;
    } 
    // Invalid/Surrogate range
    if(value > CORETEN_RUNE_MAX || CORETEN_IS_BETWEEN(value, 0xd800, 0xdff)) {
        value = CORETEN_RUNE_INVALID;
        dst[0] = (0xe0) | (cast(char)(value >> 12));
        dst[1] = (0x80) | (cast(char)(value >> 12) & mask);
        dst[2] = (0x80) | (cast(char)(value) & mask);
        return dst;
    } else if(value <= (value << 16) - 1) {
        dst[0] = (0xe0) | (cast(char)(value >> 12));
        dst[1] = (0x80) | (cast(char)(value >> 6) & mask);
        dst[2] = (0x80) | (cast(char)(value) & mask);
        return dst;
    }

    dst[0] = (0xf0) | (cast(char)(value >> 18));
    dst[1] = (0x80) | (cast(char)(value >> 12) & mask);
    dst[2] = (0x80) | (cast(char)(value >> 6) & mask);
    dst[3] = (0x80) | (cast(char)(value) & mask);

    return dst;
}

/*
    WIP
*/

static cstlUTF8Str* ubuff_new(Rune* data) {
    cstlUTF8Str* ubuff = cast(cstlUTF8Str*)calloc(1, sizeof(ubuff));
    CORETEN_ENFORCE_NN(ubuff, "Could not allocate memory. Memory full.");

    // ubuff_set(ubuff, data);
    return ubuff;
}

void __grow_ubuff(cstlUTF8Str* ubuff, int grow_by) {
    CORETEN_ENFORCE_NN(ubuff, "Expected not null");
    CORETEN_ENFORCE(grow_by > 0);

    ubuff->data = cast(Byte*)realloc(ubuff->data, grow_by);
    memset(ubuff->data + ubuff->nbytes, 0, grow_by);
    ubuff->nbytes += grow_by;
}

void ubuff_push_char(cstlUTF8Str* ubuff, Rune ch) {
    CORETEN_ENFORCE(ubuff->data[ubuff->len] == 0); // ensure this is writeable (we shouldn't overwrite over a char)

    // 0b0xxxxxxx
    if(ch <= 0x7f) {
        __push_ascii_char(ubuff, ch);
    }
    // 110xxxxx 10xxxxxx
    else if(ch >= 0xC080 && ch <= 0xDFBF) {
        // 2-byte character
        __grow_ubuff(ubuff, 2);
        UInt64 nbytes = ubuff->nbytes;
        ubuff->data[nbytes    ] = cast(Byte)(ch >> 8);
        ubuff->data[nbytes + 1] = cast(Byte)(ch);
        ubuff->data[nbytes + 2] = 0;
        ubuff->nbytes += 2;
        ubuff->len += 1;
    }
    // 1110xxxx 10xxxxxx 10xxxxxx
    else if(ch >= 0xE08080 && ch <= 0xEFBFBF) {
        // 3-byte character
        __grow_ubuff(ubuff, 3);
        UInt64 nbytes = ubuff->nbytes;
        ubuff->data[nbytes    ] = cast(Byte)(ch >> 16);
        ubuff->data[nbytes + 1] = cast(Byte)(ch >> 8);
        ubuff->data[nbytes + 2] = cast(Byte)(ch);
        ubuff->data[nbytes + 3] = 0;
        ubuff->nbytes += 3;
        ubuff->len += 1;
    }
    // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    else if(ch >= 0xF0808080 && ch <= 0xF7BFBFBF) {
        // 3-byte character
        __grow_ubuff(ubuff, 3);
        UInt64 nbytes = ubuff->nbytes;
        ubuff->data[nbytes    ] = cast(Byte)(ch >> 24);
        ubuff->data[nbytes + 1] = cast(Byte)(ch >> 16);
        ubuff->data[nbytes + 2] = cast(Byte)(ch >> 8);
        ubuff->data[nbytes + 3] = cast(Byte)(ch);
        ubuff->data[nbytes + 4] = 0;
        ubuff->nbytes += 4;
        ubuff->len += 1;
    }
    else {
        // TODO (jasmcaus): `panic()` here
        fprintf(stderr, "Invalid UTF-8 character: %hx", ch);
        exit(1);
    }
}

void __push_byte(cstlUTF8Str* ubuff, Byte byte) {
    CORETEN_ENFORCE(byte <= 0x7F);
    __grow_ubuff(ubuff, 1);
    ubuff->data[ubuff->nbytes++] = byte;
    ubuff->data[ubuff->nbytes] = 0;
}

void __push_ascii_char(cstlUTF8Str* ubuff, Byte byte) {
    __push_byte(ubuff, byte);
    ubuff->len += 1;
}

// Returns the number of UTF8 characters in the buffer
static UInt64 ubuff_len(cstlUTF8Str* ubuff) {
    return ubuff->len;
}

// Returns the number of bytes used by the UTF8 buffer.
static UInt64 ubuff_nbytes(cstlUTF8Str* ubuff) {
    return ubuff->nbytes;
}

/*
#define WRITE_OUT(val)	if(out)	*out = (val)
// Return the byte at the `n`th byte offset
// If `out` is null, we don't intend on knowing how many bytes the UTF8 character uses.
static int __byte_offset_at(cstlUTF8Str* ubuff, Int64 n, int* out) {
    CORETEN_ENFORCE_NN(ubuff, "Expected not null");
    CORETEN_ENFORCE_NN(ubuff->data, "Expected not null");
    // Fail-fast approach. If this changes, comment this, and uncomment the following (commented) snippet
    CORETEN_ENFORCE(n < ubuff->nbytes);
    CORETEN_ENFORCE(n > 0);
    // if((n >= ubuff->nbytes) || (n < 0)) {
    // 	return nullchar;
    // }

    Byte byte = cast(Byte)(ubuff->data[n]);
    if(byte == 0) {
        return cast(Byte)0;
    } 
    // 1-byte UTF8
    else if(byte <= 0x07F) {
        WRITE_OUT(1);
        return byte;
    } 
    // 2-byte UF8
    else if(byte >= 0x80 && byte <= 0x07FF) {
        WRITE_OUT(2);
        byte = (byte << 8) + cast(Byte)(ubuff->data[n + 1]);
        return byte;
    } 
    // 3-byte UTF8
    else if(byte >= 0x0800 && byte <= 0xFFFF) {
        WRITE_OUT(3);
        byte = byte << 16;
        byte = (byte) | (cast(Byte)(ubuff->data[n + 1]) << 8);
        byte = (byte) | (cast(Byte)(ubuff->data[n + 2]) << 16);
        return byte;
    }
    // 4-byte UTF8
    else if(byte >= 0x00010000 && byte <= 0x0010FFFF) {
        WRITE_OUT(4);
        byte = byte << 24;
        byte = (byte) | (cast(Byte)(ubuff->data[n + 1]) << 16);
        byte = (byte) | (cast(Byte)(ubuff->data[n + 2]) << 8);
        byte = (byte) | (cast(Byte)(ubuff->data[n + 3]));
        return byte;
    } else {
        // TODO (jasmcaus): `panic()` here
        fprintf(stderr, "Cannot decode UTF8 byte `%hx` at byte offset %d", byte, n);
        exit(1);
    }
}

// Return the byte at the `n`th byte offset
static Byte ubuff_byte_offset_at(cstlUTF8Str* ubuff, Int64 n) {
    return __byte_offset_at(ubuff, n, null);
}


// Returns the byte at `n`th character offset
static Byte ubuff_at(cstlUTF8Str* ubuff, Int64 n) {
    CORETEN_ENFORCE_NN(ubuff, "Expected not null");
    CORETEN_ENFORCE_NN(ubuff->data, "Expected not null");
    CORETEN_ENFORCE(n < ubuff->len);
    CORETEN_ENFORCE(n > 0);
    int a[1];

    UInt64 byte_offset = 0;
    while(n != 0) {
        Byte b = __byte_offset_at(ubuff, byte_offset, a); // we only care about `a`
        byte_offset += cast(UInt64)a;
        --n;
    }
    return ubuff_byte_offset_at(ubuff, byte_offset);
}
*/
