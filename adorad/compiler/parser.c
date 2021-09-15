#include <adorad/compiler/ast.h>
#include <adorad/compiler/parser.h>

#define pt  parser->toklist
#define pc  parser->curr_tok
#define ast_error(...)              panic(ErrorParseError, __VA_ARGS__)
#define parser_chomp()              chomp(parser)
#define parser_chomp_if(kind)       chomp_if(parser, kind)
#define parser_expect_token(kind)   expect_token(parser, kind)
#define CURR_TOK_KIND               parser->curr_tok->kind

#define pars                Parser* parser
#define ast_expected(...)   (ast_error("Expected %s", __VA_ARGS__))
#define ast_unexpected(...) (panic(ErrorUnexpectedToken, __VA_ARGS__))

// ContainerMembers
//      ContainerDeclarations (ContainerField COMMA)* (ContainerField / ConstainerDeclarations)
// ContainerDeclarations
//      | TestDecl ContainerDeclarations
//      | TopLevelComptime ContainerDeclarations
//      | KEYWORD(export)? TopLevelDecl ContainerDeclarations
// TopLevelComptime
//      ATTRIBUTE(comptime) BlockExpr
static AstNode* ast_parse_container_members(pars) {
    while(true) {
        switch(CURR_TOK_KIND) {
            case ATTR_COMPTIME:
                Token* comptime_attr = parser_chomp();
                switch(CURR_TOK_KIND) {
                    // // Currently, a top-level comptime decl is as follows:
                    // //      `[comptime] { ... }`
                    // // TODO: Support single statements as well, like
                    // //      `[comptime] UInt32 i = 2` 
                    // case IDENTIFIER:
                    //     pc += 1;
                    case LBRACE:
                        AstNode* block = ast_parse_block(parser);
                        if(block != null) {
                            AstNode* out = ast_create_node(AstNodeKindAttributeExpr);
                            out->data.expr->attr_expr->kind = AttributeKindCompileTime;
                            out->data.expr->attr_expr->expr = block;
                        }
                        break;
                    default:
                        ast_error("Expected a `[comptime]` block");
                }
        }
    }
}

// KEYWORD(if) LPAREN? Expr RPAREN? LBRACE Body RBRACE (KEYWORD(else) Body)?
static AstNode* ast_parse_if_expr(pars, AstNode* (*body_parser)(Parser*)) {
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