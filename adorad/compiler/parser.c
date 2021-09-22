#include <adorad/compiler/ast.h>
#include <adorad/compiler/parser.h>

#define pt      parser->toklist
#define pc      parser->curr_tok

#define nodepush(node)              vec_push(parser->nodelist, node)

#define parser_chomp(n)             chomp(parser, n)
#define parser_chomp_if(kind)       chomp_if(parser, kind)
#define parser_expect_token(kind)   expect_token(parser, kind)

#define ast_error(...)              panic(ErrorParseError, __VA_ARGS__)
#define ast_expected(...)           (ast_error("Expected %s; got `%s`", __VA_ARGS__, tokenHash[pc->kind]))
#define ast_unexpected(...)         (panic(ErrorUnexpectedToken, __VA_ARGS__))

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
    if((parser->curr_tok + 1) != null && (parser->curr_tok + 1)->kind == tokenkind)
        return chomp(parser, 1);

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
        return parser_chomp(1);
        
    ast_expected("`%s`", tokenHash[tokenkind]);
}

AstNode* ast_create_node(AstNodeKind kind) {
    AstNode* node = cast(AstNode*)calloc(1, sizeof(AstNode));
    node->kind = kind;
    return node;
}

// TopLevelDecl
//      | ModuleStatement
//      | ImportStatement
//      | KEYWORD(alias) Expr
//      | ATTRIBUTE(comptime) (Expr / BlockExpr)
//      | KEYWORD(export)? VariableDecl
//      | (ATTRIBUTE(INLINE) / ATTRIBUTE(NOINLINE) / ATTRIBUTE(NORETURN))? FuncDecl
//      | StructDecl
//      | EnumDecl
static AstNode* parse_toplevel_decl(Parser* parser) {
    AstNode* module = parse_module_statement(parser);
    if(module != null)
        return module;
    
    AstNode* import = parse_import_statement(parser);
    if(import != null)
        return null;
    
    AstNode* alias = parse_alias_decl(parser);
    if(alias != null)
        return alias;
    
    AstNode* comptime = parse_attribute_expr(parser, ATTR_COMPTIME);
    if(comptime != null)
        return comptime;

    AstNode* variable = parse_variable_decl(parser);
    if(variable != null)
        return variable;

    AstNode* func_decl = parse_func_decl(parser);
    if(func_decl != null)
        return func_decl;
    
    AstNode* container_decl = parse_container_decl(parser);
    if(container_decl != null)
        return container_decl;
    
    return null;
}

// ModuleStatement
//      KEYWORD(module) Statement SEMICOLON?
static AstNode* ast_parse_module_statement(Parser* parser) {
    Token* module_kwd = parser_chomp_if(MODULE);
    if(module_kwd == null)
        ast_expected("`module` keyword");
    
    Token* module_name = parser_chomp_if(IDENTIFIER);
    if(module_name == null)
        ast_expected("module name");
    
    Token* semicolon = parser_chomp_if(SEMICOLON); // this is optional

    AstNode* out = ast_create_node(AstNodeKindModuleStatement);
    out->data.stmt->module_stmt->name = module_name->value;
    return out;
}

// ImportStatement
//      | KEYWORD(import) Statement
static AstNode* parse_import_statement(Parser* parser) {
    Token* import_kwd = parser_chomp_if(IMPORT);
    if(import_kwd == null)
        ast_expected("`import` keyword");
    
    Token* import_name = parser_chomp_if(IDENTIFIER);
    if(import_name == null)
        ast_expected("import name");
    
    Token* semicolon = parser_chomp_if(SEMICOLON); // this is optional

    AstNode* out = ast_create_node(AstNodeKindImportStatement);
    out->data.stmt->import_stmt->name = import_name->value;
    return out;
}

