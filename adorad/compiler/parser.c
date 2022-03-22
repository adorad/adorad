/*
          _____   ____  _____            _____
    /\   |  __ \ / __ \|  __ \     /\   |  __ \
   /  \  | |  | | |  | | |__) |   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /   / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \  / ____ \| |__| | https://github.com/adorad/adorad/
/_/    \_\_____/ \____/|_|  \_\/_/    \_\_____/

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021-22 Jason Dsouza <@jasmcaus>
*/

/*
    The Adorad Parser.

    Terminologies:
        TODO
    
    Naming Conventions:
        1. parse_x:
            Parse specific portion of the grammar (`x`). Doesn't error out if cannot find `x` (i.e returns null)
        2. expect_x:
            Parse specific portion of the grammar (`x`). Throws an error (chokes and dies) if cannot find `x`

    Operator Precendence:
        TODO
*/

#include <adorad/compiler/ast.h>
#include <adorad/compiler/parser.h>
#include <adorad/core/debug.h>
#include <adorad/core/os.h>

#define pt      parser->toklist
#define pc      parser->curr_tok

#define NODEPUSH(node)          vec_push(parser->nodelist, node)

#define CHOMP(n)                parser_chomp(parser, n)
#define CHOMP_IF(kind)          parser_chomp_if(parser, kind)
#define EXPECT_TOK(kind)        parser_expect_token(parser, kind)

#define AST_ERROR(...)          dread(ErrorParseError, __VA_ARGS__)
#define AST_EXPECTED(...)       AST_ERROR("Expected %s; got `%s`", (__VA_ARGS__), tokenHash[pc->kind])
#define AST_UNEXPECTED(...)     dread(ErrorUnexpectedToken, __VA_ARGS__)

#ifdef ADORAD_DEBUG
    #define TRACE_PARSER()                                                \
        cstlColouredPrintf(                                               \
            CORETEN_COLOUR_WARN,                                          \
            "parsing file: `%s` | curr_tok: `%s` | location: `%d:%d`",    \
            parser->fullpath,                                             \
            tokenHash[parser->curr_tok->kind],                            \
            parser->loc->line,                                            \
            parser->loc->col                                              \
        )
#else
    #define TRACE_PARSER()
#endif // ADORAD_DEBUG

// Initialize a new Parser
Parser* parser_init(Lexer* lexer) {
    Parser* parser = cast(Parser*)calloc(1, sizeof(Parser));
    parser->fullpath = lexer->loc->fname;
    // Generally, the ratio of lexer tokens to parser nodes is about 4:1
    // So, preallocate roughly 25% of the number of lexer tokens
    parser->nodelist = VEC_NEW(AstNode, cast(UInt64)(vec_size(lexer->toklist) * .25));
    parser->lexer = lexer;
    parser->toklist = lexer->toklist;
    parser->curr_tok = cast(Token*)vec_at(parser->toklist, 0);
    parser->offset = 0;
    parser->num_tokens = vec_size(parser->toklist);
    parser->num_lines = 0;
    parser->mod_name = null;
    return parser;
}

static inline Token* parser_peek_next(Parser* parser) {
    if(parser->offset + 1 >= parser->num_tokens)
        return null;

    return cast(Token*)(parser->toklist + parser->offset + 1);
}

// Consumes a token and moves on to the next `n` tokens
static inline Token* parser_chomp(Parser* parser, UInt64 n) {
    if(parser->offset + n >= parser->num_tokens)
        return null;

    parser->offset += n;
    parser->curr_tok += n;
    return cast(Token*)parser->curr_tok;
}

// Expect the current token's kind to match `tokenkind`.
// If it does, move on to the next token, otherwise throw an error.
static inline Token* parser_expect_token(Parser* parser, TokenKind tokenkind) {
    if(pc->kind == tokenkind)
        return CHOMP(1);

    AST_ERROR(
        "Expected `%s`; got `%s`",
        tokenHash[tokenkind], 
        tokenHash[pc->kind]
    );

    return null; // Clang complains despite this point never being reached
}

// If the current token matches the expected token, consume it and move on to the next. If not, return null.
// There is no fundamental difference between this and `parser_expect_token` expect that this function does not 
// throw an error and is useful for checking for "optional" tokens (like semicolons)
// I could modify `parser_expect_token` to accept a boolean whether to throw an error or not,
// but that's an unnecessary check.
// Maybe I'm being too paranoid about these miniscule performance improvements. Maybe not.
// Will need to re-work on how I approach this. 
// TODO(jasmcaus)
static inline Token* parser_chomp_if(Parser* parser, TokenKind tokenkind) {
    if(pc->kind == tokenkind)
        return CHOMP(1);

    return null;
}

static inline void parser_put_back(Parser* parser) {
    if(parser->offset == 0)
        unreachable();
        
    parser->curr_tok -= 1;
    parser->offset -= 1;
}

AstNode* ast_create_node(AstNodeKind kind) {
    AstNode* node = cast(AstNode*)calloc(1, sizeof(AstNode));
    node->kind = kind;
    return node;
}

