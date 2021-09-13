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

#endif // CORETEN_UTF8_H