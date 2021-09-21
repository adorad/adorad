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

#ifndef ADORAD_AST_H
#define ADORAD_AST_H

#include <adorad/core/types.h>
#include <adorad/core/buffer.h>
#include <adorad/core/vector.h>
#include <adorad/compiler/location.h>
#include <adorad/compiler/tokens.h>

typedef struct AstNode AstNode;
typedef enum AstNodeKind AstNodeKind;
typedef struct AstNodeExpression AstNodeExpression;
typedef struct AstNodeStatement AstNodeStatement;
typedef struct AstNodeCompileTimeValue AstNodeCompileTimeValue;
typedef struct AstNodeDecl AstNodeDecl;
typedef struct AstNodeTypeDecl AstNodeTypeDecl;
typedef struct AstNodeIdentifier AstNodeIdentifier;

enum AstNodeKind {
    AstNodeKindIdentifier = 0,
    AstNodeKindBlock,        // `{ ... }

    // Functions
    AstNodeKindFuncPrototype,
    AstNodeKindFuncDecl,      // `func hello()`

    // Literals
    AstNodeKindIntLiteral,    // `2`
    AstNodeKindFloatLiteral,  // `3.4224829`
    AstNodeKindCharLiteral,   // 'a', 'b'...
    AstNodeKindStringLiteral, // 'str` or "str"
    AstNodeKindBoolLiteral,   // `bool`
    AstNodeKindNilLiteral,    // `nil`

    // Declarations
    AstNodeKindEnumDecl,      // `enum foo { ... }`
    AstNodeKindUnionDecl,     // `union bar { ... }`
    AstNodeKindVariableDecl,  // `some_type var_name = ...`

    // Expressions
    AstNodeKindAliasDeclExpr,  // `alias foo as bar`
    AstNodeKindFuncCallExpr,   // `sayHello('Hello!')`
    AstNodeKindIfExpr,         // `if cond { ...}`
    AstNodeKindLoopInfExpr,  // `loop {}`
    AstNodeKindLoopCExpr,      // `loop i=0; i<10; i++ {}`
    AstNodeKindLoopInExpr,     // `loop i in 0..10 {}`
    AstNodeKindMatchExpr,      // `match expr { ... }`
    AstNodeKindCatchExpr,      // `catch Error`
    AstNodeKindBinaryOpExpr,   // a binary expression like `&&` or `||
    AstNodeKindPrefixOpExpr,
    AstNodeKindFieldAccessExpr,
    AstNodeKindAttributeExpr,
    
    AstNodeKindInitExpr,
    AstNodeKindSliceExpr,
    AstNodeKindArrayAccessExpr,
    AstNodeKindArrayType,
    AstNodeKindInferredArrayType,

    AstNodeKindModuleStatement,     // `module foo`
    AstNodeKindImportStatement,     // `import foo as bar`'

    // Fields
    AstNodeKindTypeDecl,       // `type name T { ... }`  where T is one of {enum/struct}

    AstNodeKindBreak,          // `break`
    AstNodeKindContinue,       // `continue`

    // Misc
    AstNodeKindParamDecl,
    AstNodeKindDefer,
    AstNodeKindReturn,
    AstNodeKindUnreachable,
    AstNodeKindMatchBranch,
    AstNodeKindMatchRange,
    AstNodeKindOptional,
};

typedef enum {
    VisibilityModePrivate, // default
    VisibilityModePublic   // if `export` is used
} VisibilityMode;

typedef enum {
    AstAddressingModeInvalid,   // invalid addressing mode
    AstAddressingModeCompile_time_value,  // value known at compile time
    AstAddressingModeVariable, // addressable variable (known at runtime)
    AstAddressingModeConstant, // constant
    AstAddressingModeType,     // type  
} AstAddressingMode;

typedef enum {
    AstLanguageAdorad,
    AstLanguageC,
    AstLanguageAmd64,
    AstLanguageI386,
    AstLanguageArm64,
    AstLanguageArm32,
    AstLanguageRv64,
    AstLanguageRv32,
} AstLanguage;

