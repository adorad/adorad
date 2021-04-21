#ifndef CSTL_COMPILERS_H
#define CSTL_COMPILERS_H

#ifdef __cplusplus
extern "C" {
#endif

// Compilers ==========================================
// CSTL_COMPILER_...
#if defined(_MSC_VER)
    #define CSTL_COMPILER_MSVC 1
#elif defined(__GNUC__)
    #define CSTL_COMPILER_GCC 1
#elif defined(__clang__)
    #define CSTL_COMPILER_CLANG 1
#else 
    #error Unknown Compiler (Hazel currently supports only MSVC, GCC and Clang)
#endif 

#if defined(__cplusplus)
}
#endif

#endif // CSTL_COMPILERS_H