// AliasDecl
//      KEYWORD(alias) IDENTIFER KEYWORD(as) IDENTIFIER SEMICOLON?
static AstNode* ast_parse_alias_decl(Parser* parser) {
    Token* alias_kwd = parser_chomp_if(ALIAS);
    if(alias_kwd == null)
        unreachable();
    
    Token* original = parser_chomp_if(IDENTIFIER);
    if(original == null)
        ast_expected("an identifier after `alias`");
    
    Token* as_kwd = parser_chomp_if(AS);
    if(as_kwd == null)
        ast_expected("`as` keyword");
    
    Token* aliased = parser_chomp_if(IDENTIFIER);
    if(aliased == null)
        ast_expected("an alias for identifier `%s`", original->value);
    
    Token* semicolon = parser_chomp_if(SEMICOLON); // this is optional

    AstNode* out = ast_create_node(AstNodeKindAliasDeclExpr);
    out->data.decl->alias_decl->original = original->value;
    out->data.decl->alias_decl->alias = aliased->value;
    return out;
}

// VariableDecl
//      ATTRIBUTE(comptime)? KEYWORD(export)? KEYWORD(mutable)? (TypeExpr / KEYWORD(any)) IDENTIFIER (EQUALS EXPR)? SEMICOLON?
static AstNode* ast_parse_variable_decl(Parser* parser) {
    Token* comptime_attr = parser_chomp_if(ATTR_COMPTIME);
    Token* export_kwd = parser_chomp_if(EXPORT);
    Token* mutable_kwd = parser_chomp_if(MUTABLE);

    AstNode* type_expr = ast_parse_type_expr(parser);
    Token* any = null;
    if(type_expr == null) {
        any = parser_chomp_if(ANY);
        if(any == null)
            ast_expected("a type. Use `any` to let the compiler infer the type");
    }

    Token* identifier = parser_chomp_if(parser);
    if(identifier == null)
        ast_expected("an identifier");
    
    Token* equals = parser_chomp_if(EQUALS);
    AstNode* init_expr = null;
    if(equals != null) {
        // Expect an expression
        init_expr = ast_parse_expr(parser);
    }

    Token* semicolon = parser_chomp_if(SEMICOLON);

    AstNode* out = ast_create_node(AstNodeKindVariableDecl);
    out->data.scope_obj->var->name = identifier->value;
    out->data.scope_obj->var->init_expr = init_expr;
    out->data.scope_obj->var->is_local = !parser->is_in_global_context;
    out->data.scope_obj->var->is_comptime = cast(bool)(comptime_attr != null);
    out->data.scope_obj->var->is_mutable = cast(bool)(mutable_kwd != null);
    out->data.scope_obj->var->visibility = export_kwd != null ? VisibilityModePublic : VisibilityModePrivate;
    return out;
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

func_no_attrs:
    Token* export_kwd = parser_chomp_if(EXPORT);
    Token* func_kwd = parser_chomp_if(FUNC);
    if(func_kwd == null)
        ast_expected("`func` keyword");
    
    bool is_variadic = false;
    Token* identifier = parser_chomp_if(IDENTIFIER);
    Vec* params = ast_parse_param_list(parser, &is_variadic);
    Token* larrow = parser_chomp_if(LARROW);
    AstNode* return_type_expr = ast_parse_type_expr(parser);
    if(return_type_expr == null)
        ast_expected("Return type expression. Use `void` if your function doesn't return anything");
    if(larrow == null and return_type_expr != null)
        ast_expected("trailing `->` after function prototype");
    
    bool no_body = false;
    AstNode* body = null;
    AstNode* out = ast_create_node(AstNodeKindFuncDecl);
    switch(pc->kind) {
        case SEMICOLON:
            parser_chomp(1);
            no_body = true;
            break;
        case LBRACE:
            body = ast_parse_block(parser);
            if(body == null)
                ast_expected("Expected a body");
            break;
        default:
            ast_expected("Semicolon or Function Body");
    } // switch

    out->data.decl->func_decl->name = identifier->value;
    out->data.decl->func_decl->params = params;
    out->data.decl->func_decl->return_type = return_type_expr;
    out->data.decl->func_decl->body = body;
    out->data.decl->func_decl->params = params;
    out->data.decl->func_decl->visibility = export_kwd != null ? VisibilityModePublic : VisibilityModePrivate;

    // Attributes
    out->data.decl->func_decl->is_variadic = is_variadic;
    out->data.decl->func_decl->is_comptime = is_comptime;
    out->data.decl->func_decl->is_noreturn = is_noreturn;
    out->data.decl->func_decl->is_inline = is_inline;
    out->data.decl->func_decl->is_noinline = is_noinline;

    return out;
}

