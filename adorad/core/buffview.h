#ifndef CORETEN_BUFFVIEW_H
#define CORETEN_BUFFVIEW_H

#include <adorad/core/types.h>

typedef struct cstlBuffView cstlBuffView;
typedef cstlBuffView BuffView;

struct cstlBuffView {
    const char* data;    // view data (immutable)
    UInt64 len;          // buffer size
};

#define BV(cstr_lit)   buffview_new(cstr_lit, sizeof(cstr_lit) - 1)

// printf macros for BuffView
// Usage:
//     BuffView view = ...;
//     printf("View: "BV_FMT"\n", BV_ARG(view))
#define BV_FMT        "%.*s"
#define BV_ARG(bv)    (int)(bv).len, (bv).data

cstlBuffView buffview_new(const char* data, UInt64 len);
char* buffview_end(cstlBuffView* view);
cstlBuffView buff_rev(cstlBuffView* view);

char* buffview_end(cstlBuffView* view) {
    return cast(char*)view->data + view->len - 1;
}

cstlBuffView buffview_rev(cstlBuffView* view) {
    cstlBuffView rev = buff_new(null);
    UInt64 length = buffer->len;
    if(!length)
        return rev;
    
    char* temp = cast(char*)calloc(1, length + 1);
    for(UInt64 i=0; i<length; i++)
        *(temp + i) = *(buffer->data + length - i - 1);
    
    buffview_new(temp);
    return rev;
}
#endif // CORETEN_BUFFVIEW_H
