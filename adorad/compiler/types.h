/*
          _____         _____           _____  
    /\   |  __ \ / __ \|  __ \    /\   |  __ \
   /  \  | |  | | |  | | |__)|   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /  / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \ / ____ \| |__| | https://github.com/adorad/adorad/
/_/_ __\_\_____/_\____/|_|__\_/_/_ __\_\_____/

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/

#ifndef HAZEL_TYPES_H 
#define HAZEL_TYPES_H 

// list of Data types/ used in the Adorad Programming Language
typedef enum {
    HAZELTYPE_Any, 
    HAZELTYPE_Null, 
    HAZELTYPE_Bool, 
    HAZELTYPE_Byte, 
    HAZELTYPE_String, 
    HAZELTYPE_Rune, 

    HAZELTYPE_Int8, 
    HAZELTYPE_Int16, 
    HAZELTYPE_Int, // Int32
    HAZELTYPE_Int64, 
    //HAZELTYPE_Int128, // To be supported in the future 

    HAZELTYPE_Float32, 
    HAZELTYPE_Float64, 

    HAZELTYPE_UInt16,
    HAZELTYPE_UInt32, 
    HAZELTYPE_UInt64, 
    //HAZELTYPE_UInt128, // Will be supported in the future

    // Tensor-related types
    HAZELTYPE_TensorInt16,
    HAZELTYPE_TensorInt32, 
    HAZELTYPE_TensorInt64, 
    HAZELTYPE_TensorFloat32, 
    HAZELTYPE_TensorFloat64, 

    // Complex 
    HAZELTYPE_Complex32,
    HAZELTYPE_Complex64,

    // Quaternion
    HAZELTYPE_Quaternion128, 
    HAZELTYPE_Quaternion256 
} AdoradTypes; 


#endif // HAZEL_TYPES_H 