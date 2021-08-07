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

#ifndef CSTL_UTF8_H
#define CSTL_UTF8_H

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
	
	UTF8-octets = *( UTF8-char )
    UTF8-char   = UTF8-1 / UTF8-2 / UTF8-3 / UTF8-4
    UTF8-1      = %x00-7F
    UTF8-2      = %xC2-DF UTF8-tail
    UTF8-3      = %xE0 %xA0-BF UTF8-tail / %xE1-EC 2( UTF8-tail ) /
                  %xED %x80-9F UTF8-tail / %xEE-EF 2( UTF8-tail )
    UTF8-4      = %xF0 %x90-BF 2( UTF8-tail ) / %xF1-F3 3( UTF8-tail ) /
                  %xF4 %x80-8F 2( UTF8-tail )
    UTF8-tail   = %x80-BF

	Further reading: https://stackoverflow.com/questions/2241348/what-is-unicode-utf-8-utf-16
*/

#include <stddef.h>
#include <stdlib.h>
#include <adorad/core/types.h>
#include <adorad/core/misc.h>
#include <adorad/core/debug.h>

#define uBuff	cstlUTF8Str

typedef struct cstlUTF8Str {
	Byte* data;    // actual UTF8 data
	UInt64 len;    // no. of UTF8 characters
	UInt64 nbytes; // no. of bytes used by the string
} cstlUTF8Str;

static cstlUTF8Str* ubuff_new(Rune* data);
static cstlUTF8Str* ubuff_set(cstlUTF8Str* ubuff, Rune* data);
void __grow_ubuff(cstlUTF8Str* ubuff, int grow_by);
void __push_byte(cstlUTF8Str* ubuff, Byte byte);
void __push_ascii_char(cstlUTF8Str* ubuff, Byte byte);
void ubuff_push_char(cstlUTF8Str* ubuff, Rune ch);
static UInt64 ubuff_len(cstlUTF8Str* ubuff);
static UInt64 ubuff_nbytes(cstlUTF8Str* ubuff);
static Byte ubuff_byte_offset_at(cstlUTF8Str* ubuff, Int64 n);
static Byte ubuff_at(cstlUTF8Str* ubuff, Int64 n);

static cstlUTF8Str* ubuff_new(Rune* data) {
	cstlUTF8Str* ubuff = cast(cstlUTF8Str*)calloc(1, sizeof(ubuff));
    CSTL_CHECK_NOT_NULL(ubuff, "Could not allocate memory. Memory full.");

	// ubuff_set(ubuff, data);

	return ubuff;
}

/*
static cstlUTF8Str* ubuff_set(cstlUTF8Str* ubuff, Rune* data) {
	CSTL_CHECK_NOT_NULL(ubuff, "Expected not null");

    UInt64 len;
    if(!data) {
        len = 0;
        data = 0;
    } else {
        len = (UInt64)__internal_strlength(data);
    }

    buffer->data = data;
    buffer->len = len;
}
*/

void __grow_ubuff(cstlUTF8Str* ubuff, int grow_by) {
	CSTL_CHECK_NOT_NULL(ubuff, "Expected not null");
	CSTL_CHECK_GT(grow_by, 0);

	ubuff->data = cast(Byte*)realloc(ubuff->data, grow_by);
	memset(ubuff->data + ubuff->nbytes, 0, grow_by);
	ubuff->nbytes += grow_by;
}

