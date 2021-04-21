#ifndef CSTL_CPU_H
#define CSTL_CPU_H

#ifdef __cplusplus
extern "C" {
#endif


// CPUs ==========================================
// CSTL_CPU_...
#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
    #ifndef CSTL_CPU_X86
         #define CSTL_CPU_X86 1
    #endif

    #ifndef CSTL_CACHE_LINE_SIZE
        #define CSTL_CACHE_LINE_SIZE 64
    #endif

#elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__)
    #ifndef CSTL_CPU_PPC
        #define CSTL_CPU_PPC 1
    #endif

    #ifndef CSTL_CACHE_LINE_SIZE
        #define CSTL_CACHE_LINE_SIZE 128
    #endif

#elif defined(__arm__) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64)
    #ifndef CSTL_CPU_ARM
        #define CSTL_CPU_ARM 1
    #endif

    #ifndef CSTL_CACHE_LINE_SIZE
        #define CSTL_CACHE_LINE_SIZE 64
    #endif

#elif defined(__MIPSEL__) || defined(__mips_isa_rev)
    #ifndef CSTL_CPU_MIPS
        #define CSTL_CPU_MIPS 1
    #endif

    #ifndef CSTL_CACHE_LINE_SIZE
        #define CSTL_CACHE_LINE_SIZE 64
    #endif

#else
    #error Unknown CPU Type
#endif // CSTL_CPU_...

#if defined(__cplusplus)
}
#endif

#endif // CSTL_CPU_H