#include <adorad/compiler/ast.h>
#include <adorad/compiler/parser.h>
#include <adorad/compiler/parser/parser.c>

// hello()
// hello<T>()
// hello<T, Q>()
static AstNode* call_expr(Parser* parser) {
    Buff* func_name = parser->curr_tok->value;
    
    // TODO: Support generics
    // if(CURR_TOK_KIND == LESS_THAN) {
    //     // foo<int a, int b>(10)
    //     Vec* concrete_types = ast_parse_generic_list(parser);
    // }

    // `(`
    Token* lparen = parser_expect_token(LPAREN);
    free(lparen);
    AstNode* args = ast_parse_func_call_args(parser);
    // `)`
    Token* rparen = parser_expect_token(RPAREN);
    free(rparen);

    if(CURR_TOK_KIND == ORELSE) {
        // `foo()` orelse `some expr`
        
    }

}