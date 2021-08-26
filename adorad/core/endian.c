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

#include <adorad/core/endian.h>

inline UInt16 endian_swap16(UInt16 i) {
    return (i >> 8) | (i << 8);
}

inline UInt32 endian_swap32(UInt32 i) {
    return (i >> 24) | (i << 24) | ((i & 0x00ff0000u) >> 8) | ((i & 0x0000ff00u) << 8);
}

inline UInt64 endian_swap64(UInt64 i) {
    return (i >> 56) | (i << 56) |
           ((i & 0x00ff000000000000ull) >> 40) | ((i & 0x000000000000ff00ull) << 40) |
           ((i & 0x0000ff0000000000ull) >> 24) | ((i & 0x0000000000ff0000ull) << 24) |
           ((i & 0x000000ff00000000ull) >> 8)  | ((i & 0x00000000ff000000ull) << 8);
}