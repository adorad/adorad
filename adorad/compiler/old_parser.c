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
#define pc  parser->curr_tok
#define ast_error(...)              panic(ErrorParseError, __VA_ARGS__)
#define parser_chomp()              chomp(parser)
#define parser_chomp_if(kind)       chomp_if(parser, kind)
#define parser_expect_token(kind)   expect_token(parser, kind)
#define CURR_TOK_KIND               parser->curr_tok->kind

// Initialize a new Parser
Parser* parser_init(Lexer* lexer) {
    Parser* parser = cast(Parser*)calloc(1, sizeof(Parser));
    parser->lexer = lexer;
    parser->toklist = lexer->toklist;
    parser->curr_tok = cast(Token*)vec_at(parser->toklist, 0);
    parser->offset = 0;
    parser->num_tokens = vec_size(parser->toklist);
    parser->num_lines = 0;
    parser->mod_name = null;
    return parser;
}

inline Token* parser_peek_token(Parser* parser) {
    return parser->curr_tok;
}

inline Token* parser_peek_next(Parser* parser) {
    if(parser->offset + 1 >= parser->num_tokens)
        return null;

    return cast(Token*)parser->toklist + parser->offset + 1;
}

// Consumes a token and moves on to the next token
inline Token* chomp(Parser* parser) {
    if(parser->offset >= parser->num_tokens)
        return null;

    return cast(Token*)parser->toklist + parser->offset++;
}

// Consumes a token and moves on to the next, if the current token matches the expected token.
inline Token* chomp_if(Parser* parser, TokenKind tokenkind) {
    if(parser->curr_tok->kind == tokenkind)
        return parser_chomp();

    return null;
}

inline void parser_put_back(Parser* parser) {
    if(parser->offset == 0)
        unreachable();
    parser->curr_tok -= 1;
    parser->offset -= 1;
}

inline Token* expect_token(Parser* parser, TokenKind tokenkind) {
    if(parser->curr_tok->kind == tokenkind)
        return parser_chomp();
        
    panic(ErrorUnexpectedToken, "Expected `%s`; got `%s`", 
                                        tokenHash[tokenkind],
                                        tokenHash[parser->curr_tok->kind]);
    abort();
}

AstNode* ast_create_node(AstNodeKind kind) {
    AstNode* node = cast(AstNode*)calloc(1, sizeof(AstNode));
    node->kind = kind;
    return node;
}

/*
    A large part of the Parser from this point onwards has been selfishly inspired by Zig's Compiler.

    Before, we release the first stable version of Adorad, this parser implementation will be reworked and improved.

    Related source code: https://github.com/ziglang/zig/blob/master/src/stage1/parser.cpp
*/
typedef enum BinaryOpChain {
    BinaryOpChainOnce,
    BinaryOpChainInfinity
} BinaryOpChain;

static AstNode* ast_parse_func_call_args(Parser* parser);
static AstNode* ast_parse_suffix_op(Parser* parser);
static AstNode* ast_parse_prefix_op_expr(Parser* parser,
                                         AstNode* (*op_parser)(Parser*),
                                         AstNode* (*child_parser)(Parser*));
static AstNode* ast_parse_prefix_type_op(Parser* parser);
static AstNode* ast_parse_prefix_op(Parser* parser);
static AstNode* ast_parse_multiplication_op(Parser* parser);
static AstNode* ast_parse_addition_op(Parser* parser);
static AstNode* ast_parse_bitshift_op(Parser* parser);
static AstNode* ast_parse_bitwise_op(Parser* parser);
static AstNode* ast_parse_comparison_op(Parser* parser);
static AstNode* ast_parse_assignment_op(Parser* parser);
static AstNode* ast_parse_op(Parser* parser);
static AstNode* ast_parse_match_item(Parser* parser);
static AstNode* parse_match_case_kwd(Parser* parser);
static AstNode* parse_match_branch(Parser* parser);
static Token* parse_block_label(Parser* parser);
static Token* parse_break_label(Parser* parser);
static AstNode* parse_match_expr(Parser* parser);
static AstNode* parse_primary_type_expr(Parser* parser);
static AstNode* parse_suffix_expr(Parser* parser);
static AstNode* parse_type_expr(Parser* parser);
static AstNode* parse_init_list(Parser* parser);
static AstNode* parse_if_expr(Parser* parser);
static AstNode* parse_boolean_and_op(Parser* parser);
static AstNode* parse_boolean_or_op(Parser* parser);
static AstNode* parse_primary_expr(Parser* parser);
static AstNode* ast_parse_prefix_expr(Parser* parser);
static AstNode* ast_parse_multiplication_expr(Parser* parser);
static AstNode* parse_addition_expr(Parser* parser);
static AstNode* ast_parse_bitshift_expr(Parser* parser);
static AstNode* ast_parse_bitwise_expr(Parser* parser);
static AstNode* ast_parse_comparison_expr(Parser* parser);
static AstNode* ast_parse_boolean_or_expr(Parser* parser);
static AstNode* ast_parse_boolean_and_expr(Parser* parser);
static AstNode* ast_parse_expr(Parser* parser);
static AstNode* ast_parse_try_expr(Parser* parser);
static AstNode* ast_parse_binary_op_expr(Parser* parser, BinaryOpChain chain,
                                         AstNode* (*op_parser)(Parser*),
                                         AstNode* (*child_parser)(Parser*)
);
static BinaryOpKind tokenkind_to_binaryopkind(TokenKind kind);
static AstNode* ast_parse_block(Parser* parser);
static AstNode* ast_parse_assignment_expr(Parser* parser);
static AstNode* ast_parse_block_expr(Parser* parser);
static AstNode* ast_parse_block_expr_statement(Parser* parser);
static AstNode* ast_parse_loop_statement(Parser* parser);
static AstNode* ast_parse_labeled_statements(Parser* parser);
static AstNode* ast_parse_if_prefix(Parser* parser);
static AstNode* ast_parse_statement(Parser* parser);
static AstNode* ast_parse_var_decl(Parser* parser);
static AstNode* ast_parse_func_prototype(Parser* parser);

