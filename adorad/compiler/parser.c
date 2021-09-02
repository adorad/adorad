/*
          _____   ____  _____            _____
    /\   |  __ \ / __ \|  __ \     /\   |  __ \
   /  \  | |  | | |  | | |__) |   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /   / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \  / ____ \| |__| | https://github.com/adorad/adorad/
/_/    \_\_____/ \____/|_|  \_\/_/    \_\_____/

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <@jasmcaus>
*/

#include <stdlib.h>
#include <adorad/compiler/parser.h>
#include <adorad/core/debug.h>

// Shortcut to `parser->toklist`
#define pt  parser->toklist
#define ast_error(...)  adorad_panic(ErrorParseError, __VA_ARGS__)

// Initialize a new Parser
Parser* parser_init(Lexer* lexer) {
    Parser* parser = cast(Parser*)calloc(1, sizeof(Parser));
    parser->lexer = lexer;
    parser->toklist = lexer->toklist;
    parser->curr_tok = cast(Token*)vec_at(parser->toklist, 0);
    parser->num_tokens = vec_size(parser->toklist);
    parser->num_lines = 0;
    parser->mod_name = null;
    parser->defer_vars = null;
    return parser;
}

inline Token* parser_peek_token(Parser* parser) {
    return parser->curr_tok;
}

// Consumes a token and moves on to the next token
inline Token* parser_chomp(Parser* parser) {
    Token* tok = parser_peek_token(parser);
    parser->curr_tok += 1;
    return tok;
}

// Consumes a token and moves on to the next, if the current token matches the expected token.
inline Token* parser_chomp_if(Parser* parser, TokenKind tokenkind) {
    if(parser->curr_tok->kind == tokenkind)
        return parser_chomp(parser);

    return null;
}

inline Token* parser_expect_token(Parser* parser, TokenKind tokenkind) {
    if(parser->curr_tok->kind == tokenkind)
        return parser_chomp(parser);
        
    adorad_panic(ErrorUnexpectedToken, "Expected `%s`; got `%s`", 
                                        token_to_buff(tokenkind)->data,
                                        token_to_buff(parser->curr_tok->kind)->data);
    abort();
}

AstNode* ast_create_node(AstNodeKind kind) {
    AstNode* node = cast(AstNode*)calloc(1, sizeof(AstNode));
    node->kind = kind;
    return node;
}

AstNode* ast_clone_node(AstNode* node) {
    if(!node)
        adorad_panic(ErrorUnexpectedNull, "Trying to clone a null AstNode?");
    AstNode* new = ast_create_node(node->kind);
    // TODO(jasmcaus): Add more struct members
    return new;
}

// General format:
//      KEYWORD(func) IDENT LPAREN ParamDeclList RPAREN LARROW RETURNTYPE
static AstNode* ast_parse_func_prototype(Parser* parser) {
    Token* func = parser_chomp_if(parser, FUNC);
    if(func == null)
        return null;
    
    Token* identifier = parser_chomp_if(parser, IDENTIFIER);
    parser_expect_token(parser, LPAREN);
    Vec* params = ast_parse_list(params, COMMA);
    parser_expect_token(params, RPAREN);

    AstNode* return_type = ast_parse_type_expr(parser);
    if(return_type == null) {
        Token* next = parser_peek_token(parser);
        ast_error(
            "expected return type; found`%s`",
            token_to_buff(next->kind)->data
        );
    }

    AstNode* out = ast_create_node(AstNodeKindFuncPrototype);
    out->data.stmt->func_proto_decl;
    out->data.stmt->func_proto_decl->name = identifier->value;
    out->data.stmt->func_proto_decl->params = params;
    out->data.stmt->func_proto_decl->return_type = return_type;

    for(UInt64 i = 0; i < vec_size(params); i++) {
        AstNode* param_decl = vec_at(params, i);
        CORETEN_CHECK(param_decl->kind == AstNodeKindParamDecl);
        if(param_decl->data.param_decl->is_var_args)
            out->data.stmt->func_proto_decl->is_var_args = true;
        
        // Check for multiple variadic arguments in prototype
        // Adorad supports only 1
        if(i != vec_size(params) - 1 && out->data.stmt->func_proto_decl->is_var_args)
            ast_error(
                "Cannot have multiple variadic arguments in function prototype"
            );
    }
    return out;
}

// General format:
// `?` represents optional
//      KEYWORD(export)? KEYWORD(mutable/const)? TypeExpr? IDENTIFIER EQUAL? Expr?
static AstNode* ast_parse_var_decl(Parser* parser) {
    Token* export_kwd = parser_chomp_if(parser, EXPORT);
    Token* mutable_kwd = parser_chomp_if(parser, MUTABLE);
    Token* const_kwd = parser_chomp_if(parser, CONST);
    if(mutable_kwd && const_kwd)
        ast_error("Cannot decorate a variable as both `mutable` and `const`");

    AstNode* type_expr = ast_parse_type_expr(parser);
    Token* identifier = parser_expect_token(parser, IDENTIFIER);
    Token* equals = parser_chomp_if(parser, EQUALS);
    AstNode* expr;
    if(equals != null)
        expr = ast_parse_expr(parser);
    
    parser_expect_token(parser, SEMICOLON); // TODO: Remove this need

    AstNode* out = ast_create_node(AstNodeKindVarDecl);
    out->data.stmt->var_decl->name = identifier->value;
    out->data.stmt->var_decl->is_export = export_kwd != null;
    out->data.stmt->var_decl->is_mutable = mutable_kwd != null;
    out->data.stmt->var_decl->is_const = const_kwd != null;
    out->data.stmt->var_decl->expr = expr;
    return out;
}