void ubuff_push_char(cstlUTF8Str* ubuff, Rune ch) {
	CSTL_CHECK_EQ(ubuff->data[ubuff->len], 0); // ensure this is writeable (we shouldn't overwrite over a char)

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
	CSTL_CHECK_LE(byte, 0x7F);
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

#define WRITE_OUT(val)	if(out)	*out = (val)
// Return the byte at the `n`th byte offset
// If `out` is null, we don't intend on knowing how many bytes the UTF8 character uses.
static int __byte_offset_at(cstlUTF8Str* ubuff, Int64 n, int* out) {
	CSTL_CHECK_NOT_NULL(ubuff, "Expected not null");
    CSTL_CHECK_NOT_NULL(ubuff->data, "Expected not null");
	// Fail-fast approach. If this changes, comment this, and uncomment the following (commented) snippet
	CSTL_CHECK_LT(n, ubuff->nbytes);
	CSTL_CHECK_GT(n, 0);
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
	CSTL_CHECK_NOT_NULL(ubuff, "Expected not null");
    CSTL_CHECK_NOT_NULL(ubuff->data, "Expected not null");
	CSTL_CHECK_LT(n, ubuff->len);
	CSTL_CHECK_GT(n, 0);
	int a[1];

	UInt64 byte_offset = 0;
	while(n != 0) {
		Byte b = __byte_offset_at(ubuff, byte_offset, a); // we only care about `a`
		byte_offset += cast(UInt64)a;
		--n;
	}
	return ubuff_byte_offset_at(ubuff, byte_offset);
}

//
// ----------------------------------------------------------------
//

// Is UTF-8 codepoint valid?
static inline bool utf8_is_codepoint_valid(Rune uc);
static inline char* utf8_encode(Rune value);
static inline Ll utf8_encode_nbytes(Rune value);
static inline Ll utf8_decode_nbytes(Rune byte);
static inline char* utf8_encode(Rune value);

static inline bool utf8_is_codepoint_valid(Rune uc) {
	if(uc < 0 || uc >= 0x110000 || ((uc & 0xFFFF) >= 0xFFFE) || (uc >= 0xD800 && uc < 0xE000) || 
	  (uc >= 0xFDD0 && uc < 0xFDF0))
		return false;
	return true;
}

// Determine the number of bytes needed to store the UTF-8 character
// This can be one in <1, 2, 3, 4>
// Theoretically, this number can extend to 6, and even 7, bytes, but this is rare
static inline Ll utf8_encode_nbytes(Rune value) {
	Ll nbytes = 0;
	CSTL_CHECK(value > 0, "Cannot encode a negative value :(");

	if(value <= 0x7f) nbytes = 1;     // 127
	if(value <= 0x7ff) nbytes = 2;    // 2047
	if(value <= 0xffff) nbytes = 3;   // 65535
	if(value <= 0x10ffff) nbytes = 4; // 1114111
	else nbytes = 0;
	CSTL_CHECK(nbytes > 0, "Invalid code point");
	return nbytes;
}

// Determine the number of bytes used by the UTF-8 character
// This can be one in <1, 2, 3, 4>
// Theoretically, this number can extend to 6, and even 7, bytes, but this is rare
static inline Ll utf8_decode_nbytes(Rune byte) {
	CSTL_CHECK(byte > 0, "Cannot decode  a negative value :(");
	Ll nbytes;

	// If the byte starts with 10xxxxx, it's the middle of a UTF-8 sequence, so don't count it at all.
	if((byte & 0xc0) == 0x80) nbytes = 0;
	
	// The first byte's high bits tell us how many bytes are in the UTF-8 sequence.
	else if((byte & 0xf8) == 0xf0) nbytes = 4;
	else if((byte & 0xf0) == 0xe0) nbytes = 3;
	else if((byte & 0xe0) == 0xc0) nbytes = 2;
	else nbytes = 0;
	CSTL_CHECK(nbytes > 0, "Invalid code point");
	return nbytes;
}

static inline char* utf8_encode(Rune value) {
	Byte mask = 0x3f; // 63
	char* buff = cast(char*)calloc(4, sizeof(char));

	if(value <= (value << 7) - 1) {
		buff[0] = cast(char)value;
		return buff;
	} else if(value <= (value << 11) - 1) {
		buff[0] = (0xc0) | (cast(char)(value >> 6));
		buff[1] = (0x80) | (cast(char)(value) & mask);
		return buff;
	} 
	// Invalid/Surrogate range
	if(value > CSTL_RUNE_MAX || CSTL_IS_BETWEEN(value, 0xd800, 0xdff)) {
		value = CSTL_RUNE_INVALID;
		buff[0] = (0xe0) | (cast(char)(value >> 12));
		buff[1] = (0x80) | (cast(char)(value >> 12) & mask);
		buff[2] = (0x80) | (cast(char)(value) & mask);
		return buff;
	} else if(value <= (value << 16) - 1) {
		buff[0] = (0xe0) | (cast(char)(value >> 12));
		buff[1] = (0x80) | (cast(char)(value >> 6) & mask);
		buff[2] = (0x80) | (cast(char)(value) & mask);
		return buff;
	}

	buff[0] = (0xf0) | (cast(char)(value >> 18));
	buff[1] = (0x80) | (cast(char)(value >> 12) & mask);
	buff[2] = (0x80) | (cast(char)(value >> 6) & mask);
	buff[3] = (0x80) | (cast(char)(value) & mask);

	return buff;
}

#endif // CSTL_UTF8_H