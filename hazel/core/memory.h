#ifndef CSTL_MEMORY_H
#define CSTL_MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

// Memory ==========================================
#ifndef KB_TO_BYTES
    #define KB_TO_BYTES(x) (           (x) * (Int64)(1024))
    #define MB_TO_BYTES(x) (KB_TO_BYTES(x) * (Int64)(1024))
    #define GB_TO_BYTES(x) (MB_TO_BYTES(x) * (Int64)(1024))
    #define TB_TO_BYTES(x) (GB_TO_BYTES(x) * (Int64)(1024))
#endif 

#if defined(__cplusplus)
}
#endif

#endif // CSTL_MEMORY_H