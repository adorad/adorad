#ifndef HAZEL_TYPES 
#define HAZEL_TYPES 

// Data Types used in the Hazel Programming Language
typedef enum Hazel_Types {
    Null, 

    Int8, 
    Int16, 
    Int, // Int32
    Int64, 

    Float32, 
    Float64, 

    Byte, 
    String, 

    Unsigned16,
    Unsigned32, 
    Unsigned64, 

    TensorInt16,
    TensorInt32, 
    TensorInt64, 
    TensorFloat32, 
    TensorFloat64
} Types; 

#endif // HAZEL_TYPES 