typedef struct {
    Buff* scope;
} AstNodeScope;

typedef enum {
    IdentifierKindUnresolved,
    IdentifierKindBlankident, // `_`
    IdentifierKindVariable,
    IdentifierKindConst,
    IdentifierKindFunction,
    IdentifierKindGlobal,     // if declared within a `global` scope
} IdentifierKind;

// The `[]` before a function/variable
// Eg: [inline], [compiletime]
typedef struct {
    Buff* name;   // [inline]
} AstNodeAttribute;

typedef struct {
    AstNode* type;
    bool is_const;
    VisibilityMode visibility;
    bool is_mutable;
} AstNodeIdentifier;

typedef struct {
    Vec* positional_args;   // Vec<AstNodeArg*>
    Vec* args;              // Vec<AstNodeArg*>
    Vec* variadic_args;     // Vec<AstNodeArg*>
    Vec* kwd_args;          // Vec<AstNodeArg*>
} AstNodeArguments;

typedef struct {
    AstNode* arg;
    AstNode* annotation;
    Buff* type_comment;
} AstNodeArgData;

typedef struct {
    AstNode* ident;
    AstNode* value;
} AstNodeKwdData;

typedef struct {
    AstNode* expr;
} AstNodeAsCast;

typedef struct { 
    AstNode* expr;
    // AttributeKind kind;
} AstNodeAttributeExpr;

typedef struct {
    AstNode* arg;
    AstNode* expr;
    Buff* type;
    bool has_arg;
} AstNodeCastExpr;

typedef struct {
    AstNode* condition;
    AstNode* if_body;
    AstNode* else_node;
    bool is_comptime;
    bool is_expr;
    bool has_else;
} AstNodeIfExpr;

typedef struct {
    AstNode* type;
    Vec* entries;
    enum {
        InitExprKindStruct,
        InitExprKindArray,
    } kind;
} AstNodeInitExpr;

typedef struct {
    AstNode* cond;
    Vec* statements;
    AstNodeScope* scope;
} AstNodeLoopInfExpr;

typedef struct {
    Buff* label;
    AstNode* init;
    bool has_init;
    AstNode* cond;
    bool has_cond;
    AstNode* updation;  // increment/decrement
    bool has_updation;
    Vec* statements;
    AstNodeScope* scope;
} AstNodeLoopCExpr;

typedef struct {
    Buff* label;
    Buff* key_var;
    Buff* val_var;
    bool is_val_var_mutable;
    AstNode* cond;
    bool is_range;
    Vec* statements;
    TokenKind tokenkind;
    AstNodeScope* scope;
} AstNodeLoopInExpr;

typedef struct {
    Buff* label;
    bool is_inline;
    union {
        AstNodeLoopInfExpr* loop_inf_expr;
        AstNodeLoopCExpr* loop_c_expr;
        AstNodeLoopInExpr* loop_in_expr;
    };
} AstNodeLoopExpr;

typedef enum {
    FuncCallModifierNone,
    FuncCallModifierAsync,
    FuncCallModifierNeverInline,
    FuncCallModifierAlwaysInline,
    FuncCallModifierCompileTime
} FuncCallModifier;

typedef struct {
    AstNode* func_call_expr;
    Vec* params;
    FuncCallModifier modifier;
} AstNodeFuncCallExpr;

typedef struct {
    AstNode* expr;
    Vec* branches;
} AstNodeMatchExpr;

typedef struct {
    AstNode* expr;
    Vec* branches;
    bool any_branches_are_ranges; // if any branch is a range-based match
} AstNodeMatchBranchExpr;

typedef struct {
    AstNode* begin;
    AstNode* end;
} AstNodeMatchRangeExpr;

typedef struct {
    AstNode* op1;
    AstNode* symbol; // can be nullptr
    AstNode* op2;
} AstNodeCatchExpr;

