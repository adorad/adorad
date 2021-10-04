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
    AstNodeKindGroupedExpr,
    
    AstNodeKindInitExpr,
    AstNodeKindSliceExpr,
    AstNodeKindArrayAccessExpr,
    AstNodeKindArrayType,
    AstNodeKindInferredArrayType,
    AstNodeKindStructExpr,
    AstNodeKindEnumExpr,
    AstNodeKindArrayInitExpr,

    AstNodeKindModuleStatement,     // `module foo`
    AstNodeKindImportStatement,     // `import foo as bar`'

    // Fields
    AstNodeKindTypeDecl,       // `type name T { ... }`  where T is one of {enum/struct}

    AstNodeKindBreak,          // `break`
    AstNodeKindContinue,       // `continue`

    // Misc
    AstNodeKindParamDecl,
    AstNodeKindParamList,
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

// The `[]` before a function/variable
// Eg: [inline], [comptime]
typedef struct {
    Buff* name;   // [inline]
} AstNodeAttribute;

typedef struct {
    AstNode* type;
    bool is_const;
    bool is_mutable;
    VisibilityMode visibility;
} AstNodeIdentifier;

typedef struct {
    AstNode* expr;
} AstNodeAsCast;

typedef struct { 
    AstNode* expr;
    // AttributeKind kind;
} AstNodeAttributeExpr;

// @cast(u8, 292)
typedef struct {
    Buff* cast_to_type; // `u8`
    Buff* original_type;   // `292`
} AstNodeCastExpr;

// `if x < y { ... }`
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

// `loop { ... }`
typedef struct {
    AstNode* cond;
    Vec* statements;
} AstNodeLoopInfExpr;

// `loop i = 3; i < 10; i+=1 { ... }`
typedef struct {
    Buff* label;
    AstNode* init;
    bool has_init;
    AstNode* cond;
    bool has_cond;
    AstNode* updation;  // increment/decrement
    bool has_updation;
    Vec* statements;
} AstNodeLoopCExpr;

// `loop arg in args { ... }'
typedef struct {
    Buff* label;
    Buff* key_var;
    Buff* val_var;
    bool is_val_var_mutable;
    AstNode* cond;
    bool is_range;
    Vec* statements;
    TokenKind tokenkind;
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

typedef struct {
    AstNode* func_call_expr;
    Vec* params;
} AstNodeFuncCallExpr;

// `(expr)`
typedef struct {
    AstNode* expr;
} AstNodeGroupedExpr;

// `match x { when 0..3 => ...  }`
typedef struct {
    AstNode* expr;  // `x`
    Vec* branches;
    bool any_branches_are_ranges; // if any branch is a range-based match
} AstNodeMatchExpr;

// Individual branch of a `match` expression
// Eg: `when <cond> => ...`
typedef struct {
    // If range-based
    bool is_range;
    AstNode* cond_node;   // <cond>
    AstNode* block_node;  // the `...` in the above example
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

// `try expr`
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
    AstNode* expr;
} AstNodeLambdaExpr;

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
        AstNodeGroupedExpr* grouped_expr;
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
} AstNodeEnumDecl;

typedef struct {
    Buff* name;
    Vec* fields;    // variables, etc
    Vec* methods;   // methods
} AstNodeStructDecl;

// This can be one of:
//     | AstNodeEnumDecl
//     | AstNodeUnionDecl (disabled for now)
//     | AstNodeStructDecl
typedef struct {
    union {
        AstNodeEnumDecl* enum_decl;
        // AstNodeUnionDecl* union_decl;
        AstNodeStructDecl* struct_decl;
    };
} AstNodeTypeDecl;

typedef struct {
    Buff* module;    // globals declared in a module persist through that module
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
    Buff* module;         // name of the module
    Vec* params;
    AstNode* body;        // can be nullptr for no-body functions (just declarations)
    AstNode* return_type;
    Buff* parent_type;    // the `type` of which the function belongs to (null, if not a method)
    bool is_variadic;     // variadic arguments
    bool is_generic;

    bool is_main;         // true for `func main()`
    bool is_test;         // true for `func test_yyy()`
    bool no_body;         // true for function definitions (no function body) `func abc()`

    // Attributes
    bool is_noreturn;     // true for `[noreturn] func
    bool is_comptime;     // true for `[comptime] func
    bool is_inline;       // true for `[inline] func
    bool is_noinline;     // true for `[noinline] func
 
    VisibilityMode visibility;
} AstNodeFuncDecl;

// This can be one of:
//     | AstNodeAliasDecl
//     | AstNodeTypeDecl (enum/struct)
//     | AstNodeFuncDecl
//     | AstNodeDictDecl
//     | AstNodeListDecl
//     | AstNodeTupleDecl
//     | AstNodeGlobalDecl
//     | AstNodeSumTypeDecl 
typedef struct {
    Buff* name;
    VisibilityMode visibility;
    union {
        AstNodeAliasDecl* alias_decl;
        AstNodeTypeDecl* type_decl;
        AstNodeFuncDecl* func_decl;
        AstNodeGlobalDecl* global_decl;
        AstNodeSumTypeDecl* sumtype_decl;
    };
} AstNodeDecl;

typedef struct {
    AstNodeKind op;
    Vec* right;
    Vec* left;
    Vec* right_types;
    Vec* left_types;
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
        AstNodeIntegerLiteral8,  // i8
        AstNodeIntegerLiteral16, // i16
        AstNodeIntegerLiteral32, // i32
        AstNodeIntegerLiteral64, // i64
        // AstNodeIntegerLiteral128 // will be supported later
        AstNodeUIntLiteral8,     // u8
        AstNodeUIntLiteral16,    // u16
        AstNodeUIntLiteral32,    // u32
        AstNodeUIntLiteral64,    // u64
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
} AstNodeLiteral;

typedef struct {
    Buff* module;
    Buff* name;
    AstNode* expr;
    AstNodeLiteral* literal;
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
    Vec* params;
    bool is_variadic;
} AstNodeParamList;

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
    Loc* loc;

    union {
        AstNodeIdentifier* identifier;
        AstNodeTypeDecl* type_decl;
        AstNodeDecl* decl;
        AstNodeExpression* expr;
        AstNodeStatement* stmt;
        AstNodeLiteral* literal;
        AstNodeScopeObject* scope_obj;
        AstNodeTestDecl* test_decl;
        AstNodeTestExpr* test_expr;
        AstNodePrefixOpExpr* prefix_op_expr;
        AstNodeParamDecl* param_decl;
        AstNodeParamList* param_list;
        AstNodeArrayAccessExpr* array_access_expr;
        AstNodeInferredArrayType* inferred_array_type;
        AstNodeArrayType* array_type;
        AstNodeFieldAccessExpr* field_access_expr;
    } data;
};

#endif // ADORAD_AST_H