static AstNode* ast_parse_root(Parser* parser);
static AstNode* ast_parse_string_literal(Parser* parser);
static AstNode* ast_parse_suffix_op(Parser* parser);
static AstNode* ast_parse_field_init(Parser* parser);
static Token* ast_parse_block_label(Parser* parser);
static Token* ast_parse_break_label(Parser* parser);
static AstNode* ast_parse_match_item(Parser* parser);
static AstNode* ast_parse_builtin_call(Parser* parser);
static AstNode* ast_parse_struct_decl(Parser* parser);
static AstNode* ast_parse_enum_decl(Parser* parser);
static AstNode* ast_parse_match_clause(Parser* );
static AstNode* ast_parse_match_branch(Parser* parser);
static AstNode* ast_parse_match_expr(Parser* parser);
static AstNode* ast_parse_primary_type_expr(Parser* parser);
static AstNode* ast_parse_brace_suffix_expr(Parser* parser);
static AstNode* ast_parse_block(Parser* parser);
static AstNode* ast_parse_primary_expr(Parser* parser);
static AstNode* ast_parse_type_expr(Parser* parser);
static AstNode* ast_parse_prefix_expr(Parser* parser);
static AstNode* ast_parse_precedence(Parser* parser, UInt8 min_prec);
static AstNode* ast_parse_expr(Parser* parser);
static AstNode* ast_parse_assignment_expr(Parser* parser);
static AstNode* ast_parse_block_expr(Parser* parser);
static AstNode* ast_parse_block_expr_statement(Parser* parser);
static AstNode* ast_parse_loop_in_expr(Parser* parser);
static AstNode* ast_parse_loop_c_expr(Parser* parser);
static AstNode* ast_parse_loop_inf_expr(Parser* parser);
static AstNode* ast_parse_loop_expr(Parser* parser);
static AstNode* ast_parse_if_expr(Parser* parser);
static AstNode* ast_parse_statement(Parser* parser);
static AstNode* ast_parse_param_decl(Parser* parser);
static AstNode* ast_parse_param_list(Parser* parser, bool* is_variadic);
static AstNode* ast_parse_func_decl(Parser* parser);
static AstNode* ast_parse_variable_decl(Parser* parser);
static AstNode* ast_parse_use_statement(Parser* parser);
static AstNode* ast_parse_module_statement(Parser* parser);
static AstNode* ast_parse_toplevel_decl(Parser* parser);

// TopLevelDecl
//      | ModuleStatement
//      | UseStatement
//      | VariableDecl
//      | FuncDecl
//      | StructDecl
//      | EnumDecl
static AstNode* ast_parse_toplevel_decl(Parser* parser) {
    AstNode* module = ast_parse_module_statement(parser);
    if(module != null)
        return module;
    
    AstNode* use = ast_parse_use_statement(parser);
    if(use != null)
        return use;

    AstNode* variable = ast_parse_variable_decl(parser);
    if(variable != null)
        return variable;

    AstNode* func_decl = ast_parse_func_decl(parser);
    if(func_decl != null)
        return func_decl;
    
    AstNode* struct_decl = ast_parse_struct_decl(parser);
    if(struct_decl != null)
        return struct_decl;

    AstNode* enum_decl = ast_parse_enum_decl(parser);
    if(enum_decl != null)
        return enum_decl;
    
    return null;
}

// ModuleStatement
//      KEYWORD(module) Identifier SEMICOLON?
static AstNode* ast_parse_module_statement(Parser* parser) {
    Token* module_kwd = CHOMP_IF(MODULE);
    if(module_kwd == null)
        return null;
    
    Token* module_name = CHOMP_IF(IDENTIFIER);
    if(module_name == null)
        AST_EXPECTED("module name");
    
    Token* semicolon = CHOMP_IF(SEMICOLON); // this is optional

    AstNode* node = ast_create_node(AstNodeKindModuleStatement);
    node->data.stmt->module_stmt->name = module_name->value;

    return node;
}

// UseStatement
//      KEYWORD(use) Statement
//
// Currently, only `use foo` is supported.
// In the future, we'd like to have the following as well:
//      1. `from foo use bar`, or
//      2. `from foo use { ... }`
// But the above also contradicts the language's "there must be one way to do something" philosophy.
// To mitigate this, I was leaning toward JavaScript's approach (change `use` -> `import`)
//      1. `use bar from foo`, or
//      2. `use { ... } from foo`
// This seems reasonable, but I much prefer my original thinking `from foo use { ... }` since
// it's a much more streamlined process (in terms of reading) when browsing an Adorad codebase.
static AstNode* ast_parse_use_statement(Parser* parser) {
    Token* use_kwd = CHOMP_IF(USE);
    if(use_kwd == null)
        return null;
    
    Token* use_name = CHOMP_IF(IDENTIFIER);
    if(use_name == null)
        AST_EXPECTED("use name");
    
    Token* semicolon = CHOMP_IF(SEMICOLON); // this is optional

    AstNode* node = ast_create_node(AstNodeKindUseStatement);
    node->data.stmt->use_stmt->name = use_name->value;
    return node;
}

// VariableDecl
//      ATTRIBUTE(comptime)? KEYWORD(put) KEYWORD(mutable)? IDENTIFIER (COLON TypeExpr)? (EQUALS EXPR)? SEMICOLON?
// 
// Example:
//      | put x: int = 34 * 34;
//      | put mutable y: &StructMutability = ...
//      | put mutable z = 34
static AstNode* ast_parse_variable_decl(Parser* parser) {
    Token* comptime_attr = CHOMP_IF(ATTR_COMPTIME);
    Token* put_kwd = CHOMP_IF(PUT);
    if(put_kwd == null)
        AST_EXPECTED("put keyword");

    Token* mutable_kwd = CHOMP_IF(MUTABLE);

    AstNode* type_expr = ast_parse_type_expr(parser);
    if(type_expr == null) {
        AST_EXPECTED("a type");
    }

    Token* identifier = CHOMP_IF(IDENTIFIER);
    if(identifier == null)
        AST_EXPECTED("an identifier");
    
    Token* equals = CHOMP_IF(EQUALS);
    AstNode* init_expr = null;
    if(equals != null) {
        // Expect an expression
        init_expr = ast_parse_expr(parser);
    }

    Token* semicolon = CHOMP_IF(SEMICOLON);

    AstNode* node = ast_create_node(AstNodeKindVariableDecl);
    node->data.scope_obj->var->name = identifier->value;
    node->data.scope_obj->var->init_expr = init_expr;
    node->data.scope_obj->var->is_local = !parser->is_in_global_context;
    node->data.scope_obj->var->is_comptime = cast(bool)(comptime_attr != null);
    node->data.scope_obj->var->is_mutable = cast(bool)(mutable_kwd != null);
    return node;
}