static Vec* ast_parse_param_list(Parser* parser, AstNode* (*param_parser)(Parser* parser)) {
    Vec* out = vec_new(AstNode, 1);
    while(true) {
        AstNode* curr = param_parser(parser);
        if(curr == null)
            break;
        vec_push(out, curr);

        Token* sep = parser_chomp_if(COMMA);
        if(sep == null)
            break;
    }
    return out;
}

// General format:
//      KEYWORD(func) IDENT LPAREN ParamDeclList RPAREN LARROW RETURNTYPE
static AstNode* ast_parse_func_prototype(Parser* parser) {
    Token* func = parser_chomp_if(FUNC);
    if(func == null)
        return null;
    
    Token* identifier = parser_chomp_if(IDENTIFIER);
    Token* lparen = parser_expect_token(LPAREN);
    Vec* params = ast_parse_param_list(parser, ast_parse_match_branch);
    Token* rparen = parser_expect_token(RPAREN);

    AstNode* return_type = ast_parse_type_expr(parser);
    if(return_type == null) {
        Token* next = parser_peek_token(parser);
        ast_error(
            "expected return type; found`%s`",
            tokenHash[next->kind]
        );
    }

    AstNode* out = ast_create_node(AstNodeKindFuncPrototype);
    // out->data.stmt->func_proto_decl->name = identifier->value;
    // out->data.stmt->func_proto_decl->params = params;
    // out->data.stmt->func_proto_decl->return_type = return_type;

    for(UInt64 i = 0; i < vec_size(params); i++) {
        AstNode* param_decl = vec_at(params, i);
        CORETEN_ENFORCE(param_decl->kind == AstNodeKindParamDecl);
        // if(param_decl->data.param_decl->is_var_args)
        //     out->data.stmt->func_proto_decl->is_var_args = true;
        
        // Check for multiple variadic arguments in prototype
        // Adorad supports only 1
    //     if(i != vec_size(params) - 1 && out->data.stmt->func_proto_decl->is_var_args)
    //         ast_error(
    //             "Cannot have multiple variadic arguments in function prototype"
    //         );
    }
    return out;
}

// General format:
// `?` represents optional
//      KEYWORD(export)? KEYWORD(mutable/const)? TypeExpr? IDENTIFIER EQUAL? Expr?
static AstNode* ast_parse_var_decl(Parser* parser) {
    Token* export_kwd = parser_chomp_if(EXPORT);
    Token* mutable_kwd = parser_chomp_if(MUTABLE);
    Token* const_kwd = parser_chomp_if(CONST);
    if(mutable_kwd && const_kwd)
        ast_error("Cannot decorate a variable as both `mutable` and `const`");

    AstNode* type_expr = ast_parse_type_expr(parser);
    Token* identifier = parser_expect_token(IDENTIFIER);
    Token* equals = parser_chomp_if(EQUALS);
    AstNode* expr;
    if(equals != null)
        expr = ast_parse_expr(parser);
    
    parser_expect_token(SEMICOLON); // TODO: Remove this need

    AstNode* out = ast_create_node(AstNodeKindVariableDecl);
    out->data.stmt->var_decl->name = identifier->value;
    out->data.stmt->var_decl->visibility = export_kwd != null ? VisibilityModePublic : VisibilityModePrivate;;
    out->data.stmt->var_decl->is_mutable = mutable_kwd != null;
    out->data.stmt->var_decl->is_const = const_kwd != null;
    out->data.stmt->var_decl->expr = expr;
    return out;
}

// Statements
static AstNode* ast_parse_statement(Parser* parser) {
    AstNode* var_decl = ast_parse_var_decl(parser);
    if(var_decl != null) {
        CORETEN_ENFORCE(var_decl->kind == AstNodeKindVariableDecl);
        return var_decl;
    }

    // Defer
    Token* defer_stmt = parser_chomp_if(DEFER);
    if(defer_stmt != null) {
        AstNode* statement = ast_parse_block_expr_statement(parser);
        AstNode* out = ast_create_node(AstNodeKindDefer);
        
        out->data.stmt->defer_stmt->expr = statement;
        return out;
    }

    // If statement
    AstNode* if_statement = ast_parse_if_expr(parser);
    if(if_statement != null)
        return if_statement;
    
    // Labeled Statements
    AstNode* labeled_statement = ast_parse_labeled_statements(parser);
    if(labeled_statement != null)
        return labeled_statement;

    // Match statements
    AstNode* match_expr = ast_parse_match_expr(parser);
    if(match_expr != null)
        return match_expr;

    // Assignment statements
    AstNode* assignment_expr = ast_parse_assignment_expr(parser);
    if(assignment_expr != null)
        return assignment_expr;

    return null;
}

