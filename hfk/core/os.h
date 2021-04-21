#ifndef CSTL_OS_H
#define CSTL_OS_H

#ifdef __cplusplus
extern "C" {
#endif

// Operating Systems ==========================================
// CSTL_SYSTEM_...
#if defined(_WIN32) || defined(_WIN64)
    #ifndef CSTL_SYSTEM_WINDOWS
        #define CSTL_SYSTEM_WINDOWS 1
    #endif

#elif defined(__APPLE__) && defined(__MACH__)
    #ifndef CSTL_SYSTEM_OSX
        #define CSTL_SYSTEM_OSX 1
    #endif

#elif defined(__unix__)
    #ifndef CSTL_SYSTEM_UNIX
        #define CSTL_SYSTEM_UNIX 1
    #endif

    #if defined(__linux__)
        #ifndef CSTL_SYSTEM_LINUX
            #define CSTL_SYSTEM_LINUX 1
        #endif
    
    #elif defined(__FreeBSD__) && defined(__FreeBSD_kernel__)
        #ifndef CSTL_SYSTEM_FREEBSD
            #define CSTL_SYSTEM_FREEBSD 1
        #endif

    #else 
        #error This Unix Operating System in not supported by Hazel.
    #endif 

#else
    #error This Operating System in not supported by Hazel
#endif

#if defined(__cplusplus)
}
#endif

#endif // CSTL_OS_H