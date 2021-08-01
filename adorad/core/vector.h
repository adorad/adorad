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

#ifndef CSTL_VECTOR_H
#define CSTL_VECTOR_H

#include <adorad/core/types.h>
#include <adorad/core/debug.h>

// We require this to be a large number, much more than what you might eventually use for more projects,
// but because CSTL is of great use and importance in the Adorad Programming Language (which potentially requires
// these many tokens), having a large number reduces the number of `realloc`s.
#define VEC_INIT_ALLOC_CAP  4096
#define VECTOR_AT_MACRO(v, i)   ((void *)((char *) (v)->internal.data + (i) * (v)->internal.objsize))

typedef struct {
    void** data;      // pointer to the underlying memory
    UInt64 size;      // number of elements currently in `vec`
    UInt64 capacity;  // allocated memory capacity (no. of elements)
    UInt64 objsize;   // size of each element in bytes
} cstlVectorInternal;

// The actual `cstlVector` struct
typedef struct cstlVector cstlVector;
struct cstlVector {
    cstlVectorInternal internal;

#ifndef _ADORAD_
    void* (*at)(cstlVector*, UInt64);
    void* (*begin)(cstlVector*);
    void* (*end)(cstlVector*);
    bool (*is_empty)(cstlVector*);
    UInt64 (*size)(cstlVector*);
    UInt64 (*capacity)(cstlVector*);
    bool (*clear)(cstlVector*);
    bool (*push)(cstlVector*, const void*);
    bool (*pop)(cstlVector*);
    void (*free)(cstlVector*);
#endif // _ADORAD_
};

// Create a new `cstlVector`
// size     => size of each element (in bytes)
// capacity => number of elements
// Returns `null` in the event of an error (e.g memory full; could not allocate)
static cstlVector* vec_new(UInt64 objsize, UInt64 capacity);
// Grow the capacity of `vec` to at least `capacity`.
// If more space is needed, grow `vec` to `capacity`, but at least by a factor of 1.5.
static bool vec_grow(cstlVector* vec, UInt64 capacity);
// Free a cstlVector from it's associated memory
static void vec_free(cstlVector* vec);
// Return a pointer to element `i` in `vec`
static void* vec_at(cstlVector* vec, UInt64 elem);
// Return a pointer to first element in `vec`
static void* vec_begin(cstlVector* vec);
// Return a pointer to last element in `vec`
static void* vec_end(cstlVector* vec);
// Is `vec` empty (i.e no elements)?
static bool vec_is_empty(cstlVector* vec);
// Returns the size of `vec` (i.e the number of bytes)
static UInt64 vec_size(cstlVector* vec);
// Returns the allocated capacity of `vec` (i.e the number of bytes)
static UInt64 vec_cap(cstlVector* vec);
// Clear all contents of `vec`
static bool vec_clear(cstlVector* vec);
// Push an element into `vec` (at the end)
// Returns false if `data` == null
static bool vec_push(cstlVector* vec, const void* data);
// Pop an element from the end of `vec`
static bool vec_pop(cstlVector* vec);

// Create a new `cstlVector`
// size     => size of each element (in bytes)
// capacity => number of elements
// Returns `null` in the event of an error (e.g memory full; could not allocate)
static cstlVector* vec_new(UInt64 objsize, UInt64 capacity) {
    if(capacity == 0)
        capacity = VEC_INIT_ALLOC_CAP;
    
    cstlVector* vec = (cstlVector*)calloc(1, sizeof(cstlVector));
    CSTL_CHECK_NOT_NULL(vec, "Could not allocate memory. Memory full.");

    vec->internal.data = (void*)calloc(objsize, capacity);
    if(vec->internal.data == null) {
        free(vec);
        CSTL_CHECK_NOT_NULL(vec->internal.data, "Could not allocate memory. Memory full.");
    }

    vec->internal.capacity = capacity;
    vec->internal.size = 0;
    vec->internal.objsize = objsize;

#ifndef _ADORAD_
    vec->at = &vec_at;
    vec->size = &vec_size;
    vec->capacity = &vec_cap;
    vec->push = &vec_push;
    vec->pop = &vec_pop;
    vec->begin = &vec_begin;
    vec->end = &vec_end;
    vec->is_empty = &vec_is_empty;
    vec->clear = &vec_clear;
    vec->free = &vec_free;
#endif // _ADORAD_

    return vec;
} 

// Free a cstlVector from it's associated memory
static void vec_free(cstlVector* vec) {
    if(vec == null) 
        return;
    if(vec->internal.data != null) {
        free(vec->internal.data);
        vec->internal.data = null;
    }
    vec->internal.capacity = 0;
    vec->internal.size = 0;
    vec->internal.objsize = 0;
}

