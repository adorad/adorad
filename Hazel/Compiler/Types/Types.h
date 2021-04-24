#ifndef HAZEL_TYPES_H 
#define HAZEL_TYPES_H 

// Data Types used in the Hazel Programming Language
typedef enum Hazel_Types {
    HAZEL_TYPE_Any, 
    HAZEL_TYPE_Null, 
    HAZEL_TYPE_Bool, 
    HAZEL_TYPE_Byte, 
    HAZEL_TYPE_String, 
    HAZEL_TYPE_Rune, 

    HAZEL_TYPE_Int8, 
    HAZEL_TYPE_Int16, 
    HAZEL_TYPE_Int, // Int32
    HAZEL_TYPE_Int64, 
    //HAZEL_TYPE_Int128, // To be supported in the future 

    HAZEL_TYPE_Float32, 
    HAZEL_TYPE_Float64, 

    HAZEL_TYPE_UInt16,
    HAZEL_TYPE_UInt32, 
    HAZEL_TYPE_UInt64, 
    //HAZEL_TYPE_UInt128, // Will be supported in the future

    // Tensor-related types
    HAZEL_TYPE_TensorInt16,
    HAZEL_TYPE_TensorInt32, 
    HAZEL_TYPE_TensorInt64, 
    HAZEL_TYPE_TensorFloat32, 
    HAZEL_TYPE_TensorFloat64, 

    // Complex 
    HAZEL_TYPE_Complex32, 
    HAZEL_TYPE_Complex64,

    // Quaternion
    HAZEL_TYPE_Quaternion128, 
    HAZEL_TYPE_Quaternion256 
} Types; 

#endif // HAZEL_TYPES_H 