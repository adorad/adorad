#include <adorad/compiler/ast.h>
#include <adorad/compiler/parser.h>

#define pt      parser->toklist
#define pc      parser->curr_tok

#define nodepush(node)              vec_push(parser->nodelist, node)

#define parser_chomp()              chomp(parser)
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
static AstNode* ast_parse_toplevel_decl(Parser* parser) {
    AstNode* module = ast_parse_module_statement(parser);
    if(module != null)
        return module;
    
    AstNode* import = ast_parse_import_statement(parser);
    if(import != null)
        return null;
    
    AstNode* alias = ast_parse_alias_decl(parser);
    if(alias != null)
        return alias;
    
    AstNode* comptime = ast_parse_attribute_expr(parser, ATTR_COMPTIME);
    if(comptime != null)
        return comptime;

    AstNode* variable = ast_parse_variable_decl(parser);
    if(variable != null)
        return variable;

    AstNode* func_decl = ast_parse_func_decl(parser);
    if(func_decl != null)
        return func_decl;
    
    AstNode* container_decl = ast_parse_container_decl(parser);
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
static AstNode* ast_parse_import_statement(Parser* parser) {
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
    Token* attr = parser_chomp();
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
    if(larrow == null && return_type_expr != null)
        ast_expected("trailing `->` after function prototype");
    
    bool no_body = false;
    AstNode* body = null;
    AstNode* out = ast_create_node(AstNodeKindFuncDecl);
    switch(pc->kind) {
        case SEMICOLON:
            parser_chomp();
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
static AstNode* ast_parse_container_members(pars) {
    while(true) {
        switch(pc->kind) {
            case ATTR_COMPTIME:
                Token* comptime_attr = parser_chomp();
                switch(pc->kind) {
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
static AstNode* ast_parse_if_expr(Parser* parser) {
    Token* if_token = parser_chomp_if(IF);
    if(if_token == null)
        unreachable();

    Token* lparen = parser_chomp_if(LPAREN); // this is optional
    AstNode* condition = ast_parse_expr(parser);
    if(condition == null)
        ast_expected("condition");
    Token* rparen = parser_chomp_if(RPAREN); // this is optional

    if(lparen != null && rparen == null)
        ast_expected("closing `(`");
    if(lparen == null && rparen != null)
        ast_error("Extra `)` token not expected at this point");
    
    AstNode* if_body = null;
    AstNode* assignment_expr = null;
    AstNode* block_expr = ast_parse_block_expr(parser);
    if(block_expr == null) {
        assignment_expr = ast_parse_assignment_expr(parser);
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
    
    if(else_body == null && semicolon == null)  
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
static AstNode* ast_parse_labeled_statement(Parser* parser) {
    Token* label = ast_parse_block_label(parser);
    if(label == null)
        ast_expected("Label");

    AstNode* block = ast_parse_block_expr(parser);
    if(block != null) {
        block->data.stmt->block_stmt->label = label != null ? label->value : null;
        return block;
    }

    AstNode* loop_statement = ast_parse_loop_expr(parser);
    if(loop_statement != null) {
        loop_statement->data.expr->loop_expr->label = label != null ? label->value : null;
        return loop_statement;
    }

    if(label != null)
        ast_expected("either a block or loop statement");

    return null;
}

// LoopStatement
//      | ATTRIBUTE(inline)? (LoopWhileExpr / LoopCExpr / LoopInExpr)
static AstNode* ast_parse_loop_expr(Parser* parser) {
    Token* inline_attr = parser_chomp_if(ATTR_INLINE);
    AstNode* out = null;

    AstNode* loop_while_expr = ast_parse_loop_while_expr(parser);
    if(loop_while_expr != null)
        out = loop_while_expr;

    AstNode* loop_c_expr = ast_parse_loop_c_expr(parser);
    if(loop_c_expr != null)
        out = loop_c_expr;

    AstNode* loop_in_expr = ast_parse_loop_in_expr(parser);
    if(loop_in_expr != null)
        out = loop_in_expr;

    if(out == null)
        ast_expected("loop statement");
    
    out->data.expr->loop_expr->is_inline = cast(bool)(inline_attr != null);
    return out;
}

/*
    TODO: Add implementation for:
        1. ast_parse_loop_while_expr(parser)
        2. ast_parse_loop_c_expr(parser)
        3. ast_parse_loop_in_expr(parser)
*/
static AstNode* ast_parse_loop_while_expr(Parser* parser) {
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
        ast_expected("an expression after assignment op");

    AstNode* out = ast_create_node(AstNodeKindBinaryOpExpr);
    out->data.expr->binary_op_expr->op = op;
    out->data.expr->binary_op_expr->lhs = lhs;
    out->data.expr->binary_op_expr->rhs = rhs;
    return out;
}