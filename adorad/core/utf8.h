/*
          _____   ____  _____            _____
    /\   |  __ \ / __ \|  __ \     /\   |  __ \
   /  \  | |  | | |  | | |__) |   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /   / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \  / ____ \| |__| | https://github.com/adorad/AdoradInternalTests/
/_/    \_\_____/ \____/|_|  \_\/_/    \_\_____/

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021-22 Jason Dsouza <@jasmcaus>
*/

#ifndef CORETEN_UTF8_H
#define CORETEN_UTF8_H

/*
    UTF-8 refresher:

    UTF-8 encodes text in sequences of "code points", each one from 1-4 bytes. For each code point 
    that is longer than one byte, the code point begins with a unique prefix that specifies how many bytes
    follow. All bytes in the code point after this first have a continuationmarker. All code points in UTF-8 
    will thus look like one of the following binary sequences, with x meaning "don't care":
        1 byte:  0xxxxxxx
        2 bytes: 110xxxxx  10xxxxxx
        3 bytes: 1110xxxx  10xxxxxx  10xxxxxx
        4 bytes: 11110xxx  10xxxxxx  10xxxxxx  10xxxxxx
    
    UTF8-octets =   *(UTF8-char)
    UTF8-char   =   UTF8-1 / UTF8-2 / UTF8-3 / UTF8-4
    UTF8-1      =   %x00-7F
    UTF8-2      =   %xC2-DF UTF8-tail
    UTF8-3      =   %xE0 %xA0-BF UTF8-tail / %xE1-EC 2( UTF8-tail ) /
                    %xED %x80-9F UTF8-tail / %xEE-EF 2( UTF8-tail )
    UTF8-4      =   %xF0 %x90-BF 2( UTF8-tail ) / %xF1-F3 3( UTF8-tail ) /
                    %xF4 %x80-8F 2( UTF8-tail )
    UTF8-tail   =   %x80-BF

    Further reading: https://stackoverflow.com/questions/2241348/what-is-unicode-utf-8-utf-16

    Useful UTF8 Libraries (in C):
        1. https://github.com/ZHaskell/utf8rewind
        2. https://github.com/osa1/utf8
        3. https://github.com/JuliaStrings/utf8proc
        4. https://github.com/lemire/fastvalidate-utf-8
*/

#include <adorad/core/types.h>

#define uBuff  cstlUTF8Str
typedef struct cstlUTF8Str {
    Byte* data;    // actual UTF8 data
    UInt64 len;    // no. of UTF8 characters
    UInt64 nbytes; // no. of bytes used by the string
} cstlUTF8Str;

// Is UTF-8 codepoint valid?
bool utf8_is_codepoint_valid(Rune uc);
char* utf8_encode(Rune value);
Ll utf8_encode_nbytes(Rune value);
Ll utf8_decode_nbytes(Rune byte);

/*
    WIP
*/
cstlUTF8Str* ubuff_new(Rune* data);
cstlUTF8Str* ubuff_set(cstlUTF8Str* ubuff, Rune* data);
void __grow_ubuff(cstlUTF8Str* ubuff, int grow_by);
void __push_byte(cstlUTF8Str* ubuff, Byte byte);
void __push_ascii_char(cstlUTF8Str* ubuff, Byte byte);
void ubuff_push_char(cstlUTF8Str* ubuff, Rune ch);
UInt64 ubuff_len(cstlUTF8Str* ubuff);
UInt64 ubuff_nbytes(cstlUTF8Str* ubuff);
Byte ubuff_byte_offset_at(cstlUTF8Str* ubuff, Int64 n);
Byte ubuff_at(cstlUTF8Str* ubuff, Int64 n);

