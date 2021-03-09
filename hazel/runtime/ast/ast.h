#ifndef HAZEL_AST
#define HAZEL_AST 

typedef struct AstDef {
    enum {
        AST_VARIABLE_DEFINITION, 
        AST_VARIABLE, 
        AST_FUNCTION_CALL, 
        AST_STRING
    } type; 

    // AST_VARIABLE_DEFINITION
    char* variable_definition_variable_name ; 
    struct AstDef* variable_definition_value;

    // AST_VARIABLE
    char* variable_name; 

    // AST_FUNCTION_CALL
    char* function_call_name; 
    struct AstDef** function_call_args; 
    size_t function_call_args_size; 

    // AST_STRING
    char* string_value; 
} Ast;

#endif // HAZEL_AST 