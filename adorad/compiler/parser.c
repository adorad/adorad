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

// Statements
static AstNode* ast_parse_statement(Parser* parser) {
    AstNode* var_decl = ast_parse_var_decl(parser);
    if(var_decl != null) {
        CORETEN_CHECK(var_decl->kind == AstNodeKindVarDecl);
        return var_decl;
    }
    free(var_decl);

    // Defer
    Token* defer_stmt = parser_chomp_if(parser, DEFER);
    if(defer_stmt != null) {
        AstNode* statement = ast_parse_block_expr_statement(parser);
        AstNode* out = ast_create_node(AstNodeKindDefer);
        
        out->data.stmt->defer_stmt->expr = statement;
        return out;
    }
    free(defer_stmt);

    // If statement
    AstNode* if_statement = ast_parse_if_statement(parser);
    if(if_statement != null)
        return if_statement;
    free(if_statement);
    
    // Labeled Statements
    AstNode* labeled_statement = ast_parse_labeled_statements(parser);
    if(labeled_statement != null)
        return labeled_statement;
    free(labeled_statement);

    // Match statements
    AstNode* match_expr = ast_parse_match_expr(parser);
    if(match_expr != null)
        return match_expr;
    free(match_expr);

    // Assignment statements
    AstNode* assignment_expr = ast_parse_assignment_expr(parser);
    if(assignment_expr != null)
        return assignment_expr;
    free(assignment_expr);

    return null;
}

static AstNode* ast_parse_if_prefix(Parser* parser) {
    Token* if_kwd = parser_chomp_if(parser, IF);
    if(if_kwd == null) {
        free(if_kwd);
        return null;
    }
    Token* lparen = parser_expect_token(parser, LPAREN);
    AstNode* condition = ast_parse_expr(parser);
    Token* rparen = parser_expect_token(parser, RPAREN);
    free(lparen);
    free(rparen);

    AstNode* out = ast_clone_node(AstNodeKindIfExpr);
    out->data.expr->if_expr->condition = condition;

    return out;
}

static AstNode* ast_parse_if_statement(Parser* parser) {
    AstNode* out = ast_parse_if_prefix(parser);
    if(out == null) {
        free(out);
        return null;
    }
    
    AstNode* body = ast_parse_block_expr(parser);
    if(body == null)
        body = ast_parse_assignment_expr(parser);
    
    if(body == null) {
        Token* token = parser_chomp(parser);
        ast_error(
            "expected `if` body; found `%s`",
            token_to_buff(token->kind)->data
        );
    }

    AstNode* else_body = null;
    AstNode* else_kwd = parser_chomp_if(parser, ELSE);
    if(else_kwd != null)
        else_body = ast_parse_statement(parser);
    free(else_kwd);

    out->data.expr->if_expr->then_block = body;
    out->data.expr->if_expr->has_else = else_body != null;
    out->data.expr->if_expr->else_node = else_body;
    return out;
}

// Labeled Statements
static AstNode* ast_parse_labeled_statements(Parser* parser) {
    Token* label = ast_parse_block_label(parser);
    AstNode* block = ast_parse_block(parser);
    if(block != null) {
        CORETEN_CHECK(block->kind == AstNodeKindBlock);
        block->data.stmt->block_stmt->name = label->value;
        return block;
    }
    free(block);

    AstNode* loop = ast_parse_loop_statement(parser);
    if(loop != null) {
        loop->data.expr->loop_expr->label = label->value;
        return loop;
    }

    if(label != null)
        adorad_panic(
            ErrorUnexpectedToken,
            "invalid token: `%s`",
            parser_peek_token(parser)->value
        );
        
    return null;
}

// Loops
//      (KEYWORD(inline))? loop ... {  }
static AstNode* ast_parse_loop_statement(Parser* parser) {
    Token* inline_token = parser_chomp_if(parser, INLINE);

    AstNode* loop_c_statement = ast_parse_loop_c_statement(parser);
    if(loop_c_statement != null) {
        CORETEN_CHECK(loop_c_statement->kind == AstNodeKindLoopCExpr);
        loop_c_statement->data.expr->loop_expr->loop_c_expr->is_inline = inline_token != null;
        free(inline_token);
        return loop_c_statement;
    }

    AstNode* loop_while_statement = ast_parse_loop_while_statement(parser);
    if(loop_while_statement != null) {
        CORETEN_CHECK(loop_while_statement->kind == AstNodeKindLoopWhileExpr);
        loop_while_statement->data.expr->loop_expr->loop_while_expr->is_inline = inline_token != null;
        free(inline_token);
        return loop_while_statement;
    }

    AstNode* loop_in_statement = ast_parse_loop_in_statement(parser);
    if(loop_in_statement != null) {
        CORETEN_CHECK(loop_in_statement->kind == AstNodeKindLoopWhileExpr);
        loop_in_statement->data.expr->loop_expr->loop_in_expr->is_inline = inline_token != null;
        free(inline_token);
        return loop_in_statement;
    }

    if(inline_token != null)
        adorad_panic(
            ErrorUnexpectedToken,
            "invalid token: `%s`",
            parser_peek_token(parser)->value
        );
}

// Block Statement
//     BlockExpr       // { ... }
//     AssignmentExpr 
static AstNode* ast_parse_block_expr_statement(Parser* parser) {
    AstNode* block = ast_parse_block_expr(parser);
    if(block != null)
        return block;
    
    AstNode* assignment_expr = ast_parse_assignment_expr(parser);
    if(assignment_expr != null) {
        Token* semi = parser_expect_token(parser, SEMICOLON);
        free(semi);
        return assignment_expr;
    }
    
    return null;
}

// Block Expression
//      (BlockLabel)? block
static AstNode* ast_parse_block_expr(Parser* parser) {
    Token* block_label = ast_parse_block_label(parser);
    if(block_label != null) {
        AstNode* out = ast_parse_block(parser);
        CORETEN_CHECK(out->kind == AstNodeKindBlock);
        out->data.stmt->block_stmt->name = block_label->value;
        return out;
    }

    return ast_parse_block(parser);
}

// Assignment Expr
// FIXME!
static AstNode* ast_parse_assignment_expr(Parser* parser) {
    return ast_parse_binary_op_expr(parser);
}

// Primary Expression can be one of:
//      | IfExpr
//      | KEYWORD(break) BreakLabel? Expr?
//      | KEYWORD(continue) BreakLabel?
//      | KEYWORD(return) Expr?
//      | BlockLabel? LoopExpr
//      | Block
//      | CurlySuffixExpr
static AstNode* ast_parse_primary_expr(Parser* parser) {
    AstNode* if_expr = ast_parse_if_expr(parser);
    if(if_expr != null)
        return if_expr;

    Token* break_token = parser_chomp_if(parser, BREAK);
    if(break_token != null) {
        free(break_token);
        Token* label = ast_parse_break_label(parser);
        AstNode* expr = ast_parse_expr(parser);
        
        AstNode* out = ast_create_node(BREAK);
        out->data.stmt->branch_stmt->name = label->value;
        out->data.stmt->branch_stmt->type = AstNodeBranchStatementBreak;
        out->data.stmt->branch_stmt->expr = expr;
        return out;
    }
}