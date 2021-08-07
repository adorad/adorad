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

#if defined(__clang__)
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wold-style-cast"
	#pragma clang diagnostic ignored "-Wcast-qual"
#endif

// Is UTF-8 codepoint valid?
bool utf8_is_codepoint_valid(Rune uc);
char* utf8_encode(Rune value);
Ll utf8_encode_nbytes(Rune value);
Ll utf8_decode_nbytes(Rune byte);
char* utf8_encode(Rune value);

bool utf8_is_codepoint_valid(Rune uc) {
	if(uc < 0 || uc >= 0x110000 || ((uc & 0xFFFF) >= 0xFFFE) || (uc >= 0xD800 && uc < 0xE000) || 
	  (uc >= 0xFDD0 && uc < 0xFDF0))
		return false;
	return true;
}

// Codepoint size
Ll utf8_encode_nbytes(Rune value) {
	Ll nbytes = 0;
	CSTL_CHECK(value >= 0, "Cannot encode a negative value :(");

	if(value <= 0x7f) nbytes = 1;     // 127
	if(value <= 0x7ff) nbytes = 2;    // 2047
	if(value <= 0xffff) nbytes = 3;   // 65535
	if(value <= 0x10ffff) nbytes = 4; // 1114111
	else nbytes = 0;
	CSTL_CHECK(nbytes > 0, "Invalid code point");
	return nbytes;
}

Ll utf8_decode_nbytes(Rune byte) {
	CSTL_CHECK(byte >= 0, "Cannot decode  a negative value :(");
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

char* utf8_encode(Rune value) {
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