static AstNode* ast_parse_if_prefix(Parser* parser) {
    Token* if_kwd = parser_chomp_if(IF);
    if(if_kwd == null) {
        return null;
    }
    Token* lparen = parser_expect_token(LPAREN);
    AstNode* condition = ast_parse_expr(parser);
    Token* rparen = parser_expect_token(RPAREN);

    AstNode* out = ast_create_node(AstNodeKindIfExpr);
    out->data.expr->if_expr->condition = condition;

    return out;
}

static AstNode* ast_parse_if_expr(Parser* parser) {
    AstNode* out = ast_parse_if_prefix(parser);
    if(out == null) {
        return null;
    }
    
    AstNode* body = ast_parse_block_expr(parser);
    if(body == null)
        body = ast_parse_assignment_expr(parser);
    
    if(body == null) {
        Token* token = parser_chomp();
        ast_error(
            "expected `if` body; found `%s`",
            tokenHash[token->kind]
        );
    }

    AstNode* else_body = null;
    Token* else_kwd = parser_chomp_if(ELSE);
    if(else_kwd != null)
        else_body = ast_parse_statement(parser);

    out->data.expr->if_expr->if_body = body;
    out->data.expr->if_expr->has_else = else_body != null;
    out->data.expr->if_expr->else_node = else_body;
    return out;
}

// Labeled Statements
static AstNode* ast_parse_labeled_statements(Parser* parser) {
    Token* label = ast_parse_block_label(parser);
    AstNode* block = ast_parse_block(parser);
    if(block != null) {
        CORETEN_ENFORCE(block->kind == AstNodeKindBlock);
        block->data.stmt->block_stmt->name = label->value;
        return block;
    }

    AstNode* loop = ast_parse_loop_statement(parser);
    if(loop != null) {
        loop->data.expr->loop_expr->label = label->value;
        return loop;
    }

    if(label != null)
        panic(
            ErrorUnexpectedToken,
            "invalid token: `%s`",
            parser_peek_token(parser)->value->data
        );
        
    return null;
}

// Loops
//      (KEYWORD(inline))? loop ... {  }
static AstNode* parse_loop_statement(Parser* parser) {
    Token* inline_token = parser_chomp_if(ATTR_INLINE);

    CORETEN_ENFORCE(false);
    // TODO

    // AstNode* loop_c_statement = ast_parse_loop_c_statement(parser);
    // if(loop_c_statement != null) {
    //     CORETEN_ENFORCE(loop_c_statement->kind == AstNodeKindLoopCExpr);
    //     loop_c_statement->data.expr->loop_expr->loop_c_expr->is_inline = inline_token != null;
    //     return loop_c_statement;
    // }

    // AstNode* loop_while_statement = ast_parse_loop_while_statement(parser);
    // if(loop_while_statement != null) {
    //     CORETEN_ENFORCE(loop_while_statement->kind == AstNodeKindLoopWhileExpr);
    //     loop_while_statement->data.expr->loop_expr->loop_while_expr->is_inline = inline_token != null;
    //     return loop_while_statement;
    // }

    // AstNode* loop_in_statement = ast_parse_loop_in_statement(parser);
    // if(loop_in_statement != null) {
    //     CORETEN_ENFORCE(loop_in_statement->kind == AstNodeKindLoopWhileExpr);
    //     loop_in_statement->data.expr->loop_expr->loop_in_expr->is_inline = inline_token != null;
    //     return loop_in_statement;
    // }

    if(inline_token != null)
        panic(
            ErrorUnexpectedToken,
            "invalid token: `%s`",
            parser_peek_token(parser)->value->data
        );
    
    return null;
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
        Token* semi = parser_expect_token(SEMICOLON);
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
        CORETEN_ENFORCE(out->kind == AstNodeKindBlock);
        out->data.stmt->block_stmt->name = block_label->value;
        return out;
    }

    return ast_parse_block(parser);
}

// AssignmentExpr
//      Expr (AssignmentOp Expr)?
// AssignmentOp can be one of:
//      | MULT_EQUALS       (*=)
//      | SLASH_EQUALS      (/=)
//      | MOD_EQUALS        (%=)
//      | PLUS_EQUALS       (+=)
//      | MINUS_EQUALS      (-=)
//      | LBITSHIFT_EQUALS  (<<=)
//      | RBITSHIFT_EQUALS  (>>=)
//      | AND_EQUALS        (&=)
//      | XOR_EQUALS        (^=)
//      | OR_EQUALS         (|=)
//      | TILDA             (~)
//      | TILDA_EQUALS      (~=)
//      | EQUALS            (=)
static AstNode* ast_parse_assignment_expr(Parser* parser) {
    return ast_parse_binary_op_expr(
        parser,
        BinaryOpChainOnce,
        ast_parse_assignment_op,
        ast_parse_expr
    );
}

static AstNode* ast_parse_block(Parser* parser) {
    Token* lbrace = parser_chomp_if(LBRACE);
    if(lbrace == null)
        return null;

    Vec* statements = vec_new(AstNode, 1);
    AstNode* statement = null;
    while((statement = ast_parse_statement(parser)) != null)
        vec_push(statements, statement);

    Token* rbrace = parser_expect_token(RBRACE);

    AstNode* out = ast_create_node(AstNodeKindBlock);
    out->data.stmt->block_stmt->statements = statements;
    return out;
}

typedef struct ast_prec_table {
    TokenKind tok_kind;
    UInt8 prec_value;
    BinaryOpKind bin_kind;
} ast_prec_table;

