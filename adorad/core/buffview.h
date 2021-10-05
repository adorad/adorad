#ifndef CORETEN_BUFFVIEW_H
#define CORETEN_BUFFVIEW_H

#include <adorad/core/types.h>

typedef struct cstlBuffView cstlBuffView;
typedef cstlBuffView BuffView;

struct cstlBuffView {
    const char* data;    // view data (immutable)
    UInt64 len;          // buffer size
};

#define BV(cstr_lit)   buffview_new_from_len(cstr_lit, sizeof(cstr_lit) - 1)

// printf macros for BuffView
// Usage:
//     BuffView view = ...;
//     printf("View: "BV_FMT"\n", BV_ARG(view))
#define BV_FMT        "%.*s"
#define BV_ARG(bv)    (int)(bv).len, (bv).data


#endif // CORETEN_BUFFVIEW_H
