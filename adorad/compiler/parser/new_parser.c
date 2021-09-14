#include <adorad/compiler/ast.h>
#include <adorad/compiler/parser.h>
#include <adorad/compiler/parser/parser.c>

#define ast_expected(...) (ast_error("Expected %s", __VA_ARGS__))
#define ast_unexpected(...) (panic(ErrorUnexpectedToken, __VA_ARGS__))

// KEYWORD(if) LPAREN? Expr RPAREN? LBRACE Body RBRACE (KEYWORD(else) Body)?
static AstNode* ast_parse_if_expr(Parser* parser, AstNode* (*body_parser)(Parser*)) {
    Token* if_token = parser_chomp_if(IF);
    if(if_token == null) {
        unreachable();
        // return null;
    }

    Token* lparen = parser_chomp_if(LPAREN); // this is optional
    AstNode* condition = ast_parse_expr(parser);
    if(condition == null)
        ast_expected("condition");
    Token* rparen = parser_chomp_if(RPAREN); // this is optional

    AstNode* if_body = body_parser(parser);
    if(if_body == null)
        ast_expected("`if_body`");
    
    AstNode* else_body = null;
    Token* else_kwd = parser_chomp_if(ELSE);
    if(else_kwd != null)
        else_body = body_parser(parser);
    
    AstNode* out = ast_create_node(AstNodeKindIfExpr);
    out->data.expr->if_expr->condition = condition;
    out->data.expr->if_expr->if_body = if_body;
    out->data.expr->if_expr->has_else = else_body != null;
    out->data.expr->if_expr->else_node = else_body;
    return out;
}