// ParamList
//      (ParamDecl COMMA)* ParamDecl?
static AstNode* ast_parse_param_list(Parser* parser) {
    Token* lparen = parser_expect_token(LPAREN);
    bool seen_varargs = false;
    Vec* params = vec_new(AstNode, 1);
    while(true) {
        if(parser_chomp_if(RPAREN) == null)
            break;
        AstNode* param = parse_param_decl(parser);
        if(param != null) {
            vec_push(params, param);
        } else if((pc - 1)->kind == ELLIPSIS) {
            seen_varargs = true;
        }

        switch(pc->kind) {
            case COMMA: parser_chomp(1);
            case RPAREN: parser_chomp(1); break;
            case COLON: case RBRACE: case RSQUAREBRACK: 
                ast_expected("RPAREN");
            default:
                WARN(Expected comma);
                break;
        }
    }

    AstNode* out = ast_create_node(AstNodeKindParamList);
    out->data.param_list->is_variadic = cast(bool)seen_varargs;
    out->data.param_list->params = params;
    return out;
}

// Statement
//      | VariableDecl
//      | BlockExpr
//      | IfExpr
//      | LabeledStatement
//      | MatchExpr
//      | AssignmentExpr SEMICOLON?
static AstNode* parse_statement(Parser* parser) {
    AstNode* var_decl = ast_parse_variable_decl(parser);
    if(var_decl != null)
        return var_decl;
    
    AstNode* block_expr = parse_block_expr(parser);
    if(block_expr != null)
        return block_expr;
    
    AstNode* if_expr = parse_if_expr(parser);
    if(if_expr != null)
        return if_expr;
    
    AstNode* match_expr = parse_match_expr(parser);
    if(match_expr != null)
        return match_expr;
    
    AstNode* assignment_expr = parse_assignment_expr(parser);
    if(assignment_expr != null)
        return assignment_expr;
    
    WARN(Hmmm could not parse a suitable statement. Returning null);
    return null;
}

/*
// ContainerMembers
//      ContainerDeclarations (ContainerField COMMA)* (ContainerField / ConstainerDeclarations)
// ContainerDeclarations
//      | TestDecl ContainerDeclarations
//      | TopLevelComptime ContainerDeclarations
//      | KEYWORD(export)? TopLevelDecl ContainerDeclarations
// TopLevelComptime
//      ATTRIBUTE(comptime) BlockExpr
static AstNode* parse_container_members(pars) {
    while(true) {
        switch(pc->kind) {
            case ATTR_COMPTIME:
                Token* comptime_attr = parser_chomp(1);
                switch(pc->kind) {
                    // // Currently, a top-level comptime decl is as follows:
                    // //      `[comptime] { ... }`
                    // // TODO: Support single statements as well, like
                    // //      `[comptime] UInt32 i = 2` 
                    // case IDENTIFIER:
                    //     pc += 1;
                    case LBRACE:
                        AstNode* block = parse_block(parser);
                        if(block != null) {
                            AstNode* out = ast_create_node(AstNodeKindAttributeExpr);
                            out->data.expr->attr_expr->kind = AttributeKindCompileTime;
                            out->data.expr->attr_expr->expr = block;
                            nodepush(out);
                        }
                        break;
                    default:
                        ast_error("Expected a `[comptime]` block");
                }
                break;
            
            // TODO: Handle more toplevel decls
            default:
                unreachable();
        } // switch(pc->kind)
    } // while(true)
}
*/