typedef struct {
    Buff* symbol;
    AstNode* target_node;
    AstNode* then_node;
    AstNode* else_node;
    Buff* err_symbol;
} AstNodeTryExpr;

typedef enum {
    BinaryOpKindInvalid,
    BinaryOpKindAssignmentMult,    // *=
    BinaryOpKindAssignmentDiv,     // /=
    BinaryOpKindAssignmentMod,     // %=
    BinaryOpKindAssignmentPlus,    // +=
    BinaryOpKindAssignmentMinus,   // -=
    BinaryOpKindAssignmentBitshiftLeft,  // <<=
    BinaryOpKindAssignmentBitshiftRight, // >>=
    BinaryOpKindAssignmentBitAnd, // &=
    BinaryOpKindAssignmentBitXor, // ^=
    BinaryOpKindAssignmentBitOr,  // |=
    BinaryOpKindAssignmentEquals, // =

    BinaryOpKindCmpEqual,                 // ==
    BinaryOpKindCmpNotEqual,              // !=
    BinaryOpKindCmpLessThan,              // <
    BinaryOpKindCmpGreaterThan,           // >
    BinaryOpKindCmpLessThanorEqualTo,     // <=
    BinaryOpKindCmpGreaterThanorEqualTo,  // >=

    BinaryOpKindBoolAnd,                  // `and`
    BinaryOpKindBoolOr,                   // `or`
    BinaryOpKindBoolNot,                  // `not`

    BinaryOpKindBitAnd,  // &
    BinaryOpKindBitOr,   // |
    BinaryOpKindBitXor,  // ^
    BinaryOpKindBitOrOr, // ||
    BinaryOpKindBitNegation, // !

    BinaryOpKindBitshitLeft,   // <<
    BinaryOpKindBitshitRight,  // >>

    BinaryOpKindAdd,      // +
    BinaryOpKindSubtract, // -
    BinaryOpKindMult,     // *
    BinaryOpKindDiv,      // /
    BinaryOpKindMod,      // %
} BinaryOpKind;

typedef struct {
    AstNode* lhs;
    BinaryOpKind op;
    AstNode* rhs;
} AstNodeBinaryOpExpr;

typedef struct {
    AstNode* expr;
} AstNodeTypeOfExpr;

typedef struct {
    Vec* exprs;
} AstNodeSetExpr;

typedef struct {
    AstNodeArguments args;
    AstNode* expr;
} AstNodeLambdaExpr;

typedef struct {
    AstNode* value;    
} AstNodeAwaitExpr;

typedef struct {
    AstNode* array_ref_expr;
    AstNode* lower;
    AstNode* upper;
    AstNode* step;
    AstNode* sentinel;  // can be null
} AstNodeSliceExpr;

// This can be one of:
//     | AstNodeAsCast
//     | AstNodeCastExpr
//     | AstNodeIfExpr   
//     | AstNodeLoopExpr
//     | AstNodeFuncCallExpr
//     | AstNodeMatchExpr
//     | AstNodeMatchBranchExpr
//     | AstNodeMatchRangeExpr
//     | AstNodeCatchExpr
//     | AstNodeBinaryOpExpr
//     | AstNodeTypeExpr
//     | AstNodeTypeOfExpr
typedef struct {
    union {
        AstNodeAsCast* as_cast;
        AstNodeAttributeExpr* attr_expr;
        AstNodeCastExpr* cast_expr;
        AstNodeIfExpr* if_expr;
        AstNodeInitExpr* init_expr;
        AstNodeLoopExpr* loop_expr;
        AstNodeFuncCallExpr* func_call_expr;
        AstNodeMatchExpr* match_expr;
        AstNodeMatchBranchExpr* match_branch_expr;
        AstNodeMatchRangeExpr* match_range_expr;
        AstNodeCatchExpr* catch_expr;
        AstNodeTryExpr* try_expr;
        AstNodeBinaryOpExpr* binary_op_expr;
        // AstNodeTypeExpr* type_expr;
        AstNodeTypeOfExpr* typeof_expr;
        AstNodeSetExpr* set_expr;
        AstNodeLambdaExpr* lambda_expr;
        AstNodeAwaitExpr* await_expr;
        AstNodeSliceExpr* slice_expr;
    };
} AstNodeExpression;

