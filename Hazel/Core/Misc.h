#ifndef CSTL_MISCELLANEOUS_H
#define CSTL_MISCELLANEOUS_H

#ifdef __cplusplus
extern "C" {
#endif


// Defines 
#ifndef CSTL_DEF 
    #ifdef CSTL_EXTERN
        #define CSTL_DEF extern
    #else 
        #define CSTL_DEF static
    #endif 
#endif // CSTL_DEF


// Inline 
#ifdef __cplusplus
    #if defined(_MSC_VER) && _MSC_VER <= 1800 
        #define inline  __inline
    #elif !defined(__STDC_VERSION__)
        #define inline __inline__
    #else 
        #define inline 
    #endif 
#endif 


// Force Inline
#ifndef force_inline
    #if defined(_MSC_VER)
        #if _MSC_VER < 1300
            #define force_inline
        #else 
            #define force_inline __forceinline
        #endif 
    #else 
        #define force_inline    __attribute__ ((__always_inline__))
    #endif 
#endif 


// No Inline 
#ifndef no_inline
    #if defined(_MSC_VER)
        #define no_inline   __declspec(noinline)
    #else 
        #define no_inline   __attribute__ ((noinline))
    #endif 
#endif 


// Cast 
#ifndef cast 
    #define cast(Type)  (Type)
#endif 


// A signed sizeof is more useful 
#ifndef CSTL_SIZEOF
    #define CSTL_SIZEOF(x)     (Ll)(sizeof(x))
#endif 


// Statics!
// static means 3-4 different things in C/C++!!
#ifndef CSTL_GLOBAL
    #define CSTL_GLOBAL       static // Global Variables
    #define CSTL_INTERNAL     static // Internal Linkage
    #define CSTL_LOCALPERSIST static // Local Persisting Variables  
#endif 


// Execute power operations
long power(long x, long y) {
    int total; 
    
    if(y == 0) return 1; 
    else if(y == 1) return x; 
    else if(y == -1) return 1/x; 
    else if(y > 0) {
        total = x; 
        total *= power(x, y-1); 
    } 
    else {
        total = 1/x; 
        total *= power(x, y+1); 
    }

    return total; 
}


// Some fun with macros ==========================================
#ifndef CSTL_MAX
    #define CSTL_MAX(x, y)     ((x) > (y) ? (x) : (y))
#endif 

#ifndef CSTL_MIN
    #define CSTL_MIN(x, y)     ((x) < (y) ? (x) : (y))
#endif 

#ifndef CSTL_CLAMP
    #define CSTL_CLAMP(x, lower, upper)     CSTL_MIN(CSTL_MAX((x), (lower)), (upper))
#endif 

#ifndef CSTL_IS_BETWEEN
    #define CSTL_IS_BETWEEN(x, lower, upper)     (((lower) <= (x) ) && ((x) <= (upper)))
#endif 

#ifndef CSTL_ABS
    #define CSTL_ABS(x)     ((x) < 0 ? -(x) : (x))
#endif 

#if defined(__cplusplus)
}
#endif

#endif // CSTL_MISCELLANEOUS_H