// IfExpr
//      | IfPrefix BlockExpr (KEYWORD(else) Statement)?
//      | IfPrefix AssignmentExpr (SEMICOLON / KEYWORD(else) Statement)?
// where IfPrefix is:
//      KEYWORD(if) LPAREN? Expr RPAREN?
static AstNode* parse_if_expr(Parser* parser) {
    Token* if_token = parser_chomp_if(IF);
    if(if_token == null)
        unreachable();

    Token* lparen = parser_chomp_if(LPAREN); // this is optional
    AstNode* condition = parse_expr(parser);
    if(condition == null)
        ast_expected("condition");
    Token* rparen = parser_chomp_if(RPAREN); // this is optional

    if(lparen != null and rparen == null)
        ast_expected("closing `(`");
    if(lparen == null and rparen != null)
        ast_error("Extra `)` token not expected at this point");
    
    AstNode* if_body = null;
    AstNode* assignment_expr = null;
    AstNode* block_expr = parse_block_expr(parser);
    if(block_expr == null) {
        assignment_expr = parse_assignment_expr(parser);
        if(assignment_expr == null)
            ast_expected("block / assignment expression");
    }

    if(block_expr != null)
        if_body = block_expr;
    if(assignment_expr != null)
        if_body = assignment_expr;

    // If a semicolon is here, chomp it
    Token* semicolon = parser_chomp_if(SEMICOLON);
    
    AstNode* else_body = null;
    Token* else_kwd = parser_chomp_if(ELSE);
    if(else_kwd != null)
        else_body = body_parser(parser);
    
    if(else_body == null and semicolon == null)  
        ast_expected("Semicolon or `else` block");

    AstNode* out = ast_create_node(AstNodeKindIfExpr);
    out->data.expr->if_expr->condition = condition;
    out->data.expr->if_expr->if_body = if_body;
    out->data.expr->if_expr->has_else = else_body != null;
    out->data.expr->if_expr->else_node = else_body;
    return out;
}

// Labeled Statements
//      BlockLabel? (Block / LoopStatement)
static AstNode* parse_labeled_statement(Parser* parser) {
    Token* label = parse_block_label(parser);
    if(label == null)
        ast_expected("Label");

    AstNode* block = parse_block_expr(parser);
    if(block != null) {
        block->data.stmt->block_stmt->label = label != null ? label->value : null;
        return block;
    }

    AstNode* loop_statement = parse_loop_expr(parser);
    if(loop_statement != null) {
        loop_statement->data.expr->loop_expr->label = label != null ? label->value : null;
        return loop_statement;
    }

    if(label != null)
        ast_expected("either a block or loop statement");

    return null;
}

// LoopExpr
//      | ATTRIBUTE(inline)? (LoopWhileExpr / LoopCExpr / LoopInExpr)
static AstNode* parse_loop_expr(Parser* parser) {
    Token* inline_attr = parser_chomp_if(ATTR_INLINE);
    AstNode* out = null;

    AstNode* loop_inf_expr = parse_loop_inf_expr(parser);
    if(loop_inf_expr != null) {
        out = loop_inf_expr;
        goto outexpect;
    }

    AstNode* loop_c_expr = parse_loop_c_expr(parser);
    if(loop_c_expr != null) {
        out = loop_c_expr;
        goto outexpect;
    }

    AstNode* loop_in_expr = parse_loop_in_expr(parser);
    if(loop_in_expr != null) {
        out = loop_in_expr;
        goto outexpect;
    }

outexpect:
    if(out == null)
        ast_expected("loop expression");

    out->data.expr->loop_expr->is_inline = cast(bool)(inline_attr != null);
    return out;
}

/*
    TODO: Add implementation for:
        1. parse_loop_inf_expr(parser)
        2. parse_loop_c_expr(parser)
        3. parse_loop_in_expr(parser)
*/
static AstNode* parse_loop_inf_expr(Parser* parser) {
    CORETEN_ENFORCE(false, "TODO");
    return null;
}

static AstNode* parse_loop_c_expr(Parser* parser) {
    CORETEN_ENFORCE(false, "TODO");
    return null;
}

static AstNode* parse_loop_in_expr(Parser* parser) {
    CORETEN_ENFORCE(false, "TODO");
    return null;
}

// BlockExprStatement
//      | BlockExpr
//      | AssignmentExpr SEMICOLON?
static AstNode* parse_block_expr_statement(Parser* parser) {
    AstNode* block_expr = parse_block_expr(parser);
    if(block_expr != null)
        return block_expr;
    
    AstNode* assignment_expr = parse_assignment_expr(parser);
    if(assignment_expr != null) {
        Token* semicolon = parser_chomp_if(SEMICOLON);
        return assignment_expr;
    }

    ast_expected("block or assignment expression");
    return null;
}

