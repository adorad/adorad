#include <adorad/compiler/ast.h>
#include <adorad/compiler/parser.h>

// hello()
// hello<T>()
// hello<T, Q>()
static AstNode* call_expr(Parser* parser) {
    Buff* func_name = parser->curr_tok->value;
    
    // TODO: Support generics
    // if(parser->curr_tok->kind == LESS_THAN) {
    //     // foo<int a, int b>(10)
    //     Vec* concrete_types = ast_parse_generic_list(parser);
    // }

    expe
}