// Return a pointer to element `i` in `vec`
static void* vec_at(cstlVector* vec, UInt64 elem) {
    CSTL_CHECK_NOT_NULL(vec, "Expected not null");
    CSTL_CHECK_NOT_NULL(vec->internal.data, "Expected not null");

    if(elem > vec->internal.size)
        return null;

    return VECTOR_AT_MACRO(vec, elem);
}

// Return a pointer to first element in `vec`
static void* vec_begin(cstlVector* vec) {
    CSTL_CHECK_NOT_NULL(vec, "Expected not null");
    CSTL_CHECK_NOT_NULL(vec->internal.data, "Expected not null");

    if(vec->internal.size == 0) 
        return null;
    
    return vec->internal.data;
}

// Return a pointer to last element in `vec`
static void* vec_end(cstlVector* vec) {
    CSTL_CHECK_NOT_NULL(vec, "Expected not null");
    CSTL_CHECK_NOT_NULL(vec->internal.data, "Expected not null");

    if(vec->internal.data == 0)
        return null;
    
    return (void*)((char*)vec->internal.data + (vec->internal.size - 1) * sizeof(cstlVector));
}

// Is `vec` empty (i.e no elements)?
static bool vec_is_empty(cstlVector* vec) {
    CSTL_CHECK_NOT_NULL(vec, "Expected not null");
    CSTL_CHECK_NOT_NULL(vec->internal.data, "Expected not null");

    return vec->internal.size == 0;
}

// Returns the size of `vec` (i.e the number of bytes)
static UInt64 vec_size(cstlVector* vec) {
    CSTL_CHECK_NOT_NULL(vec, "Expected not null");
    CSTL_CHECK_NOT_NULL(vec->internal.data, "Expected not null");

    return vec->internal.size;
}

// Returns the allocated capacity of `vec` (i.e the number of bytes)
static UInt64 vec_cap(cstlVector* vec) {
    CSTL_CHECK_NOT_NULL(vec, "Expected not null");
    CSTL_CHECK_NOT_NULL(vec->internal.data, "Expected not null");

    return vec->internal.capacity;
}

// Clear all contents of `vec`
static bool vec_clear(cstlVector* vec) {
    CSTL_CHECK_NOT_NULL(vec, "Expected not null");
    CSTL_CHECK_NOT_NULL(vec->internal.data, "Expected not null");

    vec->internal.size = 0;
    return true;
}

// Push an element into `vec` (at the end)
// Returns false if `data` == null
static bool vec_push(cstlVector* vec, const void* data) {
    CSTL_CHECK_NOT_NULL(vec, "Expected not null");
    CSTL_CHECK_NOT_NULL(vec->internal.data, "Expected not null");

    if(vec->internal.size + 1 > vec->internal.capacity) {
        bool result = vec_grow(vec, vec->internal.size + 1);
        if(!result)
            return false;
    }

    CSTL_CHECK_GT(vec->internal.objsize, 0);

    if(vec->internal.data != null)
        memcpy(VECTOR_AT_MACRO(vec, vec->internal.size), data, vec->internal.objsize);

    vec->internal.size++;
    return true;
}

// Pop an element from the end of `vec`
static bool vec_pop(cstlVector* vec) {
    CSTL_CHECK_NOT_NULL(vec, "Expected not null");
    CSTL_CHECK_NOT_NULL(vec->internal.data, "Expected not null");

    if(vec->internal.size == 0) 
        return false;
    
    vec->internal.size--;
    return true;
}

// Grow the capacity of `vec` to at least `capacity`.
// If more space is needed, grow `vec` to `capacity`, but at least by a factor of 1.5.
static bool vec_grow(cstlVector* vec, UInt64 capacity) {
    void* newdata;
    UInt64 newcapacity;

    CSTL_CHECK_NOT_NULL(vec, "Expected not null");
    CSTL_CHECK_NOT_NULL(vec->internal.data, "Expected not null");

    if (capacity <= vec->internal.capacity)
        return true;

    CSTL_CHECK_GT(vec->internal.objsize,  0);
    CSTL_CHECK_LT(capacity, (UInt64)-1/vec->internal.objsize);

    // Grow small vectors by a factor of 2, and 1.5 for larger ones
    if (vec->internal.capacity < 4096 / vec->internal.objsize) {
        newcapacity = vec->internal.capacity + vec->internal.capacity + 1;
    } else {
        newcapacity = vec->internal.capacity + vec->internal.capacity / 2 + 1;
    }

    if (capacity > newcapacity || newcapacity >= (size_t) -1 / vec->internal.objsize)
        newcapacity = capacity;

    newdata = realloc(vec->internal.data, newcapacity * vec->internal.objsize);
    CSTL_CHECK_NOT_NULL(newdata, "Expected not null");

    vec->internal.data = newdata;
    vec->internal.capacity = newcapacity;

    return true;
}

#endif // CSTL_VECTOR_H