#ifndef HAZEL_AST
#define HAZEL_AST 

#include <hazel/core/hdef.h> 

typedef struct AstDef {
    enum {
        AST_VARIABLE_DEFINITION, 
        AST_FUNC_DEFINITION, 
        AST_VARIABLE, 
        AST_FUNC_CALL, 
        AST_STRING, 
        AST_COMPOUND,
        AST_NOOP      // List of AST nodes 
    } type; 

    // AST_VARIABLE_DEFINITION
    char* var_definition_var_name ; 
    struct AstDef* var_definition_value;

    // AST_VARIABLE
    char* var_name; 

    // AST_FUNCTION_CALL
    char* function_call_name; 
    struct AstDef** function_call_args; 
    Ull function_call_args_size; 

    // AST_STRING
    char* string_value; 

    // AST_COMPOUND
    struct AstDef** compound_value; 
    Ull compound_size; 
} Ast;

#endif // HAZEL_AST 