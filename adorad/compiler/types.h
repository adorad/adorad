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

#ifndef ADORAD_TYPES_H 
#define ADORAD_TYPES_H 

// list of Data types/ used in the Adorad Programming Language
typedef enum {
    ADORADTYPE_Any, 
    ADORADTYPE_Null, 
    ADORADTYPE_Bool, 
    ADORADTYPE_Byte, 
    ADORADTYPE_String, 
    ADORADTYPE_Rune, 

    ADORADTYPE_Int8, 
    ADORADTYPE_Int16, 
    ADORADTYPE_Int, // Int32
    ADORADTYPE_Int64, 
    //ADORADTYPE_Int128, // To be supported in the future 

    ADORADTYPE_Float32, 
    ADORADTYPE_Float64, 

    ADORADTYPE_UInt16,
    ADORADTYPE_UInt32, 
    ADORADTYPE_UInt64, 
    //ADORADTYPE_UInt128, // Will be supported in the future

    // Tensor-related types
    ADORADTYPE_TensorInt16,
    ADORADTYPE_TensorInt32, 
    ADORADTYPE_TensorInt64, 
    ADORADTYPE_TensorFloat32, 
    ADORADTYPE_TensorFloat64, 

    // Complex 
    ADORADTYPE_Complex32,
    ADORADTYPE_Complex64,

    // Quaternion
    ADORADTYPE_Quaternion128, 
    ADORADTYPE_Quaternion256 
} AdoradTypes; 


#endif // ADORAD_TYPES_H 