// FuncDecl
//      <Attributes> KEYWORD(export)? KEYWORD(func) IDENTIFIER? LPAREN ParamList RPAREN LARROW TypeExpr (SEMICOLON? / BLOCK)
// where <Attributes> can be one of:
//      | ATTR_NORETURN
//      | ATTR_COMPTIME
//      | ATTR_INLINE
//      | ATTR_NOINLINE
static AstNode* ast_parse_func_decl(Parser* parser) {
    bool is_noreturn = false;
    bool is_comptime = false;
    bool is_inline = false;
    bool is_noinline = false;

    // Attributes?
    if(!token_is_attribute(pc->kind))
        goto func_no_attrs;

    // Parse attributes
    switch(pc->kind) {
        case ATTR_NORETURN: is_noreturn = true; break;
        case ATTR_COMPTIME: is_comptime = true; break;
        case ATTR_INLINE: is_inline = true; break;
        case ATTR_NOINLINE: is_noinline = true; break;
        default: unreachable();
    }
    Token* attr = CHOMP(1);
    if(token_is_attribute(pc->kind))
        AST_ERROR("Can only have one attribute decorating a function");

func_no_attrs:;
    Token* export_kwd = CHOMP_IF(EXPORT);
    Token* func_kwd = CHOMP_IF(FUNC);
    if(func_kwd == null)
        AST_EXPECTED("`func` keyword");
    
    bool is_variadic = false;
    Token* identifier = CHOMP_IF(IDENTIFIER);
    AstNode* params = ast_parse_param_list(parser, &is_variadic);
    
    Token* larrow = CHOMP_IF(LARROW);
    AstNode* return_type_expr = ast_parse_type_expr(parser);
    if(return_type_expr == null)
        AST_EXPECTED("Return type expression. Use `void` if your function doesn't return anything");
    if(larrow == null && return_type_expr != null)
        AST_EXPECTED("trailing `->` after function prototype");
    
    bool no_body = false;
    AstNode* body = null;
    AstNode* node = ast_create_node(AstNodeKindFuncDecl);
    switch(pc->kind) {
        case SEMICOLON:
            CHOMP(1);
            no_body = true;
            break;
        case LBRACE:
            body = ast_parse_block(parser);
            if(body == null)
                AST_EXPECTED("Expected a body");
            break;
        default:
            AST_EXPECTED("Semicolon or Function Body");
    } // switch

    node->data.decl->func_decl->name = identifier->value;
    node->data.decl->func_decl->params = params;
    node->data.decl->func_decl->return_type = return_type_expr;
    node->data.decl->func_decl->no_body = no_body;
    node->data.decl->func_decl->body = body;
    node->data.decl->func_decl->params = params;
    node->data.decl->func_decl->visibility = export_kwd != null ? VisibilityModePublic : VisibilityModePrivate;

    // Attributes
    node->data.decl->func_decl->is_comptime = is_comptime;
    node->data.decl->func_decl->is_noreturn = is_noreturn;
    node->data.decl->func_decl->is_inline = is_inline;
    node->data.decl->func_decl->is_noinline = is_noinline;

    return node;
}

// ParamList
//      (ParamDecl COMMA)* ParamDecl?
static AstNode* ast_parse_param_list(Parser* parser, bool* is_variadic) {
    Token* lparen = EXPECT_TOK(LPAREN);
    bool seen_varargs = false;
    Vec* params = VEC_NEW(AstNode, 1);
    while(true) {
        if(CHOMP_IF(RPAREN) == null)
            break;
        AstNode* param = ast_parse_param_decl(parser);
        if(param != null) {
            vec_push(params, param);
        } else if((pc - 1)->kind == ELLIPSIS) {
            seen_varargs = true;
        }

        switch(pc->kind) {
            case COMMA: CHOMP(1); break;
            case RPAREN: CHOMP(1); break;
            case COLON: 
            case RBRACE: 
            case RSQUAREBRACK: 
                AST_EXPECTED("RPAREN");
            default:
                WARN("Expected comma");
                break;
        }
    }

    AstNode* node = ast_create_node(AstNodeKindParamList);
    node->data.param_list->is_variadic = cast(bool)seen_varargs;
    node->data.param_list->params = params;
    node->data.param_list->is_variadic = seen_varargs;
    *is_variadic = seen_varargs;
    return node;
}

// ParamDecl
static AstNode* ast_parse_param_decl(Parser* parser) {
    CORETEN_ENFORCE(false, "TODO");
    return null;
}

// Statement
//      | VariableDecl
//      | BlockExpr
//      | IfExpr
//      | LabeledStatement
//      | MatchExpr
//      | AssignmentExpr SEMICOLON?
static AstNode* ast_parse_statement(Parser* parser) {
    AstNode* var_decl = ast_parse_variable_decl(parser);
    if(var_decl != null)
        return var_decl;
    
    AstNode* block_expr = ast_parse_block_expr(parser);
    if(block_expr != null)
        return block_expr;
    
    AstNode* if_expr = ast_parse_if_expr(parser);
    if(if_expr != null)
        return if_expr;
    
    AstNode* match_expr = ast_parse_match_expr(parser);
    if(match_expr != null)
        return match_expr;
    
    AstNode* assignment_expr = ast_parse_assignment_expr(parser);
    if(assignment_expr != null)
        return assignment_expr;
    
    WARN("Hmmm could not parse a suitable statement. Returning null");
    return null;
}

static AstNode* ast_parse_struct_decl(Parser* parser) {
    CORETEN_ENFORCE(false, "TODO");
    return null;
}

static AstNode* ast_parse_enum_decl(Parser* parser) {
    CORETEN_ENFORCE(false, "TODO");
    return null;
}

// IfExpr
//      | IfPrefix BlockExpr (KEYWORD(else) Statement)?
//      | IfPrefix AssignmentExpr (SEMICOLON / KEYWORD(else) Statement)?
// where IfPrefix is:
//      KEYWORD(if) LPAREN? Expr RPAREN?
static AstNode* ast_parse_if_expr(Parser* parser) {
    Token* if_token = CHOMP_IF(IF);
    if(if_token == null)
        return null;

    Token* lparen = CHOMP_IF(LPAREN); // this == optional
    AstNode* condition = ast_parse_expr(parser);
    if(condition == null)
        AST_EXPECTED("condition");
    Token* rparen = CHOMP_IF(RPAREN); // this == optional

    if(lparen != null && rparen == null)
        AST_EXPECTED("closing `(`");
    if(lparen == null && rparen != null)
        AST_ERROR("Extra `)` token not expected at this point");
    
    AstNode* if_body = null;
    AstNode* assignment_expr = null;
    AstNode* block_expr = ast_parse_block_expr(parser);
    if(block_expr == null) {
        assignment_expr = ast_parse_assignment_expr(parser);
        if(assignment_expr == null)
            AST_EXPECTED("block / assignment expression");
    }

    if(block_expr != null)
        if_body = block_expr;
    if(assignment_expr != null)
        if_body = assignment_expr;

    // If a semicolon is here, chomp it
    Token* semicolon = CHOMP_IF(SEMICOLON);
    
    AstNode* else_body = null;
    Token* else_kwd = CHOMP_IF(ELSE);
    if(else_kwd != null)
        else_body = ast_parse_block_expr(parser);
    
    if(else_body == null && semicolon == null)  
        AST_EXPECTED("Semicolon or `else` block");

    AstNode* node = ast_create_node(AstNodeKindIfExpr);
    node->data.expr->if_expr->condition = condition;
    node->data.expr->if_expr->if_body = if_body;
    node->data.expr->if_expr->has_else = else_body != null;
    node->data.expr->if_expr->else_node = else_body;
    return node;
}

