#ifndef CSTL_ENDIAN_H_
#define CSTL_ENDIAN_H_

#if defined(__APPLE__)
    #include <machine/endian.h>
    #define CSTL_BIG_ENDIAN    BIG_ENDIAN
    #define CSTL_LITTLE_ENDIAN LITTLE_ENDIAN
    #define CSTL_BYTE_ORDER    BYTE_ORDER
#elif defined(__DragonFly__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
    #include <sys/endian.h>
    #define CSTL_BIG_ENDIAN    _BIG_ENDIAN
    #define CSTL_LITTLE_ENDIAN _LITTLE_ENDIAN
    #define CSTL_BYTE_ORDER    _BYTE_ORDER
#elif defined(_WIN32) || defined(_WIN64)
    // Assume that Windows installations are always little endian.
    #define CSTL_LITTLE_ENDIAN 1
    #define CSTL_BYTE_ORDER    CSTL_LITTLE_ENDIAN
#else // Linux
    #include <endian.h>
    #define CSTL_BIG_ENDIAN    __BIG_ENDIAN
    #define CSTL_LITTLE_ENDIAN __LITTLE_ENDIAN
    #define CSTL_BYTE_ORDER    __BYTE_ORDER
#endif

#if defined(CSTL_BYTE_ORDER) && CSTL_BYTE_ORDER == CSTL_LITTLE_ENDIAN
    const bool native_is_big_endian = false;
#elif defined(CSTL_BYTE_ORDER) && CSTL_BYTE_ORDER == CSTL_BIG_ENDIAN
    const bool native_is_big_endian = true;
#else
    #error Unsupported endianness
#endif

#endif // CSTL_ENDIAN_H_