typedef struct {
    Buff* original;
    Buff* alias;
} AstNodeAliasDecl;

typedef struct {
    Buff* name;
    VisibilityMode visibility;
    Vec* fields;
    Vec* attributes;
} AstNodeTypeEnumDecl;

typedef struct {
    Buff* name;
    Vec* fields;    // variables, etc
    Vec* methods;   // methods
} AstNodeTypeStructDecl;

// This can be one of:
//     | AstNodeTypeEnumDecl
//     | AstNodeTypeUnionDecl (disabled for now)
//     | AstNodeTypeStructDecl
typedef struct {
    union {
        AstNodeTypeEnumDecl* enum_decl;
        // AstNodeTypeUnionDecl* union_decl;
        AstNodeTypeStructDecl* struct_decl;
    };
} AstNodeTypeDecl;

typedef struct {
    VisibilityMode visibility;
    bool is_block;  // `const ( ... )`
    Vec* fields;    // various constant declarations
} AstNodeConstantDecl;

typedef struct {
    Buff* module;    // globals declared in a module, persist through that module
    bool is_block;   // `global ( ... )`
    Vec* fields;     // various global declarations
} AstNodeGlobalDecl;

typedef struct {
    Buff* name;
    VisibilityMode visibility;
    Vec* variant_types; // Vec<AstNodeType*>
} AstNodeSumTypeDecl;

// Function or Method Declaration
typedef struct {
    Buff* name;
    Buff* module;      // name of the module
    Vec* params;
    AstNode* body;      // can be nullptr for no-body functions (just declarations)
    AstNode* return_type;
    Buff* parent_type; // the `type` of which the function belongs to (null, if not a method)
    bool is_variadic;  // variadic arguments
    bool is_generic;

    bool is_main;      // true for `func main()`
    bool is_test;      // true for `func test_yyy()`
    bool no_body;      // true for function definitions (no function body) `func abc()`

    // Attributes
    bool is_noreturn;  // true for `[noreturn] func
    bool is_comptime;
    bool is_inline;
    bool is_noinline;
 
    VisibilityMode visibility;
} AstNodeFuncDecl;

// This can be one of:
//     | AstNodeAliasDecl
//     | AstNodeTypeDecl (enum/struct)
//     | AstNodeFuncDecl
//     | AstNodeDictDecl
//     | AstNodeListDecl
//     | AstNodeTupleDecl
//     | AstNodeConstantDecl
//     | AstNodeGlobalDecl
//     | AstNodeSumTypeDecl 
typedef struct {
    Buff* name;
    VisibilityMode visibility;
    union {
        AstNodeAliasDecl* alias_decl;
        AstNodeTypeDecl* type_decl;
        AstNodeFuncDecl* func_decl;
        AstNodeConstantDecl* const_decl;
        AstNodeGlobalDecl* global_decl;
        AstNodeSumTypeDecl* sumtype_decl;
    };
} AstNodeDecl;

typedef struct {
    AstNodeKind op;
    Vec* right;        // Vec<AstNodeExpression*>
    Vec* left;         // Vec<AstNodeExpression*>
    Vec* right_types;  // Vec<AstNodeExpression*>
    Vec* left_types;   // Vec<AstNodeExpression*>
    bool is_compound;
} AstNodeAssignmentStatement;

// `{ ... }`
typedef struct {
    Buff* label; // for labeled block statements
    Vec* statements;
} AstNodeBlock;