// LoopExpr
//      ATTRIBUTE(inline)? (LoopWhileExpr / LoopCExpr / LoopInExpr)
static AstNode* ast_parse_loop_expr(Parser* parser) {
    Token* inline_attr = CHOMP_IF(ATTR_INLINE);
    AstNode* node = null;

    AstNode* loop_inf_expr = ast_parse_loop_inf_expr(parser);
    if(loop_inf_expr != null) {
        node = loop_inf_expr;
        goto outexpect;
    }

    AstNode* loop_c_expr = ast_parse_loop_c_expr(parser);
    if(loop_c_expr != null) {
        node = loop_c_expr;
        goto outexpect;
    }

    AstNode* loop_in_expr = ast_parse_loop_in_expr(parser);
    if(loop_in_expr != null) {
        node = loop_in_expr;
        goto outexpect;
    }

outexpect:
    if(node == null)
        AST_EXPECTED("loop expression");

    node->data.expr->loop_expr->is_inline = cast(bool)(inline_attr != null);
    return node;
}

/*
    TODO: Add implementation for:
        1. ast_parse_loop_inf_expr(parser)
        2. ast_parse_loop_c_expr(parser)
        3. ast_parse_loop_in_expr(parser)
*/
static AstNode* ast_parse_loop_inf_expr(Parser* parser) {
    CORETEN_ENFORCE(false, "TODO");
    return null;
}

static AstNode* ast_parse_loop_c_expr(Parser* parser) {
    CORETEN_ENFORCE(false, "TODO");
    return null;
}

static AstNode* ast_parse_loop_in_expr(Parser* parser) {
    CORETEN_ENFORCE(false, "TODO");
    return null;
}

// BlockExprStatement
//      | BlockExpr
//      | AssignmentExpr SEMICOLON?
static AstNode* ast_parse_block_expr_statement(Parser* parser) {
    AstNode* block_expr = ast_parse_block_expr(parser);
    if(block_expr != null)
        return block_expr;
    
    AstNode* assignment_expr = ast_parse_assignment_expr(parser);
    if(assignment_expr != null) {
        Token* semicolon = CHOMP_IF(SEMICOLON);
        return assignment_expr;
    }

    AST_EXPECTED("block or assignment expression");
    return null;
}

// BlockExpr
//      BlockLabel? Block
static AstNode* ast_parse_block_expr(Parser* parser) {
    switch(pc->kind) {
        case IDENTIFIER:
            if((pc->kind + 1) == COLON && (pc->kind + 2) == LBRACE) {
                pc += 2;
                return ast_parse_block(parser);    
            } else {
                return null;
            }
            break;
        case LBRACE:
            return ast_parse_block(parser);
        default:
            return null;
    }
}

// AssignmentExpr
//      Expr (AssignmentOp Expr)?
// where AssignmentOp can be one of:
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
//      | EQUALS            (=)
static AstNode* ast_parse_assignment_expr(Parser* parser) {
    AstNode* lhs = ast_parse_expr(parser);
    if(lhs == null)
        return null;

    BinaryOpKind op;
    switch(pc->kind) {
        // AssignmentOp
        case MULT_EQUALS: op = BinaryOpKindAssignmentMult; break;
        case SLASH_EQUALS: op = BinaryOpKindAssignmentDiv; break;
        case MOD_EQUALS: op = BinaryOpKindAssignmentMod; break;
        case PLUS_EQUALS: op = BinaryOpKindAssignmentPlus; break;
        case MINUS_EQUALS: op = BinaryOpKindAssignmentMinus; break;
        case LBITSHIFT_EQUALS: op = BinaryOpKindAssignmentBitshiftLeft; break;
        case RBITSHIFT_EQUALS: op = BinaryOpKindAssignmentBitshiftRight; break;
        case AND_EQUALS: op = BinaryOpKindAssignmentBitAnd; break;
        case XOR_EQUALS: op = BinaryOpKindAssignmentBitXor; break;
        case OR_EQUALS: op = BinaryOpKindAssignmentBitOr; break;
        case EQUALS: op = BinaryOpKindAssignmentEquals; break;

        default: return lhs;
    }

    AstNode* rhs = ast_parse_expr(parser);
    if(rhs == null)
        AST_EXPECTED("an expression after assignment op");

    AstNode* node = ast_create_node(AstNodeKindBinaryOpExpr);
    node->data.expr->binary_op_expr->op = op;
    node->data.expr->binary_op_expr->lhs = lhs;
    node->data.expr->binary_op_expr->rhs = rhs;
    return node;
}

typedef struct ast_prec {
    TokenKind tok_kind;
    UInt8 prec;
    BinaryOpKind bin_kind;
} ast_prec;