// Unicode categories
typedef enum {
    UTF8_CATEGORY_CN  = 0, // < Other, not assigned
    UTF8_CATEGORY_LU  = 1, // < Letter, uppercase
    UTF8_CATEGORY_LL  = 2, // < Letter, lowercase
    UTF8_CATEGORY_LT  = 3, // < Letter, titlecase
    UTF8_CATEGORY_LM  = 4, // < Letter, modifier
    UTF8_CATEGORY_LO  = 5, // < Letter, other
    UTF8_CATEGORY_MN  = 6, // < Mark, nonspacing
    UTF8_CATEGORY_MC  = 7, // < Mark, spacing combining
    UTF8_CATEGORY_ME  = 8, // < Mark, enclosing
    UTF8_CATEGORY_ND  = 9, // < Number, decimal digit
    UTF8_CATEGORY_NL = 10, // < Number, letter
    UTF8_CATEGORY_NO = 11, // < Number, other
    UTF8_CATEGORY_PC = 12, // < Punctuation, connector
    UTF8_CATEGORY_PD = 13, // < Punctuation, dash
    UTF8_CATEGORY_PS = 14, // < Punctuation, open
    UTF8_CATEGORY_PE = 15, // < Punctuation, close
    UTF8_CATEGORY_PI = 16, // < Punctuation, initial quote
    UTF8_CATEGORY_PF = 17, // < Punctuation, final quote
    UTF8_CATEGORY_PO = 18, // < Punctuation, other
    UTF8_CATEGORY_SM = 19, // < Symbol, math
    UTF8_CATEGORY_SC = 20, // < Symbol, currency
    UTF8_CATEGORY_SK = 21, // < Symbol, modifier
    UTF8_CATEGORY_SO = 22, // < Symbol, other
    UTF8_CATEGORY_ZS = 23, // < Separator, space
    UTF8_CATEGORY_ZL = 24, // < Separator, line
    UTF8_CATEGORY_ZP = 25, // < Separator, paragraph
    UTF8_CATEGORY_CC = 26, // < Other, control
    UTF8_CATEGORY_CF = 27, // < Other, format
    UTF8_CATEGORY_CS = 28, // < Other, surrogate
    UTF8_CATEGORY_CO = 29, // < Other, private use
} cstlUTF8Category;

// Bidirectional character classes.
typedef enum {
    UTF8_BIDI_CLASS_L     = 1, // < Left-to-Right
    UTF8_BIDI_CLASS_LRE   = 2, // < Left-to-Right Embedding
    UTF8_BIDI_CLASS_LRO   = 3, // < Left-to-Right Override
    UTF8_BIDI_CLASS_R     = 4, // < Right-to-Left
    UTF8_BIDI_CLASS_AL    = 5, // < Right-to-Left Arabic
    UTF8_BIDI_CLASS_RLE   = 6, // < Right-to-Left Embedding
    UTF8_BIDI_CLASS_RLO   = 7, // < Right-to-Left Override
    UTF8_BIDI_CLASS_PDF   = 8, // < Pop Directional Format
    UTF8_BIDI_CLASS_EN    = 9, // < European Number
    UTF8_BIDI_CLASS_ES   = 10, // < European Separator
    UTF8_BIDI_CLASS_ET   = 11, // < European Number Terminator
    UTF8_BIDI_CLASS_AN   = 12, // < Arabic Number
    UTF8_BIDI_CLASS_CS   = 13, // < Common Number Separator
    UTF8_BIDI_CLASS_NSM  = 14, // < Nonspacing Mark
    UTF8_BIDI_CLASS_BN   = 15, // < Boundary Neutral
    UTF8_BIDI_CLASS_B    = 16, // < Paragraph Separator
    UTF8_BIDI_CLASS_S    = 17, // < Segment Separator
    UTF8_BIDI_CLASS_WS   = 18, // < Whitespace
    UTF8_BIDI_CLASS_ON   = 19, // < Other Neutrals
    UTF8_BIDI_CLASS_LRI  = 20, // < Left-to-Right Isolate
    UTF8_BIDI_CLASS_RLI  = 21, // < Right-to-Left Isolate
    UTF8_BIDI_CLASS_FSI  = 22, // < First Strong Isolate
    UTF8_BIDI_CLASS_PDI  = 23, // < Pop Directional Isolate
} cstlUTF8BidiClass;

// Decomposition type.
typedef enum {
    UTF8_DECOMP_TYPE_FONT      = 1, // < Font
    UTF8_DECOMP_TYPE_NOBREAK   = 2, // < Nobreak
    UTF8_DECOMP_TYPE_INITIAL   = 3, // < Initial
    UTF8_DECOMP_TYPE_MEDIAL    = 4, // < Medial
    UTF8_DECOMP_TYPE_FINAL     = 5, // < Final
    UTF8_DECOMP_TYPE_ISOLATED  = 6, // < Isolated
    UTF8_DECOMP_TYPE_CIRCLE    = 7, // < Circle
    UTF8_DECOMP_TYPE_SUPER     = 8, // < Super
    UTF8_DECOMP_TYPE_SUB       = 9, // < Sub
    UTF8_DECOMP_TYPE_VERTICAL = 10, // < Vertical
    UTF8_DECOMP_TYPE_WIDE     = 11, // < Wide
    UTF8_DECOMP_TYPE_NARROW   = 12, // < Narrow
    UTF8_DECOMP_TYPE_SMALL    = 13, // < Small
    UTF8_DECOMP_TYPE_SQUARE   = 14, // < Square
    UTF8_DECOMP_TYPE_FRACTION = 15, // < Fraction
    UTF8_DECOMP_TYPE_COMPAT   = 16, // < Compat
} cstlUTF8DecompType;

