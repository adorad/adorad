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
#include <adorad/core/vector.h>

// Shortcut to `parser->toklist`
#define pt  parser->toklist
#define ast_error(...)  panic(ErrorParseError, __VA_ARGS__)

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
        
    panic(ErrorUnexpectedToken, "Expected `%s`; got `%s`", 
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
        panic(ErrorUnexpectedNull, "Trying to clone a null AstNode?");
    AstNode* new = ast_create_node(node->kind);
    // TODO(jasmcaus): Add more struct members
    return new;
}

/*
    A large part of the Parser from this point onwards has been selfishly stolen from Zig's Compiler.

    Before, we release the first stable version of Adorad, this parser implementation will be reworked and improved.

    Related source code: https://github.com/ziglang/zig/blob/master/src/stage1/parser.cpp
*/

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
        panic(
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
        panic(
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

static AstNode* ast_parse_block(Parser* parser) {
    Token* lbrace = parser_chomp_if(parser, LBRACE);
    if(lbrace == null)
        return null;

    Vec* statements = vec_new(AstNode, 1);
    AstNode* statement = null;
    while((statement = ast_parse_statement(parser)) != null)
        vec_push(statements, statement);

    Token* rbrace = parser_expect_token(parser, RBRACE);
    free(lbrace);
    free(rbrace);

    AstNode* out = ast_create_node(AstNodeKindBlock);
    out->data.stmt->block_stmt->statements = statements;
    return out;
}


// This has been selfishly ported from Zig's Compiler
// Source for this: https://github.com/ziglang/zig/blob/master/src/stage1/parser.cpp
typedef enum BinaryOpChain {
    BinaryOpChainOnce,
    BinaryOpChainInfinity
} BinaryOpChain;

// Child (Op Child)(*/?)
static AstNode* ast_parse_binary_op_expr(
    Parser* parser,
    BinaryOpChain chain,
    AstNode* (*op_parser)(Parser*),
    AstNode* (*child_parser)(Parser*)
) {
    AstNode* out = child_parser(parser);
    AstNode* ou;
    if(out == null)
        return null;

    do {
        AstNode* op = op_parser(parser);
        if(op == null)
            break;

        AstNode* left = out;
        AstNode* right = child_parser(parser);
        out = op;

        if(op->kind == AstNodeKindBinaryOpExpr) {
            op->data.expr->binary_op_expr->lhs = left;
            op->data.expr->binary_op_expr->rhs = right;
        } else {
            unreachable();
        }
    } while(chain == BinaryOpChainInfinity);

    return out;
}

// AssignmentExpr
//      Expr  (AssignmentOp Expr)?
static AstNode* ast_parse_assignment_expr(Parser* parser) {
    return ast_parse_binary_op_expr(
        parser,
        BinaryOpChainOnce,
        ast_parse_assignment_op,
        ast_parse_expr
    );
}

// Expression
//      KEYWORD(try) BoolOrExpr
static AstNode* ast_parse_expr(Parser* parser) {
    return ast_parse_prefix_op_expr(
        parser,
        [](Parser* parser) {
            Token* try = parser_chomp_if(parser, TRY);
            if(try != null) {
                AstNode* out = ast_create_node(AstNodeKindReturnExpr);
                out->data.stmt->return_stmt->kind = ReturnKindError;
                return out;
            }
            return cast(AstNode*)null;
        },
        ast_parse_bool_or_expr
    );
}

// BoolOrExpr
//      BoolAndExpr (KEYWORD(or) BoolAndExpr)*
static AstNode* ast_parse_bool_or_expr(
    return ast_parse_binary_op_expr(
        parser, 
        BinaryOpChainInfinity,
        ast_parse_binary_op_bool_or,
        ast_parse_bool_and_expr
    );
)

// BoolAndExpr
//      CompareExpr (KEYWORD(and) CompareExpr)
static AstNode* ast_parse_bool_and_expr(Parser* parser) {
    return ast_parse_binary_op_expr(
        parser, 
        BinaryOpChainInfinity,
        ast_parse_binary_op_bool_and,
        ast_parse_compare_expr
    );
}

// CompareExpr
//     BitwiseExpr (CompareOp BitWiseExpr)?
static AstNode* ast_parse_compare_expr(Parser* pc) {
    return ast_parse_bin_op_expr(
        parser, 
        BinOpChainOnce, 
        ast_parse_compare_op, 
        ast_parse_bitwise_expr
    );
}

// BitwiseExpr
//      BitShiftExpr (BitwiseOp BitShiftExpr)*
static AstNode* ast_parse_bitwise_expr(Parser* pc) {
    return ast_parse_bin_op_expr(
        parser, 
        BinOpChainInf, 
        ast_parse_bitwise_op, 
        ast_parse_bit_shift_expr
    );
}

// BitShiftExpr <- AdditionExpr (BitShiftOp AdditionExpr)*
static AstNode* ast_parse_bit_shift_expr(Parser* parser) {
    return ast_parse_bin_op_expr(
        parser, 
        BinOpChainInf, 
        ast_parse_bit_shift_op, 
        ast_parse_addition_expr
    );
}

// AdditionExpr
//      MultiplyExpr (AdditionOp MultiplyExpr)*
static AstNode* ast_parse_addition_expr(Parser* parser) {
    return ast_parse_bin_op_expr(
        parser, 
        BinOpChainInf, 
        ast_parse_addition_op, a
        st_parse_multiply_expr
    );
}

// MultiplyExpr
//      PrefixExpr (MultiplyOp PrefixExpr)*
static AstNode* ast_parse_multiply_expr(Parser* parser) {
    return ast_parse_bin_op_expr(
        parser, 
        BinOpChainInf, 
        ast_parse_multiply_op, 
        ast_parse_prefix_expr
    );
}

// PrefixExpr
//      PrefixOp* PrimaryExpr
static AstNode* ast_parse_prefix_expr(Parser* parser) {
    return ast_parse_prefix_op_expr(
        parser,
        ast_parse_prefix_op,
        ast_parse_primary_expr
    );
}


// Primary Expression can be one of:
//      | IfExpr
//      | KEYWORD(break) BreakLabel? Expr?
//      | KEYWORD(continue) BreakLabel?
//      | KEYWORD(return) Expr?
//      | BlockLabel? LoopExpr
//      | Block
static AstNode* ast_parse_primary_expr(Parser* parser) {
    AstNode* if_expr = ast_parse_if_expr(parser);
    if(if_expr != null)
        return if_expr;

    Token* break_token = parser_chomp_if(parser, BREAK);
    if(break_token != null) {
        free(break_token);
        Token* label = ast_parse_break_label(parser);
        AstNode* expr = ast_parse_expr(parser);
        
        AstNode* out = ast_create_node(AstNodeKindBreak);
        out->data.stmt->branch_stmt->name = label->value;
        out->data.stmt->branch_stmt->type = AstNodeBranchStatementBreak;
        out->data.stmt->branch_stmt->expr = expr;
        return out;
    }
    
    Token* continue_token = parser_chomp_if(parser, CONTINUE);
    if(continue_token != null) {
        Token* label = ast_parse_break_label(parser);
        AstNode* out = ast_create_node(AstNodeKindContinue);
        out->data.stmt->branch_stmt->name = label == null ? label->value : null;
        out->data.stmt->branch_stmt->type = AstNodeBranchStatementContinue;
    }

    Token* return_token = parser_chomp_if(parser, RETURN);
    if(return_token != null) {
        free(return_token);
        AstNode* expr = ast_parse_expr(parser);
        AstNode* out = ast_create_node(AstNodeKindReturn);
        out->data.stmt->return_stmt->expr = expr;
        return out;
    }

    AstNode* block = ast_parse_block(parser);
    if(block != null)
        return block;
    
    return null;
}

// IfPrefix
//      IfPrefix Expr (KEYWORD(else) Expr)

// IfExpr <- IfPrefix Expr (KEYWORD_else Payload? Expr)?
static AstNode* ast_parse_if_expr(Parser* parser) {
    return ast_parse_if_expr_helper(parser, ast_parse_expr);
}

// Block <- LBRACE Statement* RBRACE
static AstNode* ast_parse_block(Parser* parser) {
    Token* lbrace = parser_chomp_if(parser, TokenIdLBrace);
    if(lbrace == 0)
        return null;

    ZigList<AstNode* > statements = {};
    AstNode* statement;
    while((statement = ast_parse_statement(parser)) != null)
        statements.append(statement);

    expect_token(parser, RBRACE);

    AstNode* out = ast_create_node(parser, AstNodeKindBlock, lbrace);
    out->data.block.statements = statements;
    return out;
}

// LoopExpr <- KEYWORD_inline? (ForExpr / WhileExpr)
static AstNode* ast_parse_loop_expr(Parser* parser) {
    return ast_parse_loop_expr_helper(
        pc,
        ast_parse_for_expr,
        ast_parse_while_expr
    );
}

// ForExpr <- ForPrefix Expr (KEYWORD_else Expr)?
static AstNode* ast_parse_for_expr(Parser* parser) {
    return ast_parse_for_expr_helper(
        parser, 
        ast_parse_expr
    );
}

// WhileExpr <- WhilePrefix Expr (KEYWORD_else Payload? Expr)?
static AstNode* ast_parse_while_expr(Parser* parser) {
    return ast_parse_while_expr_helper(
        parser, 
        ast_parse_expr
    );
}

// CurlySuffixExpr <- TypeExpr InitList?
static AstNode* ast_parse_curly_suffix_expr(Parser* parser) {
    AstNode* type_expr = ast_parse_type_expr(parser);
    if(type_expr == null)
        return null;

    AstNode* out = ast_parse_init_list(parser);
    if(out == null)
        return type_expr;

    assert(out->type == AstNodeKindContainerInitExpr);
    out->data.container_init_expr.type = type_expr;
    return out;
}

// InitList
//      | LBRACE FieldInit (COMMA FieldInit)* COMMA? RBRACE
//      | LBRACE Expr (COMMA Expr)* COMMA? RBRACE
//      | LBRACE RBRACE
static AstNode* ast_parse_init_list(Parser* parser) {
    Token* lbrace = parser_chomp_if(parser, TokenIdLBrace);
    if(lbrace == 0)
        return null;

    AstNode* first = ast_parse_field_init(parser);
    if(first != null) {
        AstNode* out = ast_create_node(parser, AstNodeKindContainerInitExpr, lbrace);
        out->data.container_init_expr.kind = ContainerInitKindStruct;
        out->data.container_init_expr.entries.append(first);

        while(parser_chomp_if(parser, COMMA) != 0) {
            AstNode* field_init = ast_parse_field_init(parser);
            if(field_init == null)
                break;
            out->data.container_init_expr.entries.append(field_init);
        }

        expect_token(parser, RBRACE);
        return out;
    }

    AstNode* out = ast_create_node(parser, AstNodeKindContainerInitExpr, lbrace);
    out->data.container_init_expr.kind = ContainerInitKindArray;

    first = ast_parse_expr(parser);
    if(first != null) {
        out->data.container_init_expr.entries.append(first);

        while(parser_chomp_if(parser, COMMA) != 0) {
            AstNode* expr = ast_parse_expr(parser);
            if(expr == null)
                break;
                
            out->data.container_init_expr.entries.append(expr);
        }

        expect_token(parser, RBRACE);
        return out;
    }

    expect_token(parser, RBRACE);
    return out;
}

// TypeExpr <- PrefixTypeOp* ErrorUnionExpr
static AstNode* ast_parse_type_expr(Parser* parser) {
    return ast_parse_prefix_op_expr(
        parser,
        ast_parse_prefix_type_op,
        ast_parse_error_union_expr
    );
}

// ErrorUnionExpr <- SuffixExpr (EXCLAMATIONMARK TypeExpr)?
static AstNode* ast_parse_error_union_expr(Parser* parser) {
    AstNode* out = ast_parse_suffix_expr(pc);
    if(out == null)
        return null;

    AstNode* op = ast_parse_bin_op_hash_errorunion(parser)
    if(op == null)
        return out;

    AstNode* right = ast_parse_type_expr(parser);
    assert(op->type == AstNodeKindBinaryOpExpr);
    op->data.bin_op_expr.lhs = out;
    op->data.bin_op_expr.rhs = right;
    return op;
}

// SuffixExpr
//      | PrimaryTypeExpr (SuffixOp / FuncCallArguments)*
static AstNode* ast_parse_suffix_expr(Parser* parser) {
    AstNode* out = ast_parse_primary_type_expr(pc);
    if(out == null)
        return null;

    while(true) {
        AstNode* suffix = ast_parse_suffix_op(pc);
        if(suffix != null) {
            switch (suffix->type) {
                case AstNodeKindSliceExpr:
                    suffix->data.slice_expr.array_ref_expr = out;
                    break;
                case AstNodeKindArrayAccessExpr:
                    suffix->data.array_access_expr.array_ref_expr = out;
                    break;
                case AstNodeKindFieldAccessExpr:
                    suffix->data.field_access_expr.struct_expr = out;
                    break;
                case AstNodeKindUnwrapOptional:
                    suffix->data.unwrap_optional.expr = out;
                    break;
                case AstNodeKindPtrDeref:
                    suffix->data.ptr_deref_expr.target = out;
                    break;
                default:
                    unreachable();
            }
            out = suffix;
            continue;
        }

        AstNode* call = ast_parse_fn_call_arguments(pc);
        if(call != null) {
            assert(call->type == AstNodeKindFnCallExpr);
            call->data.fn_call_expr.fn_ref_expr = out;
            out = call;
            continue;
        }
        break;
    }

    return out;

}

// PrimaryTypeExpr
//     <- BUILTINIDENTIFIER FuncCallArguments
//      / CHAR_LITERAL
//      / ContainerDecl
//      / DOT IDENTIFIER
//      / ErrorSetDecl
//      / FLOAT
//      / FnProto
//      / GroupedExpr
//      / LabeledTypeExpr
//      / IDENTIFIER
//      / IfTypeExpr
//      / INTEGER
//      / KEYWORD_comptime TypeExpr
//      / KEYWORD_error DOT IDENTIFIER
//      / KEYWORD_false
//      / KEYWORD_null
//      / KEYWORD_promise
//      / KEYWORD_true
//      / KEYWORD_undefined
//      / KEYWORD_unreachable
//      / STRINGLITERAL
//      / SwitchExpr
static AstNode* ast_parse_primary_type_expr(Parser* parser) {
    Token* builtin_tok = parser_chomp_if(pc, TokenIdBuiltin);
    if(builtin_tok != 0) {
        AstNode* out = ast_parse_fn_call_arguments(parser);
        AstNode* name_sym = ast_create_node(pc, AstNodeKindIdentifier, builtin_tok);

        assert(out->type == AstNodeKindFnCallExpr);
        out->main_token = builtin_tok;
        out->data.fn_call_expr.fn_ref_expr = name_sym;
        out->data.fn_call_expr.modifier = CallModifierBuiltin;
        return out;
    }

    Token* char_lit = parser_chomp_if(pc, TokenIdCharLiteral);
    if(char_lit != 0) {
        return ast_create_node(pc, AstNodeKindCharLiteral, char_lit);
    }

    AstNode* container_decl = ast_parse_container_decl(pc);
    if(container_decl != null)
        return container_decl;

    AstNode* anon_lit = ast_parse_anon_lit(pc);
    if(anon_lit != null)
        return anon_lit;

    AstNode* error_set_decl = ast_parse_error_set_decl(pc);
    if(error_set_decl != null)
        return error_set_decl;

    Token* float_lit = parser_chomp_if(pc, TokenIdFloatLiteral);
    if(float_lit != 0) {
        return ast_create_node(pc, AstNodeKindFloatLiteral, float_lit);
    }

    AstNode* fn_proto = ast_parse_fn_proto(pc);
    if(fn_proto != null)
        return fn_proto;

    AstNode* grouped_expr = ast_parse_grouped_expr(pc);
    if(grouped_expr != null)
        return grouped_expr;

    AstNode* labeled_type_expr = ast_parse_labeled_type_expr(pc);
    if(labeled_type_expr != null)
        return labeled_type_expr;

    Token* identifier = parser_chomp_if(pc, TokenIdIdentifier);
    if(identifier != 0)
        return token_identifier(pc, identifier);

    AstNode* if_type_expr = ast_parse_if_type_expr(pc);
    if(if_type_expr != null)
        return if_type_expr;

    Token* int_lit = parser_chomp_if(pc, TokenIdIntLiteral);
    if(int_lit != 0) {
        return ast_create_node(pc, AstNodeKindIntLiteral, int_lit);
    }

    Token* comptime = parser_chomp_if(pc, TokenIdKeywordCompTime);
    if(comptime != 0) {
        AstNode* expr = ast_parse_type_expr(parser);
        AstNode* out = ast_create_node(pc, AstNodeKindCompTime, comptime);
        out->data.comptime_expr.expr = expr;
        return out;
    }

    Token* error = parser_chomp_if(pc, TokenIdKeywordError);
    if(error != 0) {
        Token* dot = expect_token(pc, TokenIdDot);
        Token* name = expect_token(pc, TokenIdIdentifier);
        AstNode* left = ast_create_node(pc, AstNodeKindErrorType, error);
        AstNode* out = ast_create_node(pc, AstNodeKindFieldAccessExpr, dot);
        out->data.field_access_expr.struct_expr = left;
        out->data.field_access_expr.field_name = token_buf(pc, name);
        return out;
    }

    Token* false_token = parser_chomp_if(pc, TokenIdKeywordFalse);
    if(false_token != 0) {
        AstNode* out = ast_create_node(pc, AstNodeKindBoolLiteral, false_token);
        out->data.bool_literal.value = false;
        return out;
    }

    Token* null_kwd = parser_chomp_if(pc, TokenIdKeywordNull);
    if(null_kwd != 0)
        return ast_create_node(pc, AstNodeKindNullLiteral, null);

    Token* anyframe = parser_chomp_if(pc, TokenIdKeywordAnyFrame);
    if(anyframe != 0)
        return ast_create_node(pc, AstNodeKindAnyFrameType, anyframe);

    Token* true_token = parser_chomp_if(pc, TokenIdKeywordTrue);
    if(true_token != 0) {
        AstNode* out = ast_create_node(pc, AstNodeKindBoolLiteral, true_token);
        out->data.bool_literal.value = true;
        return out;
    }

    Token* undefined = parser_chomp_if(pc, TokenIdKeywordUndefined);
    if(undefined != 0)
        return ast_create_node(pc, AstNodeKindUndefinedLiteral, undefined);

    Token* unreachable = parser_chomp_if(pc, TokenIdKeywordUnreachable);
    if(unreachable != 0)
        return ast_create_node(pc, AstNodeKindUnreachable, unreachable);


    Token* string_lit = parser_chomp_if(pc, TokenIdStringLiteral);
    if(string_lit != 0) {
        return ast_create_node(pc, AstNodeKindStringLiteral, string_lit);
    }

    Token* multiline_str_lit = ast_parse_multi_tok(pc, TokenIdMultilineStringLiteralLine);
    if(multiline_str_lit != 0) {
        return ast_create_node(pc, AstNodeKindStringLiteral, multiline_str_lit);
    }

    AstNode* switch_expr = ast_parse_switch_expr(pc);
    if(switch_expr != null)
        return switch_expr;

    return null;
}

// ContainerDecl <- (KEYWORD_extern / KEYWORD_packed)? ContainerDeclAuto
static AstNode* ast_parse_container_decl(Parser* parser) {
    Token* layout_token = parser_chomp_if(pc, TokenIdKeywordExtern);
    if(layout_token == 0)
        layout_token = parser_chomp_if(pc, TokenIdKeywordPacked);

    AstNode* out = ast_parse_container_decl_auto(pc);
    if(res == null) {
        if(layout_token != 0)
            put_back_token(pc);
        return null;
    }

    assert(res->type == AstNodeKindContainerDecl);
    if(layout_token != 0) {
        res->main_token = layout_token;
        res->data.container_decl.layout = pc->token_ids[layout_token] == TokenIdKeywordExtern
            ? ContainerLayoutExtern
            : ContainerLayoutPacked;
    }
    return res;
}

// ErrorSetDecl <- KEYWORD_error LBRACE IdentifierList RBRACE
static AstNode* ast_parse_error_set_decl(Parser* parser) {
    Token* first = parser_chomp_if(pc, TokenIdKeywordError);
    if(first == 0)
        return null;
    if(parser_chomp_if(pc, TokenIdLBrace) == 0) {
        put_back_token(pc);
        return null;
    }

    ZigList<AstNode* > decls = ast_parse_list<AstNode>(pc, TokenIdComma, [](ParseContext *context) {
        Token* doc_token = ast_parse_doc_comments(context);
        Token* ident = parser_chomp_if(context, TokenIdIdentifier);
        if(ident == 0)
            return (AstNode*)null;

        AstNode* symbol_node = token_identifier(context, ident);
        if(doc_token == 0)
            return symbol_node;

        AstNode* field_node = ast_create_node(context, AstNodeKindErrorSetField, doc_token);
        field_node->data.err_set_field.field_name = symbol_node;
        field_node->data.err_set_field.doc_comments = doc_token;
        return field_node;
    });
    expect_token(pc, TokenIdRBrace);

    AstNode* out = ast_create_node(pc, AstNodeKindErrorSetDecl, first);
    res->data.err_set_decl.decls = decls;
    return res;
}

// GroupedExpr <- LPAREN Expr RPAREN
static AstNode* ast_parse_grouped_expr(Parser* parser) {
    Token* lparen = parser_chomp_if(pc, TokenIdLParen);
    if(lparen == 0)
        return null;

    AstNode* expr = ast_parse_expr(parser);
    expect_token(pc, TokenIdRParen);

    AstNode* out = ast_create_node(pc, AstNodeKindGroupedExpr, lparen);
    res->data.grouped_expr = expr;
    return res;
}

// IfTypeExpr <- IfPrefix TypeExpr (KEYWORD_else Payload? TypeExpr)?
static AstNode* ast_parse_if_type_expr(Parser* parser) {
    return ast_parse_if_expr_helper(pc, ast_parse_type_expr);
}

// LabeledTypeExpr
//     <- BlockLabel Block
//      / BlockLabel? LoopTypeExpr
static AstNode* ast_parse_labeled_type_expr(Parser* parser) {
    Token* label = ast_parse_block_label(pc);
    if(label != 0) {
        AstNode* block = ast_parse_block(pc);
        if(block != null) {
            assert(block->type == AstNodeKindBlock);
            block->data.block.name = token_buf(pc, label);
            return block;
        }
    }

    AstNode* loop = ast_parse_loop_type_expr(pc);
    if(loop != null) {
        switch (loop->type) {
            case AstNodeKindForExpr:
                loop->data.for_expr.name = token_buf(pc, label);
                break;
            case AstNodeKindWhileExpr:
                loop->data.while_expr.name = token_buf(pc, label);
                break;
            default:
                unreachable();
        }
        return loop;
    }

    if(label != 0) {
        put_back_token(pc);
        put_back_token(pc);
    }
    return null;
}

// LoopTypeExpr <- KEYWORD_inline? (ForTypeExpr / WhileTypeExpr)
static AstNode* ast_parse_loop_type_expr(Parser* parser) {
    return ast_parse_loop_expr_helper(
        pc,
        ast_parse_for_type_expr,
        ast_parse_while_type_expr
    );
}

// ForTypeExpr <- ForPrefix TypeExpr (KEYWORD_else TypeExpr)?
static AstNode* ast_parse_for_type_expr(Parser* parser) {
    return ast_parse_for_expr_helper(pc, ast_parse_type_expr);
}

// WhileTypeExpr <- WhilePrefix TypeExpr (KEYWORD_else Payload? TypeExpr)?
static AstNode* ast_parse_while_type_expr(Parser* parser) {
    return ast_parse_while_expr_helper(pc, ast_parse_type_expr);
}

// SwitchExpr <- KEYWORD_switch LPAREN Expr RPAREN LBRACE SwitchProngList RBRACE
static AstNode* ast_parse_switch_expr(Parser* parser) {
    Token* switch_token = parser_chomp_if(pc, TokenIdKeywordSwitch);
    if(switch_token == 0)
        return null;

    expect_token(pc, TokenIdLParen);
    AstNode* expr = ast_parse_expr(parser);
    expect_token(pc, TokenIdRParen);
    expect_token(pc, TokenIdLBrace);
    ZigList<AstNode* > prongs = ast_parse_list(pc, TokenIdComma, ast_parse_switch_prong);
    expect_token(pc, TokenIdRBrace);

    AstNode* out = ast_create_node(pc, AstNodeKindSwitchExpr, switch_token);
    res->data.switch_expr.expr = expr;
    res->data.switch_expr.prongs = prongs;
    return res;
}
