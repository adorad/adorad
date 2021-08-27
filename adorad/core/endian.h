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

#ifndef CORETEN_ENDIAN_H_
#define CORETEN_ENDIAN_H_

#if defined(__APPLE__)
    #include <machine/endian.h>
    #define CORETEN_BIG_ENDIAN    BIG_ENDIAN
    #define CORETEN_LITTLE_ENDIAN LITTLE_ENDIAN
    #define CORETEN_BYTE_ORDER    BYTE_ORDER
#elif defined(__DragonFly__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
    #include <sys/endian.h>
    #define CORETEN_BIG_ENDIAN    _BIG_ENDIAN
    #define CORETEN_LITTLE_ENDIAN _LITTLE_ENDIAN
    #define CORETEN_BYTE_ORDER    _BYTE_ORDER
#elif defined(_WIN32) || defined(_WIN64)
    // Assume that Windows installations are always little endian.
    #define CORETEN_LITTLE_ENDIAN 1
    #define CORETEN_BYTE_ORDER    CORETEN_LITTLE_ENDIAN
#else // Linux
    #include <endian.h>
    #define CORETEN_BIG_ENDIAN    __BIG_ENDIAN
    #define CORETEN_LITTLE_ENDIAN __LITTLE_ENDIAN
    #define CORETEN_BYTE_ORDER    __BYTE_ORDER
#endif

#if defined(CORETEN_BYTE_ORDER) && CORETEN_BYTE_ORDER == CORETEN_LITTLE_ENDIAN
    #define NATIVE_IS_BIG_ENDIAN    0
#elif defined(CORETEN_BYTE_ORDER) && CORETEN_BYTE_ORDER == CORETEN_BIG_ENDIAN
    #define NATIVE_IS_BIG_ENDIAN    1
#else
    #error AdoradError: Unsupported endianness
#endif

static inline UInt16 endian_swap16(UInt16 i);
static inline UInt32 endian_swap32(UInt32 i);
static inline UInt64 endian_swap64(UInt64 i);

#endif // CORETEN_ENDIAN_H_