// break/continue
typedef struct {
    Buff* name;
    AstNode* expr;  // can be nullptr (`break`). always nullptr for `continue`
    enum {
        AstNodeBranchStatementBreak,
        AstNodeBranchStatementContinue
    } type;
} AstNodeBranchStatement;

typedef struct {
    AstNode* expr;
} AstNodeDeferStatement;

typedef struct {
    Buff* fixme;
} AstNodeEmptyStatement;

typedef enum FuncInline {
    FuncInlineAuto,
    FuncInlineInline,
    FuncInlineNoinline
} FuncInline;

typedef struct {
    Buff* name;
    Buff* alias; // can be null
} AstNodeImportStatement;

typedef struct {
    Buff* name;
    Buff* short_name;
    bool is_skip;
    VisibilityMode visibility;
} AstNodeModuleStatement;

typedef struct {
    AstNode* expr;
    enum {
        ReturnKindOk,
        ReturnKindError,
    } kind;
} AstNodeReturnStatement;

typedef struct {
    Buff* name;
    AstNode* type;    // can be null
    AstNode* expr;

    bool is_const;
    bool is_mutable;
    VisibilityMode visibility;
} AstNodeVarDecl;

// This can be one of:
//     | AstNodeAssignmentStatement
//     | AstNodeBlock
//     | AstNodeBranchStatement
//     | AstNodeDeferStatement
//     | AstNodeEmptyStatement
//     | AstNodeExpression
//     | AstNodeFuncDecl
//     | AstNodeImportStatement
//     | AstNodeModuleStatement
//     | AstNodeReturnStatement
//     | AstNodeVarDecl
typedef struct {
    union {
        AstNodeAssignmentStatement* assign_stmt;
        AstNodeBlock* block_stmt;
        AstNodeBranchStatement* branch_stmt;
        AstNodeDeferStatement* defer_stmt;
        AstNodeEmptyStatement* empty_stmt;
        AstNodeExpression* expr_stmt;
        AstNodeImportStatement* import_stmt;
        AstNodeModuleStatement* module_stmt;
        AstNodeReturnStatement* return_stmt;
        AstNodeVarDecl* var_decl;
    };
} AstNodeStatement;

typedef struct {
    bool value;
} AstNodeBoolLiteral;

typedef struct {
    Buff* value;
} AstNodeByteLiteral;

typedef struct {
    Buff* value;
    // TODO (jasmcaus) - Come up with a workaround for this
    enum {
        AstNodeFloatLiteral32,    // default
        AstNodeFloatLiteral64,
        // AstNodeFloatLiteral128 // will be supported later
    } type;
} AstNodeFloatLiteral;

typedef struct {
    Buff* value;
    // TODO (jasmcaus) - Come up with a workaround for this
    enum {
        AstNodeIntegerLiteral8,
        AstNodeIntegerLiteral16,
        AstNodeIntegerLiteral32, // default
        AstNodeIntegerLiteral64,
        // AstNodeIntegerLiteral128 // will be supported later
        AstNodeUIntLiteral8,
        AstNodeUIntLiteral16,
        AstNodeUIntLiteral32, // default
        AstNodeUIntLiteral64,
        // AstNodeUIntLiteral128 // will be supported later
    } type;
} AstNodeIntegerLiteral;

typedef struct {
    Buff* value;
} AstNodeCharLiteral;

typedef struct {
    Buff* value;
    bool is_special;   // format / raw string
    enum {
        AstNodeStringLiteralNone,   // if `is_special` is false
        AstNodeStringLiteralRaw,    // `r"abc"`
        AstNodeStringLiteralFormat  // `f"name = {name}"`
    } type;
} AstNodeStringLiteral;