// A table of binary operator precedence, from strong to weak. Binary Operators of the same precedence value
// are grouped together in the order given by their associativity.
#define PRECEDENCE_TABLE_SIZE   (sizeof(precedence_table)/sizeof(precedence_table[0]))
static const ast_prec precedence_table[] = {
    // { .tok_kind = MULT_MULT, .prec = 60, .bin_kind = BinaryOpKindMultMult },
    { .tok_kind = MULT,  .prec = 60, .bin_kind = BinaryOpKindMult },
    { .tok_kind = MOD,   .prec = 60, .bin_kind = BinaryOpKindMod },
    { .tok_kind = SLASH, .prec = 60, .bin_kind = BinaryOpKindDiv },

    { .tok_kind = PLUS,         .prec = 50, .bin_kind = BinaryOpKindAdd },
    { .tok_kind = MINUS,        .prec = 50, .bin_kind = BinaryOpKindSubtract },
    { .tok_kind = PLUS_EQUALS,  .prec = 50, .bin_kind = BinaryOpKindAssignmentPlus },
    { .tok_kind = MINUS_EQUALS, .prec = 50, .bin_kind = BinaryOpKindAssignmentMinus },

    { .tok_kind = LBITSHIFT, .prec = 40, .bin_kind = BinaryOpKindBitshitLeft },
    { .tok_kind = RBITSHIFT, .prec = 40, .bin_kind = BinaryOpKindBitshitRight },
    
    { .tok_kind = LESS_THAN,                 .prec = 30, .bin_kind = BinaryOpKindCmpLessThan },
    { .tok_kind = GREATER_THAN,              .prec = 30, .bin_kind = BinaryOpKindCmpGreaterThan },
    { .tok_kind = EQUALS_EQUALS,             .prec = 30, .bin_kind = BinaryOpKindCmpEqual },
    { .tok_kind = EXCLAMATION_EQUALS,        .prec = 30, .bin_kind = BinaryOpKindCmpNotEqual },
    { .tok_kind = LESS_THAN_OR_EQUAL_TO,     .prec = 30, .bin_kind = BinaryOpKindCmpLessThanorEqualTo },
    { .tok_kind = GREATER_THAN_OR_EQUAL_TO,  .prec = 30, .bin_kind = BinaryOpKindCmpGreaterThanorEqualTo },

    { .tok_kind = AND, .prec = 20, .bin_kind = BinaryOpKindBoolAnd },

    { .tok_kind = OR,  .prec = 10, .bin_kind = BinaryOpKindBoolOr },
};

static inline ast_prec lookup_precedence(TokenKind kind) {
    for(int i = 0; i < PRECEDENCE_TABLE_SIZE; i++) {
        if(precedence_table[i].tok_kind == kind)
            return precedence_table[i];
    }
    AST_ERROR("Expected a valid assignment token op");
    return cast(ast_prec) {0}; // Clang complains despite this point never being reached
}

static AstNode* ast_parse_precedence(Parser* parser, UInt8 min_prec) {
    AstNode* node = ast_parse_prefix_expr(parser);
    if(node == null)
        return null;

    UInt8 banned_prec = 0;

    while(true) {
        ast_prec prec = lookup_precedence(pc->kind);
        if(prec.prec < min_prec || prec.prec == banned_prec)
            break;
        
        CHOMP(1);
        Token* op_token = pc;

        AstNode* rhs = ast_parse_precedence(parser, prec.prec + 1);
        if(rhs == null)
            AST_ERROR("Invalid token");
        
        node->data.expr->binary_op_expr->lhs = node;
        node->data.expr->binary_op_expr->op = prec.bin_kind;
        node->data.expr->binary_op_expr->rhs = rhs;

        switch(pc->kind) {
            case EQUALS_EQUALS:
            case EXCLAMATION_EQUALS:
            case GREATER_THAN:
            case LESS_THAN:
            case GREATER_THAN_OR_EQUAL_TO:
            case LESS_THAN_OR_EQUAL_TO:
                banned_prec = prec.prec;
                break;
            default:
                break;
        }
    }
    return node;
}

static AstNode* ast_parse_expr(Parser* parser) {
    return ast_parse_precedence(parser, 0);
}

// PrefixExpr
//      PrefixOp* PrimaryExpr
// where PrefixOp is one of:
//      | EXCLAMATION   (!)
//      | MINUS         (-)
//      | TILDA         (~)
//      | AND           (&)
static AstNode* ast_parse_prefix_expr(Parser* parser) {
    PrefixOpKind op;
    switch(parser->curr_tok->kind) {
        case NOT: op = PrefixOpKindBoolNot; break;
        case EXCLAMATION: op = PrefixOpKindNegation; break;
        case AND: op = PrefixOpKindAddrOf; break;
        default: return ast_parse_primary_expr(parser);
    }

    AstNode* lhs = ast_parse_prefix_expr(parser);
    if(lhs == null)
        AST_EXPECTED("prefix op expression");

    AstNode* node = ast_create_node(AstNodeKindPrefixOpExpr);
    node->data.prefix_op_expr->op = op;
    node->data.prefix_op_expr->expr = lhs;
    return node;
}

// TypeExpr
//      (QUESTION / AND)? TypeExpr SliceExpr?
// where SliceExpr is:
//      LSQUAREBRACK (COLON Expr)? RSQUAREBRACK
static AstNode* ast_parse_type_expr(Parser* parser) {
    AstNode* node = null;
    AstNode* expr = null;
    TokenKind kind;

    switch(pc->kind) {
        case QUESTION: case AND:
            kind = pc->kind;
            expr = ast_parse_expr(parser);

            node = ast_create_node(AstNodeKindTypeExpr);
            node->data.expr->type_expr->expr = expr;
            node->data.expr->type_expr->is_address = kind == AND;
            node->data.expr->type_expr->is_optional = kind == QUESTION;
            node->data.expr->type_expr->is_slice_expr = false;
            break;
        case IDENTIFIER:
            expr = ast_parse_expr(parser);
            if(pc->kind == LSQUAREBRACK) {
                CORETEN_ENFORCE(false, "TODO: Parse optional SliceExpr");
            }
            break;
        default:
            AST_EXPECTED("Something");
            break;
    }

    return node;
}