// A table of binary operator precedence. Higher precedence numbers are stickier.
static const ast_prec_table precedence_table[] = {
    // { MULT_MULT, 60, BinaryOpKindMultMult  },
    { MULT, 60, BinaryOpKindMult  },
    { MOD, 60, BinaryOpKindMod  },
    { SLASH, 60, BinaryOpKindDiv  },

    { PLUS, 50, BinaryOpKindAdd  },
    { MINUS, 50, BinaryOpKindSubtract  },
    { PLUS_EQUALS, 50, BinaryOpKindAssignmentPlus  },
    { MINUS_EQUALS, 50, BinaryOpKindAssignmentMinus  },

    { LBITSHIFT, 40, BinaryOpKindBitshitLeft  },
    { RBITSHIFT, 40, BinaryOpKindBitshitRight  },
    
    { EQUALS_EQUALS, 30, BinaryOpKindCmpEqual  },
    { EXCLAMATION_EQUALS, 30, BinaryOpKindCmpNotEqual  },
    { GREATER_THAN, 30, BinaryOpKindCmpGreaterThan  },
    { LESS_THAN, 30, BinaryOpKindCmpLessThan  },
    { GREATER_THAN_OR_EQUAL_TO, 30, BinaryOpKindCmpGreaterThanorEqualTo  },
    { LESS_THAN_OR_EQUAL_TO, 30, BinaryOpKindCmpLessThanorEqualTo  },

    { AND, 20, BinaryOpKindBoolAnd  },

    { OR, 10, BinaryOpKindBoolOr  },
};

// Returns the `BinaryOpKind` representation of a `TokenKind`
static BinaryOpKind tokenkind_to_binaryopkind(TokenKind kind) {
    BinaryOpKind value;
    switch(kind) {
        // AdditionOp
        case PLUS: value = BinaryOpKindAdd; break;
        case MINUS: value = BinaryOpKindSubtract; break;
        case MULT: value = BinaryOpKindMult; break;
        case SLASH: value = BinaryOpKindDiv; break;
        case MOD: value = BinaryOpKindMod; break;

        // ComparisonOp
        case GREATER_THAN: value = BinaryOpKindCmpGreaterThan; break;
        case LESS_THAN: value = BinaryOpKindCmpLessThan; break;
        case GREATER_THAN_OR_EQUAL_TO: value = BinaryOpKindCmpGreaterThanorEqualTo; break;
        case LESS_THAN_OR_EQUAL_TO: value = BinaryOpKindCmpLessThanorEqualTo; break;
        case EQUALS_EQUALS: value = BinaryOpKindCmpEqual; break;
        case EXCLAMATION_EQUALS: value = BinaryOpKindCmpNotEqual; break;

        // AssignmentOp
        case PLUS_EQUALS: value = BinaryOpKindAssignmentPlus; break;
        case MINUS_EQUALS: value = BinaryOpKindAssignmentMinus; break;
        case MULT_EQUALS: value = BinaryOpKindAssignmentMult; break;
        case SLASH_EQUALS: value = BinaryOpKindAssignmentDiv; break;
        case MOD_EQUALS: value = BinaryOpKindAssignmentMod; break;
        case AND_EQUALS: value = BinaryOpKindAssignmentBitAnd; break;
        case OR_EQUALS : value = BinaryOpKindAssignmentBitOr; break;
        case XOR_EQUALS: value = BinaryOpKindAssignmentBitXor; break;
        case LBITSHIFT_EQUALS : value = BinaryOpKindAssignmentBitshiftLeft; break;
        case RBITSHIFT_EQUALS : value = BinaryOpKindAssignmentBitshiftRight; break;

        // BitshiftOp
        case LBITSHIFT: value = BinaryOpKindBitshitLeft; break;
        case RBITSHIFT: value = BinaryOpKindBitshitRight; break;

        // BitwiseOp
        case AND: value = BinaryOpKindBitAnd; break;
        case OR: value = BinaryOpKindBitOr; break;
        case XOR: value = BinaryOpKindBitXor; break;

        // We should _never_ reach here
        default: value = BinaryOpKindInvalid; break;
    }

    return value;
}

