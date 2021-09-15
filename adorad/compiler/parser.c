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

// Initialize a new Parser
Parser* parser_init(Lexer* lexer) {
    Parser* parser = cast(Parser*)calloc(1, sizeof(Parser));
    parser->fullpath = lexer->loc->fname;
    // Generally, the ratio of lexer tokens to parser nodes is about 4:1
    // So, preallocate roughly 25% of the number of lexer tokens
    parser->nodelist = vec_new(AstNode, cast(UInt64)(vec_size(lexer->toklist) * .25));
    parser->lexer = lexer;
    parser->toklist = lexer->toklist;
    parser->curr_tok = cast(Token*)vec_at(parser->toklist, 0);
    parser->offset = 0;
    parser->num_tokens = vec_size(parser->toklist);
    parser->num_lines = 0;
    parser->mod_name = null;
    return parser;
}

static inline Token* parser_peek_token(Parser* parser) {
    return parser->curr_tok;
}

static inline Token* parser_peek_next(Parser* parser) {
    if(parser->offset + 1 >= parser->num_tokens)
        return null;

    return cast(Token*)parser->toklist + parser->offset + 1;
}

// Consumes a token and moves on to the next token
static inline Token* chomp(Parser* parser) {
    if(parser->offset >= parser->num_tokens)
        return null;

    return cast(Token*)parser->toklist + parser->offset++;
}

// Consumes a token and moves on to the next, if the current token matches the expected token.
static inline Token* chomp_if(Parser* parser, TokenKind tokenkind) {
    if(parser->curr_tok->kind == tokenkind)
        return parser_chomp();

    return null;
}

static inline void parser_put_back(Parser* parser) {
    if(parser->offset == 0)
        unreachable();
    parser->curr_tok -= 1;
    parser->offset -= 1;
}

static inline Token* expect_token(Parser* parser, TokenKind tokenkind) {
    if(parser->curr_tok->kind == tokenkind)
        return parser_chomp();
        
    ast_expected("`%s`; got `%s`", tokenHash[tokenkind], tokenHash[parser->curr_tok->kind]);
}

AstNode* ast_create_node(AstNodeKind kind) {
    AstNode* node = cast(AstNode*)calloc(1, sizeof(AstNode));
    node->kind = kind;
    return node;
}

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