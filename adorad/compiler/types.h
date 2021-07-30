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
    AdoradTypeAny, 
    AdoradTypeNull, 
    AdoradTypeBool, 
    AdoradTypeByte, 
    AdoradTypeString, 
    AdoradTypeRune, 

    AdoradTypeInt8, 
    AdoradTypeInt16, 
    AdoradTypeInt, // Int32
    AdoradTypeInt64, 
    //AdoradTypeInt128, // To be supported in the future 

    AdoradTypeFloat32, 
    AdoradTypeFloat64, 

    AdoradTypeUInt16,
    AdoradTypeUInt32, 
    AdoradTypeUInt64, 
    //AdoradTypeUInt128, // Will be supported in the future

    // Tensor-related types
    AdoradTypeTensorInt16,
    AdoradTypeTensorInt32, 
    AdoradTypeTensorInt64, 
    AdoradTypeTensorFloat32, 
    AdoradTypeTensorFloat64, 

    // Complex 
    AdoradTypeComplex32,
    AdoradTypeComplex64,

    // Quaternion
    AdoradTypeQuaternion128, 
    AdoradTypeQuaternion256 
} AdoradTypes; 


#endif // ADORAD_TYPES_H 