// A `generic`-like function that parses binary expressions.
// These (expressions) utilize similar functionality, so this function is here to avoid code duplication.
// Here, `op_parser` parses the operand (e.g. `+=`, `or`...)
static AstNode* ast_parse_binary_op_expr(
    Parser* parser,
    BinaryOpChain chain,
    AstNode* (*op_parser)(Parser*),
    AstNode* (*child_parser)(Parser*)
) {
    AstNode* out = child_parser(parser);
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

static AstNode* ast_parse_try_expr(Parser* parser) {
    Token* try_kwd = parser_chomp_if(TRY);
    if(try_kwd != null) {
        AstNode* out = ast_create_node(AstNodeKindReturn);
        out->data.stmt->return_stmt->kind = ReturnKindError;
        return out;
    }
    return cast(AstNode*)null;
}

// Expr
//      KEYWORD(try)* BoolAndExpr
static AstNode* ast_parse_expr(Parser* parser) {
    return ast_parse_prefix_op_expr(
        parser,
        ast_parse_try_expr,
        ast_parse_boolean_and_expr
    );
}

// BooleanAndExpr
static AstNode* ast_parse_boolean_and_expr(Parser* parser) {
    return ast_parse_binary_op_expr(
        parser,
        BinaryOpChainInfinity,
        ast_parse_boolean_and_op,
        ast_parse_boolean_or_expr
    );
}

// BooleanOrExpr
static AstNode* ast_parse_boolean_or_expr(Parser* parser) {
    return ast_parse_binary_op_expr(
        parser,
        BinaryOpChainInfinity,
        ast_parse_boolean_or_op,
        ast_parse_comparison_expr
    );
}

// ComparisonExpr
//      BitwiseExpr (ComparisonOp BitwiseExpr)*
static AstNode* ast_parse_comparison_expr(Parser* parser) {
    return ast_parse_binary_op_expr(
        parser,
        BinaryOpChainOnce,
        ast_parse_comparison_op,
        ast_parse_bitwise_expr
    ); 
}

// BitwiseExpr
//      BitShiftExpr (BitwiseOp BitShiftExpr)*
static AstNode* ast_parse_bitwise_expr(Parser* parser) {
    return ast_parse_binary_op_expr(
        parser,
        BinaryOpChainInfinity,
        ast_parse_bitwise_op,
        ast_parse_bitshift_expr
    ); 
}

// BitShiftExpr
//      AdditionExpr (BitshiftOp AdditionExpr)*
static AstNode* ast_parse_bitshift_expr(Parser* parser) {
    return ast_parse_binary_op_expr(
        parser,
        BinaryOpChainInfinity,
        ast_parse_bitshift_op,
        ast_parse_addition_expr
    ); 
}

// AdditionExpr
//      MultiplyExpr (AdditionOp MultiplyExpr)*
static AstNode* ast_parse_addition_expr(Parser* parser) {
    return ast_parse_binary_op_expr(
        parser,
        BinaryOpChainInfinity,
        parse_addition_op,
        parse_multiplication_expr
    ); 
}

// MultiplyExpr
//      AdditionExpr (MultiplicationOp AdditionExpr)*
static AstNode* ast_parse_multiplication_expr(Parser* parser) {
    return ast_parse_binary_op_expr(
        parser,
        BinaryOpChainInfinity,
        ast_parse_multiplication_op,
        ast_parse_bitwise_expr
    ); 
}

// PrefixExpr
//      PrefixOp? PrimaryExpr
// PrefixOp can be one of:
//      | EXCLAMATION   (!)
//      | MINUS         (-)
//      | TILDA         (~)
//      | AND           (&)
//      | KEYWORD(try) 
static AstNode* ast_parse_prefix_expr(Parser* parser) {
    return ast_parse_prefix_op_expr(
        parser,
        ast_parse_prefix_op,
        ast_parse_primary_expr
    );
}

// PrimaryExpr
//      | IfExpr
//      | KEYWORD(break) BreakLabel? Expr?
//      | KEYWORD(continue) BreakLabel?
//      | ATTRIBUTE Expr
//      | KEYWORD(return) Expr?
//      | BlockLabel? LoopExpr
//      | Block
static AstNode* ast_parse_primary_expr(Parser* parser) {
    AstNode* if_expr = ast_parse_if_expr(parser);
    if (if_expr != null)
        return if_expr;

    Token* break_token = parser_chomp_if(BREAK);
    if(break_token != null) {
        Token* label = ast_parse_break_label(parser);
        AstNode* expr = ast_parse_expr(parser);
        
        AstNode* out = ast_create_node(AstNodeKindBreak);
        out->data.stmt->branch_stmt->name = label->value;
        out->data.stmt->branch_stmt->type = AstNodeBranchStatementBreak;
        out->data.stmt->branch_stmt->expr = expr;
        return out;
    }
    
    Token* continue_token = parser_chomp_if(CONTINUE);
    if(continue_token != null) {
        Token* label = ast_parse_break_label(parser);
        AstNode* out = ast_create_node(AstNodeKindContinue);
        out->data.stmt->branch_stmt->name = label == null ? label->value : null;
        out->data.stmt->branch_stmt->type = AstNodeBranchStatementContinue;
    }

    // Token* attribute = parser_chomp_if(ATTRIBUTE);
    // if (attribute != 0) {
    //     AstNode* expr = ast_parse_expr();
    //     AstNode* out = ast_create_node(AstNodeKindAttribute);
    //     out->data.attribute_expr.expr = expr;
    //     return out;
    // }

    Token* return_token = parser_chomp_if(RETURN);
    if(return_token != null) {
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

static AstNode* ast_parse_boolean_and_op(Parser* parser) {
    Token* op_token = parser_chomp_if(AND);
    if(op_token == null)
        return null;
    
    AstNode* out = ast_create_node(AstNodeKindBinaryOpExpr);
    out->data.expr->binary_op_expr->op = BinaryOpKindBoolAnd;
    return out;
}

static AstNode* ast_parse_boolean_or_op(Parser* parser) {
    Token* op_token = parser_chomp_if(OR);
    if(op_token == null)
        return null;
    
    AstNode* out = ast_create_node(AstNodeKindBinaryOpExpr);
    out->data.expr->binary_op_expr->op = BinaryOpKindBoolOr;
    return out;
}

// TODO
// LoopExpr
// static AstNode* ast_parse_loop_expr(Parser* parser) {
//     return ast_parse_loop_expr_helper(
//         parser,
//         ast_parse_for_expr,
//         ast_parse_while_expr
//     );
// }

// InitList
//      | LBRACE Expr (COMMA Expr)* COMMA? RBRACE
//      | LBRACE RBRACE
static AstNode* ast_parse_init_list(Parser* parser) {
    Token* lbrace = parser_chomp_if(LBRACE);
    if(lbrace == null)
        return null;

    AstNode* out = ast_create_node(AstNodeKindInitExpr);
    out->data.expr->init_expr->kind = InitExprKindArray;
    out->data.expr->init_expr->entries = vec_new(AstNode, 1);

    AstNode* first = ast_parse_expr(parser);
    if(first != null) {
        vec_push(out->data.expr->init_expr->entries, first);

        Token* comma;
        while((comma = parser_chomp_if(COMMA)) != null) {
            AstNode* expr = ast_parse_expr(parser);
            if(expr == null)
                break;
            vec_push(out->data.expr->init_expr->entries, expr);
        }

        Token* rbrace = parser_expect_token(RBRACE);
        return out;
    }
    Token* rbrace = parser_expect_token(RBRACE);
    return out;
}

// TypeExpr
//      PrefixTypeOp* SuffixExpr
static AstNode* ast_parse_type_expr(Parser* parser) {
    return ast_parse_prefix_op_expr(
        parser,
        ast_parse_prefix_type_op,
        ast_parse_suffix_expr
    );
}

// SuffixExpr
//      | PrimaryTypeExpr SuffixOp* FuncCallArgs
//      | PrimaryTypeExpr (SuffixOp / FuncCallArguments)*
static AstNode* ast_parse_suffix_expr(Parser* parser) {
    AstNode* out = ast_parse_primary_type_expr(parser);
    if(out == null)
        return null;
    
    while(true) {
        AstNode* suffix = ast_parse_suffix_op(parser);
        if(suffix != null) { 
            switch(suffix->kind) {
                case AstNodeKindSliceExpr:
                    suffix->data.expr->slice_expr->array_ref_expr = out;
                    break;
                // case AstNodeKindArrayAccessExpr:
                //     suffix->data.array_access_expr->array_ref_expr = out;
                //     break;
                default:
                    unreachable();
            }
            out = suffix;
            continue;
        }

        AstNode* call = ast_parse_func_call_args(parser);
        if(call != null) {
            CORETEN_ENFORCE(call->kind == AstNodeKindFuncCallExpr);
            call->data.expr->func_call_expr->func_call_expr = out;
            out = call;
            continue;
        }
        break;
    } // while(true)

    return out;
}

// PrimaryTypeExpr
//      | CHAR
//      | FLOAT
//      | FuncPrototype
//      | LabeledTypeExpr
//      | IDENT
//      | IfTypeExpr
//      | INTEGER
//      | TypeExpr
//      | KEYWORD(true)
//      | KEYWORD(false)
//      | KEYWORD(null)
//      | KEYWORD(unreachable)
//      | STRING (Literal)
//      | MatchExpr
static AstNode* ast_parse_primary_type_expr(Parser* parser) {
    Token* char_lit = parser_chomp_if(CHAR_LIT);
    if(char_lit != null) {
        return ast_create_node(AstNodeKindCharLiteral);
    }

    Token* float_lit = parser_chomp_if(FLOAT_LIT);
    if(float_lit != null) {
        return ast_create_node(AstNodeKindFloatLiteral);
    }

    AstNode* func_prototype = ast_parse_func_prototype(parser);
    if(func_prototype != null)
        return func_prototype;

    Token* identifier = parser_chomp_if(IDENTIFIER);
    if(identifier != null) {
        return ast_create_node(AstNodeKindIdentifier);
    }

    // Token* if_type_expr = ast_parse_if_type_expr(parser);
    // if(if_type_expr != null)
    //     return if_type_expr;

    Token* int_lit = parser_chomp_if(INTEGER);
    if(int_lit != null) {
        return ast_create_node(AstNodeKindIntLiteral);
    }
    
    Token* true_token = parser_chomp_if(TOK_TRUE);
    if(true_token != null) {
        AstNode* out = ast_create_node(AstNodeKindBoolLiteral);
        out->data.comptime_value->bool_value->value = true;
        return out;
    }

    Token* false_token = parser_chomp_if(TOK_TRUE);
    if(false_token != null) {
        AstNode* out = ast_create_node(AstNodeKindBoolLiteral);
        out->data.comptime_value->bool_value->value = false;
        return out;
    }

    Token* unreachable_token = parser_chomp_if(UNREACHABLE);
    if(unreachable_token != null) {
        return ast_create_node(AstNodeKindUnreachable);
    }

    Token* string_lit = parser_chomp_if(STRING);
    if(string_lit != null) {
        return ast_create_node(AstNodeKindStringLiteral);
    }

    AstNode* match_token = ast_parse_match_expr(parser);
    if(match_token != null)
        return match_token;

    return null;
}

static Vec* ast_parse_branch_list(Parser* parser, AstNode* (*list_parser)(Parser* parser)) {
    Vec* out = vec_new(AstNode, 1);
    while(true) {
        AstNode* curr = list_parser(parser);
        if(curr == null)
            break;
        
        vec_push(out, curr);
        Token* sep = parser_chomp_if(COMMA);
        if(sep == null)
            break;
    }
    return out;
}

// MatchExpr
//      KEYWORD(match) LPAREN? Expr RPAREN? LBRACE MatchBranchList RBRACE
static AstNode* ast_parse_match_expr(Parser* parser) {
    Token* match_token = parser_chomp_if(MATCH);
    if(match_token == null)
        return null;

    // Left and Right Parenthesis' here are optional
    Token* lparen = parser_chomp_if(LPAREN);
    AstNode* expr = parse_expr(parser);
    Token* rparen = parser_chomp_if(RPAREN);

    // These *aren't* optional
    Token* lbrace = parser_expect_token(LBRACE);
    Vec* branches = parse_branch_list(parser,parse_match_branch);
    Token* rbrace = parser_expect_token(RBRACE);

    AstNode* out = ast_create_node(AstNodeKindMatchExpr);
    out->data.expr->match_expr->expr = expr;
    out->data.expr->match_expr->branches = branches;
    return out;
}

// BreakLabel
//      COLON IDENTIFIER
static Token* parse_break_label(Parser* parser) {
    Token* colon = parser_chomp_if(COLON);
    if(colon == null) {
        return null;
    }
    Token* ident = parser_expect_token(IDENTIFIER);
    return ident;
}

// BlockLabel
//      IDENTIFIER COLON
static Token* ast_parse_block_label(Parser* parser) {
    Token* ident = parser_chomp_if(IDENTIFIER);
    if(ident == null)
        return null;
    
    Token* colon = parser_chomp_if(COLON);
    if(colon == null) {
        return colon;
    }

    return ident;
}

// MatchBranch
//      KEYWORD(case) (COLON? / EQUALS_ARROW?) AssignmentExpr
static AstNode* ast_parse_match_branch(Parser* parser) {
    AstNode* out = parse_match_case_kwd(parser);
    CORETEN_ENFORCE(out->kind == AstNodeKindMatchBranch);
    if(out == null)
        return null;
    
    Token* colon = parser_chomp_if(COLON); // `:`
    Token* equals_arrow = parser_chomp_if(EQUALS_ARROW); // `=>`
    if(colon == null && equals_arrow == null)
        ast_error(
            "Missing token after `case`. Either `:` or `=>`"
        );

    AstNode* expr = parse_assignment_expr(parser);
    out->data.expr->match_branch_expr->expr = expr;

    return out;
}

// MatchCase
//      | MatchItem (COMMA MatchItem)* COMMA?
//      | KEYWORD(else)
static AstNode* parse_match_case_kwd(Parser* parser) {
    AstNode* match_item = ast_parse_match_item(parser);
    if(match_item != null) {
        AstNode* out = ast_create_node(AstNodeKindMatchBranch);
        vec_push(out->data.expr->match_branch_expr->branches, match_item);

        Token* comma;
        while((comma = parser_chomp_if(COMMA)) != null) {
            AstNode* item = parse_match_item(parser);
            if(item == null)
                break;
            
            vec_push(out->data.expr->match_branch_expr->branches, item);
            out->data.expr->match_branch_expr->any_branches_are_ranges = cast(bool)(item->kind == AstNodeKindMatchRange);
        }

        return out;
    }

    Token* else_kwd = parser_chomp_if(ELSE);
    if(else_kwd != null) {
        AstNode* out = ast_create_node(AstNodeKindMatchBranch);
        return out;
    }

    return null;
}

// MatchItem
//      Expr (ELLIPSIS Expr)?
static AstNode* parse_match_item(Parser* parser) {
    AstNode* expr = parse_expr(parser);
    if(expr == null)
        return null;
    
    Token* ellipsis = parser_chomp_if(ELLIPSIS);
    if(ellipsis != null) {
        AstNode* expr2 = parse_expr(parser);
        AstNode* out = ast_create_node(AstNodeKindMatchRange);
        out->data.expr->match_range_expr->begin = expr;
        out->data.expr->match_range_expr->end = expr2;
        return out;
    }

    return expr;
}

static AstNode* parse_op(Parser* parser) {
    BinaryOpKind op = tokenkind_to_binaryopkind( parser->curr_tok->kind);

    if(op != BinaryOpKindInvalid) {
        Token* op_token = parser_chomp();
        AstNode* out = ast_create_node(AstNodeKindBinaryOpExpr);
        out->data.expr->binary_op_expr->op = op;
        return out;
    }

    return null;
}

// AssignmentOp can be one of:
//      | PLUS_EQUALS
//      | MINUS_EQUALS
//      | MULT_EQUALS
//      | SLASH_EQUALS
//      | MOD_EQUALS
//      | AND_EQUALS
//      | OR_EQUALS 
//      | XOR_EQUALS
//      | LBITSHIFT_EQUALS
//      | RBITSHIFT_EQUALS
static AstNode* parse_assignment_op(Parser* parser) {
    return parse_op(parser);
}

// ComparisonOp can be one of:
//      | GREATER_THAN
//      | LESS_THAN
//      | GREATER_THAN_OR_EQUAL_TO
//      | LESS_THAN_OR_EQUAL_TO
//      | EQUALS_EQUALS
//      | EXCLAMATION_EQUALS
static AstNode* parse_comparison_op(Parser* parser) {
    return parse_op(parser);
}

// BitwiseOp can be one of:
//      | AND
//      | OR
//      | XOR
static AstNode* parse_bitwise_op(Parser* parser) {
    return parse_op(parser);
}

// BitshiftOp can be one of:
//      | LBITSHIFT
//      | RBITSHIFT
static AstNode* parse_bitshift_op(Parser* parser) {
    return parse_op(parser);
}

// AdditionOp can be one of:
//      | PLUS
//      | MINUS
static AstNode* parse_addition_op(Parser* parser) {
    return parse_op(parser);
}

// MultiplicationOp can be one of:
//      | MULT
//      | SLASH
//      | MOD
static AstNode* parse_multiplication_op(Parser* parser) {
    return parse_op(parser);
}

// PrefixOp can be one of:
//      | EXCLAMATION   (!)
//      | TILDA         (~)
//      | AND           (&)
//      | KEYWORD(try) 
static AstNode* parse_prefix_op(Parser* parser) {
    PrefixOpKind op;
    switch(parser->curr_tok->kind) {
        case NOT: op = PrefixOpKindBoolNot; break;
        case EXCLAMATION: op = PrefixOpKindNegation; break;
        case AND: op = PrefixOpKindAddrOf; break;
        case TRY: op = PrefixOpKindTry; break;
        default: op = PrefixOpKindInvalid; break;
    }

    if(op != PrefixOpKindInvalid) {
        Token* op_token = parser_chomp();
        AstNode* out = ast_create_node(AstNodeKindPrefixOpExpr);
        out->data.prefix_op_expr->op = op;
        return out;
    }

    return null;
}

// PrefixTypeOp
//      | QUESTION
//      | ArrayTypeStart (KEYWORD(const) / KEYWORD(volatile))*
static AstNode* parse_prefix_type_op(Parser* parser) {
    Token* question_mark = parser_chomp_if(QUESTION);
    if(question_mark != null) {
        AstNode* out = ast_create_node(AstNodeKindPrefixOpExpr);
        out->data.prefix_op_expr->op = PrefixOpKindOptional;
        return out;
    }

    Token* arr_init_lbrace = parser_chomp_if(LBRACE);
    Buff* underscore_value = buff_new("_");
    if(arr_init_lbrace != null) {
        Token* underscore = parser_chomp_if(IDENTIFIER);
        if(underscore == null) {
            parser_put_back(parser);
        } else if(!buff_cmp(underscore->value, underscore_value)) {
            parser_put_back(parser);
            parser_put_back(parser);
        } else {
            AstNode* sentinel = null;
            Token* colon = parser_chomp_if(COLON);
            if(colon != null)
                sentinel = ast_parse_expr(parser);
            
            Token* rbrace = parser_expect_token(RBRACE);
            AstNode* out = ast_create_node(AstNodeKindArrayType);
            out->data.inferred_array_type->sentinel = sentinel;
            return out;
        }
    }

    return null;
}

// SuffixOp
//      | LBRACKET Expr (DOT2 (Expr (COLON Expr)?)?)? RBRACKET
//      | DOT IDENTIFIER
static AstNode* parse_suffix_op(Parser* parser) {
    Token* lbrace = parser_chomp_if(LBRACE);
    if(lbrace != null) {
        AstNode* lower = parse_expr(parser);
        AstNode* upper = null;
        Token* ellipsis = parser_chomp_if(ELLIPSIS);
        if(ellipsis != null) {
            AstNode* sentinel = null;
            upper = ast_parse_expr(parser);
            Token* colon = parser_chomp_if(COLON);
            if(colon != null) {
                sentinel = ast_parse_expr(parser);
            }
            Token* rbrace = parser_expect_token(RBRACE);

            AstNode* out = ast_create_node(AstNodeKindSliceExpr);
            out->data.expr->slice_expr->lower = lower;
            out->data.expr->slice_expr->upper = upper;
            out->data.expr->slice_expr->sentinel = sentinel;
            return out;
        }

        Token* rbrace = parser_expect_token(RBRACE);

        AstNode* out = ast_create_node(AstNodeKindArrayAccessExpr);
        out->data.array_access_expr->subscript = lower;
        return out;
    }

    Token* dot = parser_chomp_if(DOT);
    if(dot != null) {
        Token* identifier = parser_expect_token(IDENTIFIER);
        AstNode* out = ast_create_node(AstNodeKindFieldAccessExpr);
        out->data.field_access_expr->field_name = identifier->value;
        return out;
    }

    return null;
}

static AstNode* ast_parse_prefix_op_expr( 
    Parser* parser,
    AstNode* (*op_parser)(Parser*),
    AstNode* (*child_parser)(Parser*)
) {
    AstNode* out = null;
    AstNode** right = &out;
    while(true) {
        AstNode* prefix = op_parser(parser);
        if(prefix == null)
            break;
        
        *right = prefix;
        switch(prefix->kind) {
            case AstNodeKindPrefixOpExpr:
                right = &prefix->data.prefix_op_expr->expr;
                break;
            case AstNodeKindReturn:
                right = &prefix->data.stmt->return_stmt->expr;
                break;
            case AstNodeKindArrayType:
                right = &prefix->data.array_type->child_type;
                break;
            case AstNodeKindInferredArrayType:
                right = &prefix->data.inferred_array_type->child_type;
                break;
            default:
                unreachable();
        }
    }

    // If we've alredy chomped a token, and determned that this node is a prefix op, then we expect that the node
    // has a child
    if(out != null) {
        *right = child_parser(parser);
    } else {
        // Otherwise, if we didn't chomp a token, then we return null if the child expr did.
        *right = child_parser(parser);
        if(*right == null)
            return null;
    }

    return out;
}

// FuncCallArguments
static AstNode* ast_parse_func_call_args(Parser* parser) {
    Token* lparen = parser_chomp_if(LPAREN);
    if(lparen == null)
        return null;
    
    Vec* params = ast_parse_param_list(parser, ast_parse_expr);
    Token* rparen = parser_expect_token(RPAREN);

    AstNode* out = ast_create_node(AstNodeKindFuncCallExpr);
    out->data.expr->func_call_expr->params = params;
    return out;
}

// Main entry point of the Parser.
// It will return the whole AST tree of the entire source code (for each file) when parsed. 
static AstNode* ast_parse(Parser* parser) { 
    return null;
}