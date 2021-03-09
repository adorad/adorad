#include <stdlib.h> 

#include <hazel/core/hdef.h>
#include <hazel/runtime/ast/ast.h> 

Ast* ast_init(int type) {
    Ast* ast = calloc(1, sizeof(struct AstDef)); 
    ast->type = type; 
    
    // AST_VARIABLE_DEFINITION
    ast->variable_definition_variable_name = null; 
    ast->variable_definition_value = null;

    // AST_VARIABLE
    ast->variable_name = null; 

    // AST_FUNCTION_CALL
    ast->function_call_name = null; 
    ast->function_call_args = null; 
    ast->function_call_args_size = 0; 

    // AST_STRING
    ast->string_value = null; 

    return ast; 
}