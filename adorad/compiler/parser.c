#include <adorad/compiler/ast.h>
#include <adorad/compiler/parser.h>
#include <adorad/core/debug.h>

#define pt      parser->toklist
#define pc      parser->curr_tok

#define nodepush(node)              vec_push(parser->nodelist, node)

#define parser_chomp(n)             chomp(parser, n)
#define parser_chomp_if(kind)       chomp_if(parser, kind)
#define parser_expect_token(kind)   expect_token(parser, kind)

#define ast_error(...)              panic(ErrorParseError, __VA_ARGS__)
#define ast_expected(...)           (ast_error("Expected %s; got `%s`", (__VA_ARGS__), tokenHash[pc->kind]))
#define ast_unexpected(...)         (panic(ErrorUnexpectedToken, __VA_ARGS__))

#ifdef ADORAD_DEBUG
    #define TRACE_PARSER()                                                \
        cstlColouredPrintf(                                               \
            CORETEN_COLOUR_WARN,                                          \
            "parsing file: `%s` | curr_tok: `%s` | location: `%d:%d`",    \
            parser->basename,                                             \
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

static inline Token* parser_peek_next(Parser* parser) {
    if(parser->offset + 1 >= parser->num_tokens)
        return null;

    return cast(Token*)(parser->toklist + parser->offset + 1);
}

// Consumes a token and moves on to the next `N` tokenS
static inline Token* chomp(Parser* parser, UInt64 n) {
    if(parser->offset + n >= parser->num_tokens)
        return null;

    parser->offset += n;
    parser->curr_tok += n;
    return cast(Token*)parser->curr_tok;
}

// Consumes a token and moves on to the next, if the current token matches the expected token.
static inline Token* chomp_if(Parser* parser, TokenKind tokenkind) {
    if((parser->curr_tok + 1) isnot null && (parser->curr_tok + 1)->kind is tokenkind)
        return chomp(parser, 1);

    return null;
}

static inline void parser_put_back(Parser* parser) {
    if(parser->offset is 0)
        unreachable();
    parser->curr_tok -= 1;
    parser->offset -= 1;
}

static inline Token* expect_token(Parser* parser, TokenKind tokenkind) {
    if(parser->curr_tok->kind is tokenkind)
        return parser_chomp(1);
        
    ast_error("Expected `%s`; got `%s`", tokenHash[tokenkind], tokenHash[pc->kind]);
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
// static AstNode* ast_parse_suffix_expr(Parser* parser);
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
static AstNode* ast_parse_labeled_statement(Parser* parser);
static AstNode* ast_parse_if_expr(Parser* parser);
static AstNode* ast_parse_statement(Parser* parser);
static AstNode* ast_parse_toplevel_comptime_expr(Parser* parser);
static AstNode* ast_parse_param_decl(Parser* parser);
static AstNode* ast_parse_param_list(Parser* parser, bool* is_variadic);
static AstNode* ast_parse_func_decl(Parser* parser);
static AstNode* ast_parse_variable_decl(Parser* parser);
static AstNode* ast_parse_alias_decl(Parser* parser);
static AstNode* ast_parse_import_statement(Parser* parser);
static AstNode* ast_parse_module_statement(Parser* parser);
static AstNode* ast_parse_toplevel_decl(Parser* parser);

// TopLevelDecl
//      | ModuleStatement
//      | ImportStatement
//      | AliasExpr
//      | TopLevelComptime
//      | VariableDecl
//      | FuncDecl
//      | StructDecl
//      | EnumDecl
// where `TopLevelComptime` is
//      ATTRIBUTE(comptime) BlockExpr
static AstNode* ast_parse_toplevel_decl(Parser* parser) {
    AstNode* module = ast_parse_module_statement(parser);
    if(module isnot null)
        return module;
    
    AstNode* import = ast_parse_import_statement(parser);
    if(import isnot null)
        return import;
    
    AstNode* alias = ast_parse_alias_decl(parser);
    if(alias isnot null)
        return alias;
    
    AstNode* comptime = ast_parse_toplevel_comptime_expr(parser);
    if(comptime isnot null)
        return comptime;

    AstNode* variable = ast_parse_variable_decl(parser);
    if(variable isnot null)
        return variable;

    AstNode* func_decl = ast_parse_func_decl(parser);
    if(func_decl isnot null)
        return func_decl;
    
    AstNode* struct_decl = ast_parse_struct_decl(parser);
    if(struct_decl isnot null)
        return struct_decl;

    AstNode* enum_decl = ast_parse_enum_decl(parser);
    if(enum_decl isnot null)
        return enum_decl;
    
    return null;
}

// ModuleStatement
//      KEYWORD(module) Statement SEMICOLON?
static AstNode* ast_parse_module_statement(Parser* parser) {
    Token* module_kwd = parser_chomp_if(MODULE);
    if(module_kwd is null)
        ast_expected("`module` keyword");
    
    Token* module_name = parser_chomp_if(IDENTIFIER);
    if(module_name is null)
        ast_expected("module name");
    
    Token* semicolon = parser_chomp_if(SEMICOLON); // this is optional

    AstNode* node = ast_create_node(AstNodeKindModuleStatement);
    node->data.stmt->module_stmt->name = module_name->value;
    return node;
}

// ImportStatement
//      KEYWORD(import) Statement
static AstNode* ast_parse_import_statement(Parser* parser) {
    Token* import_kwd = parser_chomp_if(IMPORT);
    if(import_kwd is null)
        ast_expected("`import` keyword");
    
    Token* import_name = parser_chomp_if(IDENTIFIER);
    if(import_name is null)
        ast_expected("import name");
    
    Token* semicolon = parser_chomp_if(SEMICOLON); // this is optional

    AstNode* node = ast_create_node(AstNodeKindImportStatement);
    node->data.stmt->import_stmt->name = import_name->value;
    return node;
}

// AliasDecl
//      KEYWORD(alias) IDENTIFER KEYWORD(as) IDENTIFIER SEMICOLON?
static AstNode* ast_parse_alias_decl(Parser* parser) {
    Token* alias_kwd = parser_chomp_if(ALIAS);
    if(alias_kwd is null)
        unreachable();
    
    Token* original = parser_chomp_if(IDENTIFIER);
    if(original is null)
        ast_expected("an identifier after `alias`");
    
    Token* as_kwd = parser_chomp_if(AS);
    if(as_kwd is null)
        ast_expected("`as` keyword");
    
    Token* aliased = parser_chomp_if(IDENTIFIER);
    if(aliased is null)
        ast_expected("an alias for identifier");
    
    Token* semicolon = parser_chomp_if(SEMICOLON); // this is optional

    AstNode* node = ast_create_node(AstNodeKindAliasDeclExpr);
    node->data.decl->alias_decl->original = original->value;
    node->data.decl->alias_decl->alias = aliased->value;
    return node;
}

// VariableDecl
//      ATTRIBUTE(comptime)? KEYWORD(export)? KEYWORD(mutable)? (TypeExpr / KEYWORD(any)) IDENTIFIER (EQUALS EXPR)? SEMICOLON?
static AstNode* ast_parse_variable_decl(Parser* parser) {
    Token* comptime_attr = parser_chomp_if(ATTR_COMPTIME);
    Token* export_kwd = parser_chomp_if(EXPORT);
    Token* mutable_kwd = parser_chomp_if(MUTABLE);

    AstNode* type_expr = ast_parse_type_expr(parser);
    Token* any = null;
    if(type_expr is null) {
        any = parser_chomp_if(ANY);
        if(any is null)
            ast_expected("a type. Use `any` to let the compiler infer the type");
    }

    Token* identifier = parser_chomp_if(IDENTIFIER);
    if(identifier is null)
        ast_expected("an identifier");
    
    Token* equals = parser_chomp_if(EQUALS);
    AstNode* init_expr = null;
    if(equals isnot null) {
        // Expect an expression
        init_expr = ast_parse_expr(parser);
    }

    Token* semicolon = parser_chomp_if(SEMICOLON);

    AstNode* node = ast_create_node(AstNodeKindVariableDecl);
    node->data.scope_obj->var->name = identifier->value;
    node->data.scope_obj->var->init_expr = init_expr;
    node->data.scope_obj->var->is_local = !parser->is_in_global_context;
    node->data.scope_obj->var->is_comptime = cast(bool)(comptime_attr isnot null);
    node->data.scope_obj->var->is_mutable = cast(bool)(mutable_kwd isnot null);
    node->data.scope_obj->var->visibility = export_kwd isnot null ? VisibilityModePublic : VisibilityModePrivate;
    return node;
}

// FuncDecl
//      <Attributes> KEYWORD(export) KEYWORD(func) IDENTIFIER? LPAREN ParamList RPAREN LARROW TypeExpr (SEMICOLON / BLOCK)
// where <Attributes> can be one of:
//      | ATTR_NORETURN
//      | ATTR_COMPTIME
//      | ATTR_INLINE
//      | ATTR_NOINLINE
static AstNode* ast_parse_func_decl(Parser* parser) {
    // Attributes?
    if(!token_is_attribute(pc->kind))
        goto func_no_attrs;

    bool is_noreturn = false;
    bool is_comptime = false;
    bool is_inline = false;
    bool is_noinline = false;
    // Parse attributes
    switch(pc->kind) {
        case ATTR_NORETURN: is_noreturn = true; break;
        case ATTR_COMPTIME: is_comptime = true; break;
        case ATTR_INLINE: is_inline = true; break;
        case ATTR_NOINLINE: is_noinline = true; break;
        default: unreachable();
    }
    Token* attr = parser_chomp(1);
    if(token_is_attribute(pc->kind))
        ast_error("Can only have one attribute decorating a function");

func_no_attrs:;
    Token* export_kwd = parser_chomp_if(EXPORT);
    Token* func_kwd = parser_chomp_if(FUNC);
    if(func_kwd is null)
        ast_expected("`func` keyword");
    
    bool is_variadic = false;
    Token* identifier = parser_chomp_if(IDENTIFIER);
    AstNode* params = ast_parse_param_list(parser, &is_variadic);
    
    Token* larrow = parser_chomp_if(LARROW);
    AstNode* return_type_expr = ast_parse_type_expr(parser);
    if(return_type_expr is null)
        ast_expected("Return type expression. Use `void` if your function doesn't return anything");
    if(larrow is null and return_type_expr isnot null)
        ast_expected("trailing `->` after function prototype");
    
    bool no_body = false;
    AstNode* body = null;
    AstNode* node = ast_create_node(AstNodeKindFuncDecl);
    switch(pc->kind) {
        case SEMICOLON:
            parser_chomp(1);
            no_body = true;
            break;
        case LBRACE:
            body = ast_parse_block(parser);
            if(body is null)
                ast_expected("Expected a body");
            break;
        default:
            ast_expected("Semicolon or Function Body");
    } // switch

    node->data.decl->func_decl->name = identifier->value;
    node->data.decl->func_decl->params = params;
    node->data.decl->func_decl->return_type = return_type_expr;
    node->data.decl->func_decl->no_body = no_body;
    node->data.decl->func_decl->body = body;
    node->data.decl->func_decl->params = params;
    node->data.decl->func_decl->visibility = export_kwd isnot null ? VisibilityModePublic : VisibilityModePrivate;

    // Attributes
    node->data.decl->func_decl->is_comptime = is_comptime;
    node->data.decl->func_decl->is_noreturn = is_noreturn;
    node->data.decl->func_decl->is_inline = is_inline;
    node->data.decl->func_decl->is_noinline = is_noinline;

    return node;
}

// TopLevelComptime
//      ATTRIBUTE(comptime) BlockExpr
static AstNode* ast_parse_toplevel_comptime_expr(Parser* parser) {
    Token* comptime_attr = parser_chomp_if(ATTR_COMPTIME);
    if(comptime_attr is null)
        return null;
    
    Token* lbrace = parser_chomp_if(LBRACE);
    if(lbrace is null)
        ast_expected("Left brace `{`");

    AstNode* block = ast_parse_block_expr(parser);

    AstNode* out = ast_create_node(AstNodeKindTopLevelComptime);
    out->data.toplevel_comptime_expr->expr = block;
    return out;
}

// ParamList
//      (ParamDecl COMMA)* ParamDecl?
static AstNode* ast_parse_param_list(Parser* parser, bool* is_variadic) {
    Token* lparen = parser_expect_token(LPAREN);
    bool seen_varargs = false;
    Vec* params = vec_new(AstNode, 1);
    while(true) {
        if(parser_chomp_if(RPAREN) is null)
            break;
        AstNode* param = ast_parse_param_decl(parser);
        if(param isnot null) {
            vec_push(params, param);
        } else if((pc - 1)->kind is ELLIPSIS) {
            seen_varargs = true;
        }

        switch(pc->kind) {
            case COMMA: parser_chomp(1); break;
            case RPAREN: parser_chomp(1); break;
            case COLON: 
            case RBRACE: 
            case RSQUAREBRACK: 
                ast_expected("RPAREN");
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
    if(var_decl isnot null)
        return var_decl;
    
    AstNode* block_expr = ast_parse_block_expr(parser);
    if(block_expr isnot null)
        return block_expr;
    
    AstNode* if_expr = ast_parse_if_expr(parser);
    if(if_expr isnot null)
        return if_expr;
    
    AstNode* match_expr = ast_parse_match_expr(parser);
    if(match_expr isnot null)
        return match_expr;
    
    AstNode* assignment_expr = ast_parse_assignment_expr(parser);
    if(assignment_expr isnot null)
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
    Token* if_token = parser_chomp_if(IF);
    if(if_token is null)
        unreachable();

    Token* lparen = parser_chomp_if(LPAREN); // this is optional
    AstNode* condition = ast_parse_expr(parser);
    if(condition is null)
        ast_expected("condition");
    Token* rparen = parser_chomp_if(RPAREN); // this is optional

    if(lparen isnot null and rparen is null)
        ast_expected("closing `(`");
    if(lparen is null and rparen isnot null)
        ast_error("Extra `)` token not expected at this point");
    
    AstNode* if_body = null;
    AstNode* assignment_expr = null;
    AstNode* block_expr = ast_parse_block_expr(parser);
    if(block_expr is null) {
        assignment_expr = ast_parse_assignment_expr(parser);
        if(assignment_expr is null)
            ast_expected("block / assignment expression");
    }

    if(block_expr isnot null)
        if_body = block_expr;
    if(assignment_expr isnot null)
        if_body = assignment_expr;

    // If a semicolon is here, chomp it
    Token* semicolon = parser_chomp_if(SEMICOLON);
    
    AstNode* else_body = null;
    Token* else_kwd = parser_chomp_if(ELSE);
    if(else_kwd isnot null)
        else_body = ast_parse_block_expr(parser);
    
    if(else_body is null and semicolon is null)  
        ast_expected("Semicolon or `else` block");

    AstNode* node = ast_create_node(AstNodeKindIfExpr);
    node->data.expr->if_expr->condition = condition;
    node->data.expr->if_expr->if_body = if_body;
    node->data.expr->if_expr->has_else = else_body isnot null;
    node->data.expr->if_expr->else_node = else_body;
    return node;
}

// Labeled Statements
//      BlockLabel? (Block / LoopStatement)
static AstNode* ast_parse_labeled_statement(Parser* parser) {
    Token* label = ast_parse_block_label(parser);
    if(label is null)
        ast_expected("Label");

    AstNode* block = ast_parse_block_expr(parser);
    if(block isnot null) {
        block->data.stmt->block_stmt->label = label isnot null ? label->value : null;
        return block;
    }

    AstNode* loop_statement = ast_parse_loop_expr(parser);
    if(loop_statement isnot null) {
        loop_statement->data.expr->loop_expr->label = label isnot null ? label->value : null;
        return loop_statement;
    }

    if(label isnot null)
        ast_expected("either a block or loop statement");

    return null;
}

// LoopExpr
//      | ATTRIBUTE(inline)? (LoopWhileExpr / LoopCExpr / LoopInExpr)
static AstNode* ast_parse_loop_expr(Parser* parser) {
    Token* inline_attr = parser_chomp_if(ATTR_INLINE);
    AstNode* node = null;

    AstNode* loop_inf_expr = ast_parse_loop_inf_expr(parser);
    if(loop_inf_expr isnot null) {
        node = loop_inf_expr;
        goto outexpect;
    }

    AstNode* loop_c_expr = ast_parse_loop_c_expr(parser);
    if(loop_c_expr isnot null) {
        node = loop_c_expr;
        goto outexpect;
    }

    AstNode* loop_in_expr = ast_parse_loop_in_expr(parser);
    if(loop_in_expr isnot null) {
        node = loop_in_expr;
        goto outexpect;
    }

outexpect:
    if(node is null)
        ast_expected("loop expression");

    node->data.expr->loop_expr->is_inline = cast(bool)(inline_attr isnot null);
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
    if(block_expr isnot null)
        return block_expr;
    
    AstNode* assignment_expr = ast_parse_assignment_expr(parser);
    if(assignment_expr isnot null) {
        Token* semicolon = parser_chomp_if(SEMICOLON);
        return assignment_expr;
    }

    ast_expected("block or assignment expression");
    return null;
}

// BlockExpr
//      BlockLabel? Block
static AstNode* ast_parse_block_expr(Parser* parser) {
    switch(pc->kind) {
        case IDENTIFIER:
            if((pc->kind + 1) is COLON and (pc->kind + 2) is LBRACE) {
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
    if(lhs is null)
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
    if(rhs is null)
        ast_expected("an expression after assignment op");

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

static ast_prec lookup_precedence(TokenKind kind) {
    for(int i = 0; i < PRECEDENCE_TABLE_SIZE; i++) {
        if(precedence_table[i].tok_kind is kind)
            return precedence_table[i];
    }
    ast_error("Expected a valid assignment token op");
}

static AstNode* ast_parse_precedence(Parser* parser, UInt8 min_prec) {
    AstNode* node = ast_parse_prefix_expr(parser);
    if(node is null)
        return null;
    
    UInt8 banned_prec = 0;

    while(true) {
        ast_prec prec = lookup_precedence(pc->kind);
        if(prec.prec < min_prec or prec.prec is banned_prec)
            break;
        
        parser_chomp(1);
        Token* op_token = pc;

        AstNode* rhs = ast_parse_precedence(parser, prec.prec + 1);
        if(rhs is null)
            ast_error("Invalid token");
        
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
//      | KEYWORD(try)
static AstNode* ast_parse_prefix_expr(Parser* parser) {
    PrefixOpKind op;
    switch(parser->curr_tok->kind) {
        case NOT: op = PrefixOpKindBoolNot; break;
        case EXCLAMATION: op = PrefixOpKindNegation; break;
        case AND: op = PrefixOpKindAddrOf; break;
        case TRY: op = PrefixOpKindTry; break;
        default: return ast_parse_primary_expr(parser);
    }

    AstNode* lhs = ast_parse_prefix_expr(parser);
    if(lhs is null)
        ast_expected("prefix op expression");

    AstNode* node = ast_create_node(AstNodeKindPrefixOpExpr);
    node->data.prefix_op_expr->op = op;
    node->data.prefix_op_expr->expr = lhs;
    return node;
}

// TypeExpr
//      PrefixTypeOp*
// where PrefixTypeOp is one of:
//      | QUESTION  (?)
//      | SliceTypeStart 
//      | PointerTypeStart LPAREN Expr RPAREN 
//      | ArrayTypeStart
// where SliceTypeStart is
//      LSQUAREBRACK (COLON EXPR)? RSQUAREBRACK
// PointerTypeStart
//      | MULT (*)
//      | LSQUAREBRACK MULT (LETTERC / COLON Expr)? RSQUAREBRACK
// and ArrayTypeStart
//      LSQUAREBRACK Expr (COLON Expr)? RSQUAREBRACK
static AstNode* ast_parse_type_expr(Parser* parser) {
    AstNode* node = null;
    AstNode* expr = null;
    switch(pc->kind) {
        case QUESTION:
            expr = ast_parse_expr(parser);
            if(expr is null)
                ast_error("expression");
            node = ast_create_node(AstNodeKindPrefixOpExpr);
            node->data.prefix_op_expr->expr = expr;
            node->data.prefix_op_expr->op = PrefixOpKindOptional;
            return node;
        default:
            CORETEN_ENFORCE(false, "TODO");
    }
    return null;
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
            parser_chomp(1);
            label = ast_parse_break_label(parser);
            expr = ast_parse_expr(parser);

            node = ast_create_node(AstNodeKindBreak);
            node->data.stmt->branch_stmt->type = AstNodeBranchStatementBreak;
            node->data.stmt->branch_stmt->name = label isnot null ? label->value : null;
            node->data.stmt->branch_stmt->expr = expr;
            return node;
        case CONTINUE:
            parser_chomp(1);
            label = ast_parse_break_label(parser);
            node = ast_create_node(AstNodeKindBreak);
            node->data.stmt->branch_stmt->type = AstNodeBranchStatementContinue;
            node->data.stmt->branch_stmt->name = label isnot null ? label->value : null;
            node->data.stmt->branch_stmt->expr = null;
            return node;
        case ATTR_COMPTIME:
            parser_chomp(1);
            node = ast_create_node(AstNodeKindAttributeExpr);
            expr = ast_parse_expr(parser);
            if(expr is null)
                ast_expected("expression");
            
            node->data.expr->attr_expr->expr = expr;
            return node;
        case RETURN:
            parser_chomp(1);
            node = ast_create_node(AstNodeKindReturn);
            expr = ast_parse_expr(parser);
            node->data.stmt->return_stmt->expr = expr;
            return node;
        case IDENTIFIER:
            // `foo:`
            if((pc + 1)->kind is COLON) {
                switch((pc + 2)->kind) {
                    case ATTR_INLINE:
                        parser_chomp(3);
                        switch(pc->kind) {
                            case LOOP: return ast_parse_loop_expr(parser);
                            default: ast_expected("inlinable expression");
                        }
                    case LOOP:
                        parser_chomp(2);
                        return ast_parse_loop_expr(parser);
                    case LBRACE:
                        parser_chomp(2);
                        return ast_parse_block(parser);
                    default:
                        unreachable();
                }
            }
            break;
        case ATTR_INLINE:
            parser_chomp(1);
            switch(pc->kind) {
                case LOOP: return ast_parse_loop_expr(parser);
                default: ast_expected("inlinable expression");
            }
        case LOOP: 
            return ast_parse_loop_expr(parser);
        default:
            ast_error("Invalid parser pattern");
    } // switch(pc->kind)
    ast_error("Invalid parser pattern");
}

// Block
//      LBRACE Statement* RBRACE
static AstNode* ast_parse_block(Parser* parser) {
    Token* lbrace = parser_chomp_if(LBRACE);
    if(lbrace is null)
        ast_expected("LBRACE `{`");

    Vec* statements = vec_new(AstNode, 1);
    AstNode* statement = null;
    while((statement = ast_parse_statement(parser)) isnot null)
        vec_push(statements, statement);

    Token* rbrace = parser_chomp_if(RBRACE);
    if(rbrace is null)
        ast_expected("RBRACE `}`");
    
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
    if(type_expr is null)
        ast_expected("type expression");
    
    Token* lbrace = parser_chomp_if(LBRACE);
    if(lbrace is null)
        return type_expr;
    
    Vec* fields = null;
    AstNode* field_init = ast_parse_field_init(parser);
    if(field_init isnot null) {
        fields = vec_new(AstNode, 1);
        vec_push(fields, field_init);
        while(true) {
            switch(pc->kind) {
                case COMMA: parser_chomp(1); break;
                case RBRACE: parser_chomp(1); break;
                case COLON: case RPAREN: case RSQUAREBRACK:
                    ast_expected("RBRACE");
                default:
                    // Likely just a missing comma; warn, but continue parsing
                    WARN("missing comma");
            }
            Token* rbrace = parser_chomp_if(RBRACE);
            if(rbrace isnot null)
                break;
            AstNode* field_init = ast_parse_field_init(parser);
            if(field_init is null)
                ast_expected("field init");
            vec_push(fields, field_init);
        } // while(true)
        Token* comma = parser_chomp_if(COMMA);
        AstNode* node = ast_create_node(AstNodeKindStructExpr);
        node->data.expr->init_expr->kind = InitExprKindStruct;
        node->data.expr->init_expr->entries = fields;
        return node;
    }

    AstNode* node = ast_create_node(AstNodeKindArrayInitExpr);
    node->data.expr->init_expr->kind = InitExprKindArray;

    AstNode* expr = ast_parse_expr(parser);
    if(expr isnot null) {
        Vec* fields = vec_new(AstNode, 1);
        vec_push(fields, expr);
        Token* comma = null;
        while(pc->kind isnot COMMA) {
            AstNode* exp = ast_parse_expr(parser);
            if(exp is null)
                break;
            vec_push(fields, exp);
        }
        node->data.expr->init_expr->entries = fields;
        return node;
    }

    Token* rbrace = parser_chomp_if(RBRACE);
    if(rbrace is null)
        ast_expected("RBRACE");
    
    return node;
}

/* WIP */
/*
// SuffixExpr
//      PrimaryTypeExpr SuffixOp* FuncCallArgs
//      PrimaryTypeExpr (SuffixOp / FuncCallArgs)*
// where FuncCallArgs are:
//      LPAREN ExprList RPAREN
// and ExprList is
//      (Expr COMMA)* Expr?
static AstNode* ast_parse_suffix_expr(Parser* parser) {
    AstNode* node = ast_parse_primary_type_expr(parser);
    while(true) {
        AstNode* suffix_op = ast_parse_suffix_op(parser);
        if(suffix_op is null)
            break;
        node = suffix_op;
    }
    Token* lparen = parser_chomp_if(LPAREN);
    if(lparen is null) {
        WARN("expected param list");
        return node;
    }

    Vec* params = vec_new(AstNode, 1);
    AstNode* param = null;
    while(parser_chomp(1)->kind isnot RPAREN) {
        param = ast_parse_expr(parser);
        if(param is null)
            break;
        vec_push(params, param);
        switch(pc->kind) {
            case COMMA: parser_chomp(1);
            case RPAREN: parser_chomp(1); break;
            case COLON: case RBRACE: case RSQUAREBRACK:
                ast_expected("RBRACE");
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
// and LabeledTypeExpr is one of:
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
            parser_chomp(1);
            return node;
        case INTEGER:
            node = ast_create_node(AstNodeKindIntLiteral);
            node->data.literal->int_value->value = pc->value;
            parser_chomp(1);
            return node;
        case FLOAT_LIT:
            node = ast_create_node(AstNodeKindFloatLiteral);
            node->data.literal->float_value->value = pc->value;
            parser_chomp(1);
            return node;
        case UNREACHABLE:
            node = ast_create_node(AstNodeKindUnreachable);
            parser_chomp(1);
            return node;
        case STRING:
            node = ast_create_node(AstNodeKindStringLiteral);
            parser_chomp(1);
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
            if(expr is null)
                ast_expected("type expr");
            node->data.expr->attr_expr->expr = expr;
            return node;
        case IDENTIFIER:
            switch((pc + 1)->kind) {
                case COLON:
                    switch((pc + 2)->kind) {
                        case ATTR_INLINE:
                            parser_chomp(3);
                            switch(pc->kind) {
                                case LOOP: return ast_parse_loop_expr(parser);
                                default: ast_expected("inlinable expression");
                            }
                        case LOOP:
                            parser_chomp(2);
                            return ast_parse_loop_expr(parser);
                        case LBRACE:
                            parser_chomp(2);
                            return ast_parse_block(parser);
                        default:
                            node = ast_create_node(AstNodeKindIdentifier);
                            parser_chomp(1);
                            return node;
                    }
                default:
                    node = ast_create_node(AstNodeKindIdentifier);
                    parser_chomp(1);
                    return node;
            }
            break;
        case ATTR_INLINE:
            parser_chomp(1);
            switch(pc->kind) {
                case LOOP: return ast_parse_loop_expr(parser);
                default: ast_expected("inlinable expression");
            }
            break;
        case LOOP: return ast_parse_loop_expr(parser);
        case DOT:
            switch((pc + 1)->kind) {
                case IDENTIFIER:
                    node = ast_create_node(AstNodeKindIdentifier);
                    parser_chomp(1);
                    return node;
                default: return null;
            }
            break;
        case LPAREN:
            node = ast_create_node(AstNodeKindGroupedExpr);
            expr = ast_parse_expr(parser);
            if(expr is null)
                ast_expected("expression");
            tok = parser_chomp_if(RPAREN);
            if(tok is null)
                ast_expected("RPAREN");

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
    Token* match_kwd = parser_chomp_if(MATCH);
    if(match_kwd is null)
        ast_expected("`match` keyword");
    
    Token* lparen = parser_chomp_if(LPAREN); // this is optional
    AstNode* expr = ast_parse_expr(parser);
    if(expr is null)
        ast_expected("expression");
    Token* rparen = parser_chomp_if(RPAREN); // this is optional
    
    Token* lbrace = parser_expect_token(RBRACE); // required

    // Branches
    AstNode* branch_node = ast_parse_match_branch(parser);
    if(branch_node is null)
        ast_expected("branches for `match`");
    Vec* branches = vec_new(AstNode, 1);
    do {
        vec_push(branches, branch_node);
    } while((branch_node = ast_parse_match_branch(parser)) isnot null);

    // Parse any trailing comma
    Token* comma = parser_chomp_if(COMMA);

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
    Token* when_kwd = parser_chomp_if(WHEN);
    if(when_kwd is null)
        ast_expected("`when` keyword");

    // MatchClause
    AstNode* node = ast_parse_match_clause(parser);
    CORETEN_ENFORCE(node->kind is AstNodeKindMatchBranch);
    if(node is null)
        return null;
    
    Token* equals_arrow = parser_chomp_if(EQUALS_ARROW); // `=>`
    if(equals_arrow is null)
        ast_error("Missing token `=>` after `when`");

    AstNode* expr = ast_parse_assignment_expr(parser);
    if(expr is null) {
        expr = ast_parse_block_expr(parser);
        if(expr is null)
            ast_expected("An assignment/block expression after the `when` clause");
    }

    node->data.expr->match_branch_expr->block_node = expr;
    return node;
}

// MatchCase
//      Expr (DDOT Expr)*
static AstNode* ast_parse_match_clause(Parser* parser) {
    AstNode* expr = ast_parse_expr(parser);
    if(expr is null) {
        WARN("Expression is null");
        return null;
    }

    AstNode* out = ast_create_node(AstNodeKindMatchBranch);
    out->data.expr->match_branch_expr->is_range = false;
    AstNode* cond_node = expr;

    Token* dot_dot = parser_chomp_if(DDOT);
    if(dot_dot isnot null) {
        // Range-based
        AstNode* expr2 = ast_parse_expr(parser);
        if(expr2 is null)
            ast_expected("Expected expression after `..`");

        AstNode* range = ast_create_node(AstNodeKindMatchRange);
        range->data.expr->match_range_expr->begin = expr;
        range->data.expr->match_range_expr->end = expr2;
        cond_node = range;
        out->data.expr->match_branch_expr->is_range = true;
    }
    
    out->data.expr->match_branch_expr->cond_node = cond_node;

    CORETEN_ENFORCE(parser_peek_next(parser)->kind is EQUALS_ARROW, "Expected `=>` after expression.");

    return out;
}

// BreakLabel
//      COLON IDENTIFIER
static Token* ast_parse_break_label(Parser* parser) {
    Token* colon = parser_chomp_if(COLON);
    if(colon is null)
        return null;
        
    Token* ident = parser_expect_token(IDENTIFIER);
    return ident;
}

// BlockLabel
//      IDENTIFIER COLON
static Token* ast_parse_block_label(Parser* parser) {
    Token* ident = parser_chomp_if(IDENTIFIER);
    if(ident is null)
        return null;
    
    Token* colon = parser_chomp_if(COLON);
    if(colon is null)
        return null;

    return ident;
}

// FieldInit
//      DOT IDENTIFIER EQUALS Expr
static AstNode* ast_parse_field_init(Parser* parser) {
    if((pc + 0)->kind is DOT and
       (pc + 1)->kind is IDENTIFIER and
       (pc + 2)->kind is EQUALS) {
            parser_chomp(3);
            AstNode* expr = ast_parse_expr(parser);
            if(expr is null)
                ast_expected("expression");
            return expr;
    }
    return null;
}

// SuffixOp
//      | LBRACKET Expr (DOT2 (Expr (COLON Expr)?)?)? RBRACKET
//      | DOT IDENTIFIER
static AstNode* ast_parse_suffix_op(Parser* parser) {
    Token* lbrace = parser_chomp_if(LBRACE);
    if(lbrace isnot null) {
        AstNode* lower = ast_parse_expr(parser);
        AstNode* upper = null;
        Token* ellipsis = parser_chomp_if(ELLIPSIS);
        if(ellipsis isnot null) {
            AstNode* sentinel = null;
            upper = ast_parse_expr(parser);
            Token* colon = parser_chomp_if(COLON);
            if(colon isnot null) {
                sentinel = ast_parse_expr(parser);
            }
            Token* rbrace = parser_expect_token(RBRACE);

            AstNode* node = ast_create_node(AstNodeKindSliceExpr);
            node->data.expr->slice_expr->lower = lower;
            node->data.expr->slice_expr->upper = upper;
            node->data.expr->slice_expr->sentinel = sentinel;
            return node;
        }

        Token* rbrace = parser_expect_token(RBRACE);

        AstNode* node = ast_create_node(AstNodeKindArrayAccessExpr);
        node->data.array_access_expr->subscript = lower;
        return node;
    }

    Token* dot = parser_chomp_if(DOT);
    if(dot isnot null) {
        Token* identifier = parser_expect_token(IDENTIFIER);
        AstNode* node = ast_create_node(AstNodeKindFieldAccessExpr);
        node->data.field_access_expr->field_name = identifier->value;
        return node;
    }

    return null;
}

// StringLiteral
static AstNode* ast_parse_string_literal(Parser* parser) {
    if(pc->kind is STRING) {
        parser_chomp(1);
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
    
//     Vec* stmts = vec_new(AstNode, 1);
//     // Imports
//     for(;;) {
//         if(pc->kind is IMPORT)
//     }
// }

AstNode* return_result(Parser* parser) {
    return ast_parse_block_expr(parser);
}