// PrimaryExpr
//      | IfExpr
//      | KEYWORD(break) BreakLabel? Expr?
//      | KEYWORD(continue) BreakLabel?
//      | ATTRIBUTE(comptime) Expr
//      | KEYWORD(return) Expr?
//      | BlockLabel? ATTRIBUTE(inline)? LoopExpr
//      | Block
static AstNode* ast_parse_primary_expr(Parser* parser) {
    AstNode* node = null;
    AstNode* expr = null;
    Token* label = null;
    switch(pc->kind) {
        case IF: return ast_parse_if_expr(parser);
        case BREAK: 
            CHOMP(1);
            label = ast_parse_break_label(parser);
            expr = ast_parse_expr(parser);

            node = ast_create_node(AstNodeKindBreak);
            node->data.stmt->branch_stmt->type = AstNodeBranchStatementBreak;
            node->data.stmt->branch_stmt->name = label != null ? label->value : null;
            node->data.stmt->branch_stmt->expr = expr;
            return node;
        case CONTINUE:
            CHOMP(1);
            label = ast_parse_break_label(parser);
            node = ast_create_node(AstNodeKindBreak);
            node->data.stmt->branch_stmt->type = AstNodeBranchStatementContinue;
            node->data.stmt->branch_stmt->name = label != null ? label->value : null;
            node->data.stmt->branch_stmt->expr = null;
            return node;
        case ATTR_COMPTIME:
            CHOMP(1);
            node = ast_create_node(AstNodeKindAttributeExpr);
            expr = ast_parse_expr(parser);
            if(expr == null)
                AST_EXPECTED("expression");
            
            node->data.expr->attr_expr->expr = expr;
            return node;
        case RETURN:
            CHOMP(1);
            node = ast_create_node(AstNodeKindReturn);
            expr = ast_parse_expr(parser);
            node->data.stmt->return_stmt->expr = expr;
            return node;
        case IDENTIFIER:
            // `foo:`
            if((pc + 1)->kind == COLON) {
                switch((pc + 2)->kind) {
                    case ATTR_INLINE:
                        CHOMP(3);
                        switch(pc->kind) {
                            case LOOP: return ast_parse_loop_expr(parser);
                            default: AST_EXPECTED("inlinable expression");
                        }
                    case LOOP:
                        CHOMP(2);
                        return ast_parse_loop_expr(parser);
                    case LBRACE:
                        CHOMP(2);
                        return ast_parse_block(parser);
                    default:
                        unreachable();
                }
            }
            break;
        case ATTR_INLINE:
            CHOMP(1);
            switch(pc->kind) {
                case LOOP: return ast_parse_loop_expr(parser);
                default: AST_EXPECTED("inlinable expression");
            }
        case LOOP: 
            return ast_parse_loop_expr(parser);
        default:
            AST_ERROR("Invalid parser pattern");
    } // switch(pc->kind)
    AST_ERROR("Invalid parser pattern");
    return null; // Clang complains despite this point never being reached
}

// Block
//      LBRACE Statement* RBRACE
static AstNode* ast_parse_block(Parser* parser) {
    Token* lbrace = CHOMP_IF(LBRACE);
    if(lbrace == null)
        AST_EXPECTED("LBRACE `{`");

    Vec* statements = VEC_NEW(AstNode, 1);
    AstNode* statement = null;
    while((statement = ast_parse_statement(parser)) != null)
        vec_push(statements, statement);

    Token* rbrace = CHOMP_IF(RBRACE);
    if(rbrace == null)
        AST_EXPECTED("RBRACE `}`");
    
    AstNode* node = ast_create_node(AstNodeKindBlock);
    node->data.stmt->block_stmt->statements = statements;
    return node;
}

/* WIP */
// BraceSuffixExpr
//      TypeExpr InitList?
// where InitList is:
//      | LBRACE FieldInit (COMMA FieldInit)* COMMA? RBRACE
//      | LBRACE Expr (COMMA Expr)* COMMA? RBRACE
//      | LBRACE RBRACE
static AstNode* ast_parse_brace_suffix_expr(Parser* parser) {
    AstNode* type_expr = ast_parse_type_expr(parser);
    if(type_expr == null)
        AST_EXPECTED("type expression");
    
    Token* lbrace = CHOMP_IF(LBRACE);
    if(lbrace == null)
        return type_expr;
    
    Vec* fields = null;
    AstNode* field_init = ast_parse_field_init(parser);
    if(field_init != null) {
        fields = VEC_NEW(AstNode, 1);
        vec_push(fields, field_init);
        while(true) {
            switch(pc->kind) {
                case COMMA: CHOMP(1); break;
                case RBRACE: CHOMP(1); break;
                case COLON: case RPAREN: case RSQUAREBRACK:
                    AST_EXPECTED("RBRACE");
                default:
                    // Likely just a missing comma; warn, but continue parsing
                    WARN("missing comma");
            }
            Token* rbrace = CHOMP_IF(RBRACE);
            if(rbrace != null)
                break;
            AstNode* field_init = ast_parse_field_init(parser);
            if(field_init == null)
                AST_EXPECTED("field init");
            vec_push(fields, field_init);
        } // while(true)
        Token* comma = CHOMP_IF(COMMA);
        AstNode* node = ast_create_node(AstNodeKindStructExpr);
        node->data.expr->init_expr->kind = InitExprKindStruct;
        node->data.expr->init_expr->entries = fields;
        return node;
    }

    AstNode* node = ast_create_node(AstNodeKindArrayInitExpr);
    node->data.expr->init_expr->kind = InitExprKindArray;

    AstNode* expr = ast_parse_expr(parser);
    if(expr != null) {
        Vec* fields = VEC_NEW(AstNode, 1);
        vec_push(fields, expr);
        Token* comma = null;
        while(pc->kind != COMMA) {
            AstNode* exp = ast_parse_expr(parser);
            if(exp == null)
                break;
            vec_push(fields, exp);
        }
        node->data.expr->init_expr->entries = fields;
        return node;
    }

    Token* rbrace = CHOMP_IF(RBRACE);
    if(rbrace == null)
        AST_EXPECTED("RBRACE");
    
    return node;
}

