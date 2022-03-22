/*
          _____   ____  _____            _____
    /\   |  __ \ / __ \|  __ \     /\   |  __ \
   /  \  | |  | | |  | | |__) |   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /   / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \  / ____ \| |__| | https://github.com/adorad/adorad/
/_/    \_\_____/ \____/|_|  \_\/_/    \_\_____/

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021-22 Jason Dsouza <@jasmcaus>
*/

#ifndef CORETEN_VECTOR_H
#define CORETEN_VECTOR_H

#include <adorad/core/types.h>

#define VEC_INIT_ALLOC_CAP      5
#define VECTOR_AT_MACRO(v, i)   ((void *)((char *) (v)->core.data + (i) * (v)->core.objsize))
#define VEC_NEW(strct, nelem)     _vec_new(sizeof(strct), (nelem))

typedef struct cstlVector cstlVector;
typedef cstlVector Vec;

struct cstlVector {
    struct {
        void** data;      // pointer to the underlying memory
        UInt64 len;      // number of elements currently in `vec`
        UInt64 capacity;  // allocated memory capacity (no. of elements)
        UInt64 objsize;   // size of each element in bytes
    } core;
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


#ifdef CORETEN_IMPL
    #include <string.h>
    
    // Create a new `cstlVector`
    // size = size of each element (in bytes)
    // capacity = number of elements
    cstlVector* _vec_new(UInt64 objsize, UInt64 capacity) {
        CORETEN_ENFORCE(cast(int)capacity > 0, "Really? `capacity` can only be > 0");
        
        cstlVector* vec = cast(cstlVector*)calloc(1, sizeof(cstlVector));
        CORETEN_ENFORCE_NN(vec, "Could not allocate memory. Memory full.");

        vec->core.data = cast(void*)calloc(objsize, capacity);
        CORETEN_ENFORCE_NN(vec->core.data, "Could not allocate memory. Memory full.");

        vec->core.capacity = capacity;
        vec->core.len = 0;
        vec->core.objsize = objsize;

        return vec;
    } 

    // Free a cstlVector from it's associated memory
    void vec_free(cstlVector* vec) {
        if(vec != null) {
            if(vec->core.data != null)
                free(vec->core.data);
            free(vec);
        }
    }

    // Return a pointer to element `i` in `vec`
    void* vec_at(cstlVector* vec, UInt64 elem) {
        CORETEN_ENFORCE_NN(vec, "Expected not null");
        CORETEN_ENFORCE_NN(vec->core.data, "Expected not null");

        if(elem > vec->core.len)
            return null;

        return VECTOR_AT_MACRO(vec, elem);
    }

    // Return a pointer to first element in `vec`
    void* vec_begin(cstlVector* vec) {
        CORETEN_ENFORCE_NN(vec, "Expected not null");
        CORETEN_ENFORCE_NN(vec->core.data, "Expected not null");

        if(vec->core.len == 0) 
            return null;
        
        return vec->core.data;
    }

    // Return a pointer to last element in `vec`
    void* vec_end(cstlVector* vec) {
        CORETEN_ENFORCE_NN(vec, "Expected not null");
        CORETEN_ENFORCE_NN(vec->core.data, "Expected not null");

        if(vec->core.data == 0)
            return null;
        
        return (void*)(cast(char*)vec->core.data + (vec->core.len - 1) * sizeof(cstlVector));
    }

    // Is `vec` empty?
    bool vec_is_empty(cstlVector* vec) {
        CORETEN_ENFORCE_NN(vec, "Expected not null");
        CORETEN_ENFORCE_NN(vec->core.data, "Expected not null");

        return vec->core.len == 0;
    }

    // Returns the size of `vec` (i.e the number of bytes)
    UInt64 vec_size(cstlVector* vec) {
        CORETEN_ENFORCE_NN(vec, "Expected not null");
        CORETEN_ENFORCE_NN(vec->core.data, "Expected not null");

        return vec->core.len;
    }

    // Returns the allocated capacity of `vec` (i.e the number of bytes)
    UInt64 vec_cap(cstlVector* vec) {
        CORETEN_ENFORCE_NN(vec, "Expected not null");
        CORETEN_ENFORCE_NN(vec->core.data, "Expected not null");

        return vec->core.capacity;
    }

    // Clear all contents of `vec`
    bool vec_clear(cstlVector* vec) {
        CORETEN_ENFORCE_NN(vec, "Expected not null");
        CORETEN_ENFORCE_NN(vec->core.data, "Expected not null");

        vec->core.len = 0;
        return true;
    }

    // Push an element into `vec` (at the end)
    bool vec_push(cstlVector* vec, const void* data) {
        CORETEN_ENFORCE_NN(vec, "Expected not null");
        CORETEN_ENFORCE_NN(vec->core.data, "Expected not null");

        if(vec->core.len + 1 > vec->core.capacity) {
            bool result = __vec_grow(vec, vec->core.len + 1);
            if(!result)
                return false;
        }

        CORETEN_ENFORCE(vec->core.objsize > 0);

        if(vec->core.data != null)
            memcpy(VECTOR_AT_MACRO(vec, vec->core.len), data, vec->core.objsize);

        vec->core.len++;
        return true;
    }

    // Pop an element from the end of `vec`
    bool vec_pop(cstlVector* vec) {
        CORETEN_ENFORCE_NN(vec, "Expected not null");
        CORETEN_ENFORCE_NN(vec->core.data, "Expected not null");

        if(vec->core.len == 0) 
            return false;
        
        vec->core.len--;
        return true;
    }

    // Grow the capacity of `vec` to at least `capacity`.
    // If more space == needed, grow `vec` to `capacity`, but at least by a factor of 1.5.
    bool __vec_grow(cstlVector* vec, UInt64 capacity) {
        void* newdata;
        UInt64 newcapacity;

        CORETEN_ENFORCE_NN(vec, "Expected not null");
        CORETEN_ENFORCE_NN(vec->core.data, "Expected not null");

        if (capacity <= vec->core.capacity)
            return true;

        CORETEN_ENFORCE(vec->core.objsize > 0);
        CORETEN_ENFORCE(capacity < cast(UInt64)-1/vec->core.objsize);

        // Grow small vectors by a factor of 2, && 1.5 for larger ones
        if (vec->core.capacity < VEC_INIT_ALLOC_CAP / vec->core.objsize) {
            newcapacity = vec->core.capacity + vec->core.capacity + 1;
        } else {
            newcapacity = vec->core.capacity + vec->core.capacity / 2 + 1;
        }

        if (capacity > newcapacity || newcapacity >= (size_t) -1 / vec->core.objsize)
            newcapacity = capacity;

        newdata = realloc(vec->core.data, newcapacity * vec->core.objsize);
        CORETEN_ENFORCE_NN(newdata, "Expected not null");

        vec->core.data = newdata;
        vec->core.capacity = newcapacity;

        return true;
    }

#endif // CORETEN_IMPL

#endif // CORETEN_VECTOR_H
