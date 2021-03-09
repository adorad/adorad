#include <stdlib.h> 

#include <hazel/core/hdef.h>
#include <hazel/runtime/ast/ast.h> 

Ast* ast_init(int type) {
    Ast* ast = calloc(1, sizeof(struct AstDef)); 
    ast->type = type; 
    
    // AST_VARIABLE_DEFINITION
    ast->var_definition_var_name = null; 
    ast->var_definition_value = null;

    // AST_VARIABLE
    ast->var_name = null; 

    // AST_FUNCTION_CALL
    ast->function_call_name = null; 
    ast->function_call_args = null; 
    ast->function_call_args_size = 0; 

    // AST_STRING
    ast->string_value = null; 

    // AST_COMPOUND
    ast->compound_value = null; 
    ast->compound_size = 0; 
    return ast; 
}