/* WIP */
/*
// SuffixExpr
//      PrimaryTypeExpr SuffixOp* FuncCallArgs
//      PrimaryTypeExpr (SuffixOp / FuncCallArgs)*
// where FuncCallArgs are:
//      LPAREN ExprList RPAREN
// && ExprList is
//      (Expr COMMA)* Expr?
static AstNode* ast_parse_suffix_expr(Parser* parser) {
    AstNode* node = ast_parse_primary_type_expr(parser);
    while(true) {
        AstNode* suffix_op = ast_parse_suffix_op(parser);
        if(suffix_op == null)
            break;
        node = suffix_op;
    }
    Token* lparen = CHOMP_IF(LPAREN);
    if(lparen == null) {
        WARN("expected param list");
        return node;
    }

    Vec* params = VEC_NEW(AstNode, 1);
    AstNode* param = null;
    while(CHOMP(1)->kind != RPAREN) {
        param = ast_parse_expr(parser);
        if(param == null)
            break;
        vec_push(params, param);
        switch(pc->kind) {
            case COMMA: CHOMP(1);
            case RPAREN: CHOMP(1); break;
            case COLON: case RBRACE: case RSQUAREBRACK:
                AST_EXPECTED("RBRACE");
            default:
                WARN("expected comma");
        }
    }
}
*/

// PrimaryTypeExpr
//      BUILTINIDENTIFIER FuncCallArgs
//      CHAR_LITERAL
//      StructDecl
//      EnumDecl
//      DOT IDENTIFIER
//      FLOAT_LITERAL
//      FuncPrototype
//      GroupedExpr
//      LabeledTypeExpr
//      IDENTIFIER
//      IfTypeExpr
//      INTEGER_LITERAL
//      ATTRIBUTE(comptime) TypeExpr
//      KEYWORD(unreachable)
//      STRING_LITERAL
//      MatchExpr
// where GroupedExpr is:
//      LPAREN Expr RPAREN
// && LabeledTypeExpr is one of:
//      | BlockLabel Block
//      | BlockLabel? LoopTypeExpr
static AstNode* ast_parse_primary_type_expr(Parser* parser) {
    AstNode* node = null;
    AstNode* expr = null;
    Token* tok = null;
    switch(pc->kind) {
        case CHAR_LIT:
            node = ast_create_node(AstNodeKindCharLiteral);
            node->data.literal->char_value->value = pc->value;
            CHOMP(1);
            return node;
        case INTEGER:
            node = ast_create_node(AstNodeKindIntLiteral);
            node->data.literal->int_value->value = pc->value;
            CHOMP(1);
            return node;
        case FLOAT_LIT:
            node = ast_create_node(AstNodeKindFloatLiteral);
            node->data.literal->float_value->value = pc->value;
            CHOMP(1);
            return node;
        case UNREACHABLE:
            node = ast_create_node(AstNodeKindUnreachable);
            CHOMP(1);
            return node;
        case STRING:
            node = ast_create_node(AstNodeKindStringLiteral);
            CHOMP(1);
            return node;
        case BUILTIN: return ast_parse_builtin_call(parser);
        case FUNC: return ast_parse_func_decl(parser);
        case IF: return ast_parse_if_expr(parser);
        case MATCH: return ast_parse_match_expr(parser);
        case STRUCT: return ast_parse_struct_decl(parser);
        case ENUM: return ast_parse_enum_decl(parser);
        case ATTR_COMPTIME:
            node = ast_create_node(AstNodeKindAttributeExpr);
            expr = ast_parse_type_expr(parser);
            if(expr == null)
                AST_EXPECTED("type expr");
            node->data.expr->attr_expr->expr = expr;
            return node;
        case IDENTIFIER:
            switch((pc + 1)->kind) {
                case COLON:
                    switch((pc + 2)->kind) {
                        case ATTR_INLINE:
                            CHOMP(3);
                            switch(pc->kind) {
                                case LOOP: return ast_parse_loop_expr(parser);
                                default: AST_EXPECTED("inlinable expression");
                            }
                        case LOOP:
                            CHOMP(2);
                            return ast_parse_loop_expr(parser);
                        case LBRACE:
                            CHOMP(2);
                            return ast_parse_block(parser);
                        default:
                            node = ast_create_node(AstNodeKindIdentifier);
                            CHOMP(1);
                            return node;
                    }
                default:
                    node = ast_create_node(AstNodeKindIdentifier);
                    CHOMP(1);
                    return node;
            }
            break;
        case ATTR_INLINE:
            CHOMP(1);
            switch(pc->kind) {
                case LOOP: return ast_parse_loop_expr(parser);
                default: AST_EXPECTED("inlinable expression");
            }
            break;
        case LOOP: return ast_parse_loop_expr(parser);
        case DOT:
            switch((pc + 1)->kind) {
                case IDENTIFIER:
                    node = ast_create_node(AstNodeKindIdentifier);
                    CHOMP(1);
                    return node;
                default: return null;
            }
            break;
        case LPAREN:
            node = ast_create_node(AstNodeKindGroupedExpr);
            expr = ast_parse_expr(parser);
            if(expr == null)
                AST_EXPECTED("expression");
            tok = CHOMP_IF(RPAREN);
            if(tok == null)
                AST_EXPECTED("RPAREN");

            node->data.expr->grouped_expr->expr = expr;
            return node;
    }
    return null;
}

static AstNode* ast_parse_builtin_call(Parser* parser) {
    CORETEN_ENFORCE(false, "TODO");
    return null;
}

// MatchExpr
//      KEYWORD(match) LPAREN? Expr RPAREN? LBRACE (MatchBranch)* RBRACE
// where MatchBranch is:
//      KEYWORD(when) Expr EQUALS_ARROW (AssignmentExpr / BlockExpr)
static AstNode* ast_parse_match_expr(Parser* parser) {
    Token* match_kwd = CHOMP_IF(MATCH);
    if(match_kwd == null)
        AST_EXPECTED("`match` keyword");
    
    Token* lparen = CHOMP_IF(LPAREN); // this is optional
    AstNode* expr = ast_parse_expr(parser);
    if(expr == null)
        AST_EXPECTED("expression");
    Token* rparen = CHOMP_IF(RPAREN); // this is optional
    
    Token* lbrace = EXPECT_TOK(RBRACE); // required

    // Branches
    AstNode* branch_node = ast_parse_match_branch(parser);
    if(branch_node == null)
        AST_EXPECTED("branches for `match`");
    Vec* branches = VEC_NEW(AstNode, 1);
    do {
        vec_push(branches, branch_node);
    } while((branch_node = ast_parse_match_branch(parser)) != null);

    // Parse any trailing comma
    Token* comma = CHOMP_IF(COMMA);

    AstNode* node = ast_create_node(AstNodeKindMatchExpr);
    node->data.expr->match_expr->expr = expr;
    node->data.expr->match_expr->branches = branches;
    return node;
}

