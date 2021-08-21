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

#ifndef CORETEN_VECTOR_H
#define CORETEN_VECTOR_H

#include <adorad/core/types.h>
#include <adorad/core/debug.h>

// We require this to be a large number, much more than what you might eventually use for more projects.
// This is because CSTL is of great use and importance in the Adorad Programming Language (which requires
// these many tokens during lexing/tokenization). Having a large number reduces the number of `realloc`s.
#define VEC_INIT_ALLOC_CAP      4096
#define VECTOR_AT_MACRO(v, i)   ((void *)((char *) (v)->internal.data + (i) * (v)->internal.objsize))
#define vec_new(obj, nelem)     _vec_new(sizeof(obj), (nelem))

typedef struct {
    void** data;      // pointer to the underlying memory
    UInt64 size;      // number of elements currently in `vec`
    UInt64 capacity;  // allocated memory capacity (no. of elements)
    UInt64 objsize;   // size of each element in bytes
} cstlVectorInternal;

// The actual `cstlVector` struct
typedef struct cstlVector cstlVector;
typedef cstlVector Vec;
struct cstlVector {
    cstlVectorInternal internal;
};

cstlVector* _vec_new(UInt64 objsize, UInt64 capacity);
bool __vec_grow(cstlVector* vec, UInt64 capacity);
void vec_free(cstlVector* vec);
void* vec_at(cstlVector* vec, UInt64 elem);
void* vec_begin(cstlVector* vec);
void* vec_end(cstlVector* vec);
bool vec_is_empty(cstlVector* vec);
UInt64 vec_size(cstlVector* vec);
UInt64 vec_cap(cstlVector* vec);
bool vec_clear(cstlVector* vec);
bool vec_push(cstlVector* vec, const void* data);
bool vec_pop(cstlVector* vec);

#endif // CORETEN_VECTOR_H