// This can be one of:
//     | AstNodeBoolLiteral
//     | AstNodeByteLiteral
//     | AstNodeFloatLiteral (32/64)
//     | AstNodeIntegerLiteral (8/16/32/64 + unsigned versions) 
//     | AstNodeCharLiteral
//     | AstNodeStringLiteral
typedef struct {
    union {
        AstNodeBoolLiteral* bool_value;
        AstNodeByteLiteral* byte_value;
        AstNodeFloatLiteral* float_value;
        AstNodeIntegerLiteral* int_value;
        AstNodeCharLiteral* char_value;
        AstNodeStringLiteral* str_value;
    };
} AstNodeCompileTimeValue;

typedef struct {
    Buff* module;
    Buff* name;
    AstNode* expr;
    AstNodeCompileTimeValue* comptime_value;
    VisibilityMode visibility;
} AstNodeConstField;

typedef struct {
    Buff* name;
    AstNode* expr;
    bool has_expr;
} AstNodeGlobalField;

typedef struct {
    Buff* name;
    AstNode* type_expr;
    AstNode* init_expr;
    bool is_local;     // false, for global vars
    bool is_comptime;
    bool is_mutable;
    bool is_argument;
    bool is_used;
    bool is_tmp;
    bool is_heap_obj;
    VisibilityMode visibility;
} AstNodeVariable;

// This can be one of:
//     | AstNodeConstField
//     | AstNodeGlobalField
//     | AstNodeVariable
typedef struct {
    Buff* name;
    union {
        AstNodeConstField* const_field;
        AstNodeGlobalField* global_field;
        AstNodeVariable* var;
    };
    // AstNodeType* type;
} AstNodeScopeObject;

typedef struct {
    Buff* name;
    AstNode* type;
    bool is_alias;
    bool is_var_args;
} AstNodeParamDecl;

typedef struct {
    AstNode* expr;
} AstNodeReturnExpr;

typedef struct {
    Buff* name;   // can be nullptr if no name
    AstNode* body;
} AstNodeTestDecl;

typedef struct {
    Buff* symbol;
    AstNode* target_node;
    AstNode* then_node;
    AstNode* else_node;  // null, block node, or an `if expr` node
} AstNodeTestExpr;

typedef enum {
    PrefixOpKindInvalid,
    PrefixOpKindBoolNot,   // KEYWORD(not)
    PrefixOpKindNegation,  // !var
    PrefixOpKindAddrOf,    // &var
    PrefixOpKindTry,       // KEYWORD(try)
    PrefixOpKindOptional,  // ?
} PrefixOpKind;

typedef struct {
    AstNode* expr;
    PrefixOpKind op;
} AstNodePrefixOpExpr;

typedef struct {
    AstNode* expr;
} AstNodeCompileTime;

typedef struct {
    AstNode* array_ref_expr;
    AstNode* subscript;
} AstNodeArrayAccessExpr;

typedef struct {
    AstNode* sentinel; // can be null
    AstNode* child_type;
} AstNodeInferredArrayType;

typedef struct {
    AstNode* size;
    AstNode* sentinel;
    AstNode* child_type;
    AstNode* align_expr;
    Token* allow_zero_token;
    bool is_const;
    bool is_volatile;
} AstNodeArrayType;

typedef struct {
    AstNode* struct_expr;
    Buff* field_name;
} AstNodeFieldAccessExpr;

struct AstNode {
    AstNodeKind kind; // type of AST Node
    Location* loc;

    union {
        AstNodeIdentifier* identifier;
        AstNodeTypeDecl* type_decl;
        AstNodeDecl* decl;
        AstNodeExpression* expr;
        AstNodeStatement* stmt;
        AstNodeCompileTimeValue* comptime_value;
        AstNodeScopeObject* scope_obj;
        AstNodeTestDecl* test_decl;
        AstNodeTestExpr* test_expr;
        AstNodePrefixOpExpr* prefix_op_expr;
        AstNodeParamDecl* param_decl;
        AstNodeArrayAccessExpr* array_access_expr;
        AstNodeInferredArrayType* inferred_array_type;
        AstNodeArrayType* array_type;
        AstNodeFieldAccessExpr* field_access_expr;
    } data;
};

#endif // ADORAD_AST_H