// MatchBranch
//      KEYWORD(when) MatchClause (COLON? / EQUALS_ARROW?) (AssignmentExpr / BlockExpr)
// where MatchClause is:
//      Expr (DOT_DOT Expr)?
static AstNode* ast_parse_match_branch(Parser* parser) {
    Token* when_kwd = CHOMP_IF(WHEN);
    if(when_kwd == null)
        AST_EXPECTED("`when` keyword");

    // MatchClause
    AstNode* node = ast_parse_match_clause(parser);
    CORETEN_ENFORCE(node->kind == AstNodeKindMatchBranch);
    if(node == null)
        return null;
    
    Token* equals_arrow = CHOMP_IF(EQUALS_ARROW); // `=>`
    if(equals_arrow == null)
        AST_ERROR("Missing token `=>` after `when`");

    AstNode* expr = ast_parse_assignment_expr(parser);
    if(expr == null) {
        expr = ast_parse_block_expr(parser);
        if(expr == null)
            AST_EXPECTED("An assignment/block expression after the `when` clause");
    }

    node->data.expr->match_branch_expr->block_node = expr;
    return node;
}

// MatchCase
//      Expr (DDOT Expr)*
static AstNode* ast_parse_match_clause(Parser* parser) {
    AstNode* expr = ast_parse_expr(parser);
    if(expr == null) {
        WARN("Expression is null");
        return null;
    }

    AstNode* out = ast_create_node(AstNodeKindMatchBranch);
    out->data.expr->match_branch_expr->is_range = false;
    AstNode* cond_node = expr;

    Token* dot_dot = CHOMP_IF(DDOT);
    if(dot_dot != null) {
        // Range-based
        AstNode* expr2 = ast_parse_expr(parser);
        if(expr2 == null)
            AST_EXPECTED("Expected expression after `..`");

        AstNode* range = ast_create_node(AstNodeKindMatchRange);
        range->data.expr->match_range_expr->begin = expr;
        range->data.expr->match_range_expr->end = expr2;
        cond_node = range;
        out->data.expr->match_branch_expr->is_range = true;
    }
    
    out->data.expr->match_branch_expr->cond_node = cond_node;

    CORETEN_ENFORCE(parser_peek_next(parser)->kind == EQUALS_ARROW, "Expected `=>` after expression.");

    return out;
}

// BreakLabel
//      COLON IDENTIFIER
static Token* ast_parse_break_label(Parser* parser) {
    Token* colon = CHOMP_IF(COLON);
    if(colon == null)
        return null;
        
    Token* ident = EXPECT_TOK(IDENTIFIER);
    return ident;
}

// BlockLabel
//      IDENTIFIER COLON
static Token* ast_parse_block_label(Parser* parser) {
    Token* ident = CHOMP_IF(IDENTIFIER);
    if(ident == null)
        return null;
    
    Token* colon = CHOMP_IF(COLON);
    if(colon == null)
        return null;

    return ident;
}

// FieldInit
//      DOT IDENTIFIER EQUALS Expr
static AstNode* ast_parse_field_init(Parser* parser) {
    if((pc + 0)->kind == DOT &&
       (pc + 1)->kind == IDENTIFIER &&
       (pc + 2)->kind == EQUALS) {
            CHOMP(3);
            AstNode* expr = ast_parse_expr(parser);
            if(expr == null)
                AST_EXPECTED("expression");
            return expr;
    }
    return null;
}

// SuffixOp
//      | LBRACKET Expr (DOT2 (Expr (COLON Expr)?)?)? RBRACKET
//      | DOT IDENTIFIER
static AstNode* ast_parse_suffix_op(Parser* parser) {
    Token* lbrace = CHOMP_IF(LBRACE);
    if(lbrace != null) {
        AstNode* lower = ast_parse_expr(parser);
        AstNode* upper = null;
        Token* ellipsis = CHOMP_IF(ELLIPSIS);
        if(ellipsis != null) {
            AstNode* sentinel = null;
            upper = ast_parse_expr(parser);
            Token* colon = CHOMP_IF(COLON);
            if(colon != null) {
                sentinel = ast_parse_expr(parser);
            }
            Token* rbrace = EXPECT_TOK(RBRACE);

            AstNode* node = ast_create_node(AstNodeKindSliceExpr);
            node->data.expr->slice_expr->lower = lower;
            node->data.expr->slice_expr->upper = upper;
            node->data.expr->slice_expr->sentinel = sentinel;
            return node;
        }

        Token* rbrace = EXPECT_TOK(RBRACE);

        AstNode* node = ast_create_node(AstNodeKindArrayAccessExpr);
        node->data.array_access_expr->subscript = lower;
        return node;
    }

    Token* dot = CHOMP_IF(DOT);
    if(dot != null) {
        Token* identifier = EXPECT_TOK(IDENTIFIER);
        AstNode* node = ast_create_node(AstNodeKindFieldAccessExpr);
        node->data.field_access_expr->field_name = identifier->value;
        return node;
    }

    return null;
}

// StringLiteral
static AstNode* ast_parse_string_literal(Parser* parser) {
    if(pc->kind == STRING) {
        CHOMP(1);
        AstNode* node = ast_create_node(AstNodeKindStringLiteral);
        return node;
    }

    WARN("Entered `ast_parse_string_literal` with unrelated token");
    return null;
}

static AstNode* ast_parse_root(Parser* parser) {
    return null;   
}

// Entry point for the Parser
// static AstNode* ast_parse(Parser* parser) {
//     LOG("Entering `ast_parse`");

//     // Begin with the module declaration
//     AstNode* module_decl = ast_parse_module_statement(parser);
    
//     Vec* stmts = VEC_NEW(AstNode, 1);
//     // Imports
//     for(;;) {
//         if(pc->kind == IMPORT)
//     }
// }

AstNode* return_result(Parser* parser) {
    return ast_parse_block_expr(parser);
}

// Free a Parser* instance
static void parser_free(Parser* parser) {
    if(parser != null) {
        lexer_free(parser->lexer);
        buff_free(parser->mod_name);
        vec_free(parser->nodelist);
        free(parser);
    }
}