#ifndef HAZEL_TYPES 
#define HAZEL_TYPES 

// Data Types used in the Hazel Programming Language
typedef enum Hazel_Types {
    Null, 
    Bool, 
    Any, 
    Byte, 
    String, 
    Rune, 

    Int8, 
    Int16, 
    Int, // Int32
    Int64, 
    // Int128, // To be supported in the future 

    Float32, 
    Float64, 

    Unsigned16,
    Unsigned32, 
    Unsigned64, 
    // Unsigned128, // Will be supported in the future

    // Tensor-related typed
    TensorInt16,
    TensorInt32, 
    TensorInt64, 
    TensorFloat32, 
    TensorFloat64, 

    // Complex 
    Complex32, 
    Complex64,

    // Quaternion
    Quaternion128, 
    Quaternion256 
} Types; 

#endif // HAZEL_TYPES 