// BlockExpr
//      BlockLabel? Block
static AstNode* parse_block_expr(Parser* parser) {
    switch(pc->kind) {
        case IDENTIFIER:
            if((pc->kind + 1) == COLON and (pc->kind + 2) == LBRACE) {
                pc += 2;
                return parse_block(parser);    
            } else {
                return null;
            }
            break;
        case LBRACE:
            return parse_block(parser);
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
static AstNode* parse_assignment_expr(Parser* parser) {
    AstNode* lhs = parse_expr(parser);
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

    AstNode* rhs = parse_expr(parser);
    if(rhs == null)
        ast_expected("an expression after assignment op");

    AstNode* out = ast_create_node(AstNodeKindBinaryOpExpr);
    out->data.expr->binary_op_expr->op = op;
    out->data.expr->binary_op_expr->lhs = lhs;
    out->data.expr->binary_op_expr->rhs = rhs;
    return out;
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
        if(precedence_table[i].tok_kind == kind)
            return precedence_table[i];
    }
    ast_error("Expected a valid assignment token op");
}

static AstNode* parse_precedence(Parser* parser, UInt8 min_prec) {
    AstNode* out = parse_prefix_expr(parser);
    if(out == null)
        return null;
    
    UInt8 banned_prec = 0;

    while(true) {
        ast_prec prec = lookup_precedence(pc->kind);
        if(prec.prec < min_prec or prec.prec == banned_prec)
            break;
        
        parser_chomp(1);
        Token* op_token = pc;

        AstNode* rhs = parse_precedence(parser, prec.prec + 1);
        if(rhs == null)
            ast_error("Invalid token");
        
        out->data.expr->binary_op_expr->lhs = out;
        out->data.expr->binary_op_expr->op = prec.bin_kind;
        out->data.expr->binary_op_expr->rhs = rhs;

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
    return out;
}

// PrefixExpr
//      PrefixOp* PrimaryExpr
// where PrefixOp is one of:
//      | EXCLAMATION   (!)
//      | MINUS         (-)
//      | TILDA         (~)
//      | AND           (&)
//      | KEYWORD(try)
static AstNode* parse_prefix_expr(Parser* parser) {
    PrefixOpKind op;
    switch(parser->curr_tok->kind) {
        case NOT: op = PrefixOpKindBoolNot; break;
        case EXCLAMATION: op = PrefixOpKindNegation; break;
        case AND: op = PrefixOpKindAddrOf; break;
        case TRY: op = PrefixOpKindTry; break;
        default: return parse_primary_expr(parser);
    }

    AstNode* lhs = parse_prefix_expr(parser);
    if(lhs == null)
        ast_expected("prefix op expression");

    AstNode* out = ast_create_node(AstNodeKindPrefixOpExpr);
    out->data.prefix_op_expr->op = op;
    out->data.prefix_op_expr->expr = lhs;
    return out;
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
static AstNode* parse_type_expr(Parser* parser) {
    AstNode* out = null;
    switch(pc->kind) {
        case QUESTION:
            AstNode* expr = parse_expr(parser);
            if(expr == null)
                ast_error("expression");
            out = ast_create_node(AstNodeKindPrefixOpExpr);
            out->data.prefix_op_expr->expr = expr;
            out->data.prefix_op_expr->op = PrefixOpKindOptional;
            return out;
        default:
            CORETEN_ENFORCE(false, "TODO");
    }
}

// PrimaryExpr
//      | IfExpr
//      | KEYWORD(break) BreakLabel? Expr?
//      | KEYWORD(continue) BreakLabel?
//      | ATTRIBUTE(comptime) Expr
//      | KEYWORD(return) Expr?
//      | BlockLabel? ATTRIBUTE(inline)? LoopExpr
//      | Block
static AstNode* parse_primary_expr(Parser* parser) {
    switch(pc->kind) {
        case IF: return parse_if_expr(parser);
        case BREAK: 
            parser_chomp(1);
            Token* label = parse_break_label(parser);
            AstNode* expr = parse_expr(parser);

            AstNode* out = ast_create_node(AstNodeKindBreak);
            out->data.stmt->branch_stmt->type = AstNodeBranchStatementBreak;
            out->data.stmt->branch_stmt->name = label != null ? label->value : null;
            out->data.stmt->branch_stmt->expr = expr;
            return out;
        case CONTINUE:
            parser_chomp(1);
            Token* label = parse_break_label(parser);
            AstNode* out = ast_create_node(AstNodeKindBreak);
            out->data.stmt->branch_stmt->type = AstNodeBranchStatementContinue;
            out->data.stmt->branch_stmt->name = label != null ? label->value : null;
            out->data.stmt->branch_stmt->expr = null;
            return out;
        case ATTR_COMPTIME:
            parser_chomp(1);
            AstNode* out = ast_create_node(AstNodeKindAttributeExpr);
            AstNode* expr = parse_expr(parser);
            if(expr == null)
                ast_expected("expression");
            
            out->data.expr->attr_expr->expr = expr;
            return out;
        case RETURN:
            parser_chomp(1);
            AstNode* out = ast_create_node(AstNodeKindReturn);
            AstNode* expr = parse_expr(parser);
            out->data.stmt->return_stmt->expr = expr;
            return out;
        case IDENTIFIER:
            // `foo:`
            if((pc + 1)->kind == COLON) {
                switch((pc + 2)->kind) {
                    case ATTR_INLINE:
                        parser_chomp(3);
                        switch(pc->kind) {
                            case LOOP: return parse_loop_expr(parser);
                            default: ast_expected("inlinable expression");
                        }
                    case LOOP:
                        parser_chomp(2);
                        return parse_loop_expr(parser);
                    case LBRACE:
                        parser_chomp(2);
                        return parse_block(parser);
                    default:
                        unreachable();
                }
            }
            break;
        case ATTR_INLINE:
            parser_chomp(1);
            switch(pc->kind) {
                case LOOP: return parse_loop_expr(parser);
                default: ast_expected("inlinable expression");
            }
        case LOOP: 
            return parse_loop_expr(parser);
        default:
            ast_error("Invalid parser pattern");
    } // switch(pc->kind)
}

// Block
//      LBRACE Statement* RBRACE
static AstNode* parse_block(Parser* parser) {
    Token* lbrace = parser_chomp_if(LBRACE);
    if(lbrace == null)
        ast_expected("LBRACE `{`");

    Vec* statements = vec_new(AstNode, 1);
    AstNode* statement = null;
    while((statement = parse_statement(parser)) != null)
        vec_push(statements, statement);

    Token* rbrace = parser_chomp_if(RBRACE);
    if(rbrace == null)
        ast_expected("RBRACE `}`");
    
    AstNode* out = ast_create_node(AstNodeKindBlock);
    out->data.stmt->block_stmt->statements = statements;
    return out;
}

/* WIP */
// BraceSuffixExpr
//      TypeExpr InitList?
// where InitList is:
//      | LBRACE FieldInit (COMMA FieldInit)* COMMA? RBRACE
//      | LBRACE Expr (COMMA Expr)* COMMA? RBRACE
//      | LBRACE RBRACE
static AstNode* parse_brace_suffix_expr(Parser* parser) {
    AstNode* type_expr = parse_type_expr(parser);
    if(type_expr == null)
        ast_expected("type expression");
    
    Token* lbrace = parser_chomp_if(LBRACE);
    if(lbrace == null)
        return type_expr;
    
    Vec* fields = null;
    AstNode* field_init = parse_field_init(parser);
    if(field_init != null) {
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
                    WARN(missing comma);
            }
            Token* rbrace = parser_chomp_if(RBRACE);
            if(rbrace != null)
                break;
            AstNode* field_init = parse_field_init(parser);
            if(field_init == null)
                ast_expected("field init");
            vec_push(fields, field_init);
        } // while(true)
        Token* comma = parser_chomp_if(COMMA);
        AstNode* out = ast_create_node(AstNodeKindStructExpr);
        out->data.expr->init_expr->kind = InitExprKindStruct;
        out->data.expr->init_expr->entries = fields;
        return out;
    }

    AstNode* out = ast_create_node(AstNodeKindArrayInitExpr);
    out->data.expr->init_expr->type = InitExprKindArray;

    AstNode* expr = parse_expr(parser);
    if(expr != null) {
        Vec* fields = vec_new(AstNode, 1);
        vec_push(fields, expr);
        Token* comma = null;
        while(pc->kind != COMMA) {
            AstNode* exp = parse_expr(parser);
            if(exp == null)
                break;
            vec_push(fields, exp);
        }
        out->data.expr->init_expr->entries = fields;
        return out;
    }

    Token* rbrace = parser_chomp_if(RBRACE);
    if(rbrace == null)
        ast_expected("RBRACE");
    
    return out;
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
static AstNode* parse_suffix_expr(Parser* parser) {
    AstNode* out = parse_primary_type_expr(parser);
    while(true) {
        AstNode* suffix_op = parse_suffix_op(parser);
        if(suffix_op == null)
            break;
        out = suffix_op;
    }
    Token* lparen = parser_chomp_if(LPAREN);
    if(lparen == null) {
        WARN(expected param list);
        return out;
    }

    Vec* params = vec_new(AstNode, 1);
    AstNode* param = null;
    while(parser_chomp(1)->kind != RPAREN) {
        param = parse_expr(parser);
        if(param == null)
            break;
        vec_push(params, param);
        switch(pc->kind) {
            case COMMA: parser_chomp(1);
            case RPAREN: parser_chomp(1); break;
            case COLON: case RBRACE: case RSQUAREBRACK:
                ast_expected("RBRACE");
            default:
                WARN(Expected comma);
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
static AstNode* parse_primary_type_expr(Parser* parser) {
    AstNode* out = null;
    AstNode* expr = null;
    Token* tok = null;
    switch(pc->kind) {
        case CHAR_LIT:
            out = ast_create_node(AstNodeKindCharLiteral);
            out->data.comptime_value->char_value->value = pc->value;
            parser_chomp(1);
            return out;
        case INTEGER:
            out = ast_create_node(AstNodeKindIntLiteral);
            out->data.comptime_value->int_value->value = pc->value;
            parser_chomp(1);
            return out;
        case FLOAT_LIT:
            out = ast_create_node(AstNodeKindFloatLiteral);
            out->data.comptime_value->float_value->value = pc->value;
            parser_chomp(1);
            return out;
        case UNREACHABLE:
            out = ast_create_node(AstNodeKindUnreachable);
            parser_chomp(1);
            return out;
        case STRING:
            out = ast_create_node(AstNodeKindStringLiteral);
            parser_chomp(1);
            return out;
        case BUILTIN: return parse_builtin_call(parser);
        case FUNC: return parse_func_decl(parser);
        case IF: return ast_parse_if_expr(parser);
        case MATCH: return parse_match_expr(parser);
        case STRUCT: return parse_struct_decl(parser);
        case ENUM: return parse_enum_decl(parser);
        case ATTR_COMPTIME:
            out = ast_create_node(AstNodeKindAttributeExpr);
            expr = parse_type_expr(parser);
            if(expr == null)
                ast_expected("type expr");
            out->data.expr->attr_expr->expr = expr;
            return out;
        case IDENTIFIER:
            switch((pc + 1)->kind) {
                case COLON:
                    switch((pc + 2)->kind) {
                        case ATTR_INLINE:
                            parser_chomp(3);
                            switch(pc->kind) {
                                case LOOP: return parse_loop_expr(parser);
                                default: ast_expected("inlinable expression");
                            }
                        case LOOP:
                            parser_chomp(2);
                            return parse_loop_expr(parser);
                        case LBRACE:
                            parser_chomp(2);
                            return parse_block(parser);
                        default:
                            out = ast_create_node(AstNodeKindIdentifier);
                            parser_chomp(1);
                            return out;
                    }
                default:
                    out = ast_create_node(AstNodeKindIdentifier);
                    parser_chomp(1);
                    return out;
            }
            break;
        case ATTR_INLINE:
            parser_chomp(1);
            switch(pc->kind) {
                case LOOP: return parse_loop_expr(parser);
                default: ast_expected("inlinable expression");
            }
            break;
        case LOOP: return parse_loop_expr(parser);
        case DOT:
            switch((pc + 1)->kind) {
                case IDENTIFIER:
                    out = ast_create_node(AstNodeKindIdentifier);
                    parser_chomp(1);
                    return out;
                default: return null;
            }
            break;
        case LPAREN:
            out = ast_create_node(AstNodeKindGroupedExpr);
            expr = parse_expr(parser);
            if(expr == null)
                ast_expected("expression");
            tok = parser_chomp_if(RPAREN);
            if(tok == null)
                ast_expected("RPAREN");

            out->data.expr->grouped_expr->expr = expr;
            return out;
    }
}

static AstNode* parse_match_expr(Parser* parser) {
    Token* switch_kwd = parser_chomp_if(MATCH);
    if(switch_kwd == null)
        ast_expected("`switch` keyword");
    
    Token* lparen = parser_chomp_if(LPAREN); // this is optional
    AstNode* expr = parse_expr(parser);
    if(expr == null)
        ast_expected("expression");
    Token* rparen = parser_chomp_if(RPAREN); // this is optional
    Token* lbrace = parser_expect_token(RBRACE); // required

    Vec* branches = ast_parse_match_branches(parser);
    if(branches == null)
        ast_expected("branches for `match`");
    
    // Parse any trailing comma
    Token* comma = parser_chomp_if(COMMA);

    AstNode* out = ast_create_node(AstNodeKindMatchExpr);
    out->data.expr->match_expr->expr = expr;
    out->data.expr->match_expr->branches = branches;
    return out;
}

// MatchBranch
//      KEYWORD(case) (COLON? / EQUALS_ARROW?) AssignmentExpr
static AstNode* parse_match_branch(Parser* parser) {
    AstNode* out = parse_match_case_kwd(parser);
    CORETEN_ENFORCE(out->kind == AstNodeKindMatchBranch);
    if(out == null)
        return null;
    
    Token* colon = parser_chomp_if(COLON); // `:`
    Token* equals_arrow = parser_chomp_if(EQUALS_ARROW); // `=>`
    if(colon == null && equals_arrow == null)
        ast_error("Missing token after `case`. Either `:` or `=>`");

    AstNode* expr = parse_assignment_expr(parser);
    out->data.expr->match_branch_expr->expr = expr;

    return out;
}

// MatchCase
//      | MatchItem (COMMA MatchItem)* COMMA?
//      | KEYWORD(else)
static AstNode* parse_match_case_kwd(Parser* parser) {
    AstNode* match_item = parse_match_item(parser);
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
// BreakLabel
//      COLON IDENTIFIER
static Token* parse_break_label(Parser* parser) {
    Token* colon = parser_chomp_if(COLON);
    if(colon == null)
        return null;
        
    Token* ident = parser_expect_token(IDENTIFIER);
    return ident;
}

// BlockLabel
//      IDENTIFIER COLON
static Token* parse_block_label(Parser* parser) {
    Token* ident = parser_chomp_if(IDENTIFIER);
    if(ident == null)
        return null;
    
    Token* colon = parser_chomp_if(COLON);
    if(colon == null)
        return null;

    return ident;
}

// FieldInit
//      DOT IDENTIFIER EQUALS Expr
static AstNode* parse_field_init(Parser* parser) {
    if((pc + 0)->kind == DOT and
       (pc + 1)->kind == IDENTIFIER and
       (pc + 2)->kind == EQUALS) {
            parser_chomp(3);
            AstNode* expr = parse_expr(parser);
            if(expr == null)
                ast_expected("expression");
            return expr;
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
            upper = parse_expr(parser);
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

// StringLiteral
static AstNode* ast_parse_string_literal(Parser* parser) {
    if(pc->kind == STRING) {
        parser_chomp(1);
        AstNode* out = ast_create_node(AstNodeKindStringLiteral);
        return out;
    }

    return null;
}