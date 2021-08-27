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

#ifndef CORETEN_HASH_H
#define CORETEN_HASH_H

/*
    Hashing & Checksum Functions
*/

UInt32 hash_adler32(void const* data, Ll len);

UInt32 hash_crc32(void const* data, Ll len);
UInt64 hash_crc64(void const* data, Ll len);

UInt32 hash_fnv32 (void const* data, Ll len);
UInt64 hash_fnv64 (void const* data, Ll len);
UInt32 hash_fnv32a(void const* data, Ll len);
UInt64 hash_fnv64a(void const* data, Ll len);

// Default seed of 0x9747b28c
// I prefer using `murmur64` formost hashes
UInt32 hash_murmur32(void const* data, Ll len);
UInt64 hash_murmur64(void const* data, Ll len);

UInt32 hash_murmur32_seed(void const* data, Ll len, UInt32 seed);
UInt64 hash_murmur64_seed(void const* data__, Ll len, UInt64 seed);

#endif // CORETEN_HASH_H