// Boundclass property. (TR29)
typedef enum {
    UTF8_BOUNDCLASS_START              =  0, // < Start
    UTF8_BOUNDCLASS_OTHER              =  1, // < Other
    UTF8_BOUNDCLASS_CR                 =  2, // < Cr
    UTF8_BOUNDCLASS_LF                 =  3, // < Lf
    UTF8_BOUNDCLASS_CONTROL            =  4, // < Control
    UTF8_BOUNDCLASS_EXTEND             =  5, // < Extend
    UTF8_BOUNDCLASS_L                  =  6, // < L
    UTF8_BOUNDCLASS_V                  =  7, // < V
    UTF8_BOUNDCLASS_T                  =  8, // < T
    UTF8_BOUNDCLASS_LV                 =  9, // < Lv
    UTF8_BOUNDCLASS_LVT                = 10, // < Lvt
    UTF8_BOUNDCLASS_REGIONAL_INDICATOR = 11, // < Regional indicator
    UTF8_BOUNDCLASS_SPACINGMARK        = 12, // < Spacingmark
    UTF8_BOUNDCLASS_PREPEND            = 13, // < Prepend
    UTF8_BOUNDCLASS_ZWJ                = 14, // < Zero Width Joiner

    // the following are no longer used in Unicode 11, but we keep the constants here for backward compatibility
    UTF8_BOUNDCLASS_E_BASE             = 15, // < Emoji Base
    UTF8_BOUNDCLASS_E_MODIFIER         = 16, // < Emoji Modifier
    UTF8_BOUNDCLASS_GLUE_AFTER_ZWJ     = 17, // < Glue_After_ZWJ
    UTF8_BOUNDCLASS_E_BASE_GAZ         = 18, // < E_BASE + GLUE_AFTER_ZJW

    // the Extended_Pictographic property is used in the Unicode 11 grapheme-boundary rules, so we store it in 
    // the boundclass field
    UTF8_BOUNDCLASS_EXTENDED_PICTOGRAPHIC = 19,
    UTF8_BOUNDCLASS_E_ZWG = 20, /* UTF8_BOUNDCLASS_EXTENDED_PICTOGRAPHIC + ZWJ */
} cstlUTF8Boundclass;


#ifdef CORETEN_IMPL
    #include <string.h>
    
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

    bool utf8_is_codepoint_valid(Rune uc) {
        // if(uc < 0 || uc >= 0x110000 || ((uc & 0xFFFF) >= 0xFFFE) || (uc >= 0xD800 && uc < 0xE000) || 
        //   (uc >= 0xFDD0 && uc < 0xFDF0))
        //     return false;
        // return true;
        return ((cast(Rune)uc) - 0xd800 > 0x07ff) && (cast(Rune)uc < 0x110000);
    }

    // Determine the number of bytes needed to store the UTF-8 character
    // This can be one in <1, 2, 3, 4>
    // Theoretically, this number can extend to 6, && even 7, bytes, but this is rare
    Ll utf8_encode_nbytes(Rune value) {
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
    // Theoretically, this number can extend to 6, && even 7, bytes, but this is rare
    Ll utf8_decode_nbytes(Rune byte) {
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

    char* utf8_encode(Rune value) {
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

    cstlUTF8Str* ubuff_new(Rune* data) {
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
            // TODO (jasmcaus): `dread()` here
            fprintf(stderr, "Invalid UTF-8 character: %x", ch);
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
    UInt64 ubuff_len(cstlUTF8Str* ubuff) {
        return ubuff->len;
    }

    // Returns the number of bytes used by the UTF8 buffer.
    UInt64 ubuff_nbytes(cstlUTF8Str* ubuff) {
        return ubuff->nbytes;
    }

    /*
    #define WRITE_OUT(val)	if(out)	*out = (val)
    // Return the byte at the `n`th byte offset
    // If `out` is null, we don't intend on knowing how many bytes the UTF8 character uses.
    static int __byte_offset_at(cstlUTF8Str* ubuff, Int64 n, int* out) {
        CORETEN_ENFORCE_NN(ubuff, "Expected not null");
        CORETEN_ENFORCE_NN(ubuff->data, "Expected not null");
        // Fail-fast approach. If this changes, comment this, && uncomment the following (commented) snippet
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
            // TODO (jasmcaus): `dread()` here
            fprintf(stderr, "Cannot decode UTF8 byte `%hx` at byte offset %d", byte, n);
            exit(1);
        }
    }

    // Return the byte at the `n`th byte offset
    Byte ubuff_byte_offset_at(cstlUTF8Str* ubuff, Int64 n) {
        return __byte_offset_at(ubuff, n, null);
    }


    // Returns the byte at `n`th character offset
    Byte ubuff_at(cstlUTF8Str* ubuff, Int64 n) {
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
   
#endif // CORETEN_IMPL


#endif // CORETEN_UTF8_H