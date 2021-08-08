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

typedef struct AstNode AstNode;

typedef enum {
    NodeKindIdentifier = 0,
    NodeKindBlock,        // `{ ... }

    // Functions
    NodeKindFuncPrototype,
    NodeKindFuncDef,      // `func hello()`

    // Literals
    NodeKindIntLiteral,    // `2`
    NodeKindFloatLiteral,  // `3.4224829`
    NodeKindCharLiteral,   // 'a', 'b'...
    NodeKindStringLiteral, // 'str` or "str"
    NodeKindBoolLiteral,   // `bool`
    NodeKindNilLiteral,    // `nil`

    // Declarations
    NodeKindEnumDecl,      // `enum Fruits { ... }`
    NodeKindVarDecl,       // `some_type var_name = ...`

    // Expressions
    NodeKindFuncCallExpr,  // `sayHello('Hello!')`
    NodeKindIfExpr,        // `if cond { ...}`
    NodeKindForExpr,       // `for {}`
    NodeKindForCExpr,      // `for i=0; i<10; i++ {}`
    NodeKindForInExpr,     // `for i in 0..10 {}`
    NodeKindMatchExpr,     // `match expr { ... }`
    NodeKindCatchExpr,     // `catch Error`
    NodeKindBinaryOpExpr,

    // Fields
    NodeKindTypeField,     // `type name { ... }`

    NodeKindBreak,         // `break`
    NodeKindContinue,      // `continue`
} AstNodeKind;

// Function or Method Declaration
typedef struct {
    Buff* name;
    Buff* module;      // name of the module
    Buff* parent_type; // the `type` of which the function belongs to (nullptr, if not a method)
    Buff* return_type;
    bool is_variadic;  // variadic arguments
    bool is_export;    // true for `export func abc()`
    bool is_noreturn;  // true for `[noreturn] func 
    bool is_main;      // true for `func main()`
    bool is_test;      // true for `func test_yyy()`
    bool no_body;      // true for function definitions (no function body) `func abc()`

    AstNode* parameters;
    AstNode* body;      // can be nullptr for no-body functions (just declarations)
    Location* loc;     // location of the `func` declaration
} AstNodeFuncDecl;

enum FuncInline {
    FuncInline__auto,
    FuncInline__inline,
    FuncInline__noinline
};

typedef struct {
    Buff* name;
    Vec* params;  // vector of `AstNode*`s -> similar to Vec<AstNode*> if C had generics
    AstNode* return_type;
    AstNode* func_def;

    enum FuncInline func_inline;
    bool is_export;
    bool is_extern;
    bool is_generic;
    bool is_var_args;  // variable arguments used?
} AstNodeFuncPrototype;

enum FuncCallModifier {
    FuncCallModifierNone,
    FuncCallModifierAsync,
    FuncCallModifierNeverInline,
    FuncCallModifierAlwaysInline,
    FuncCallModifierCompileTime
};

typedef struct {
    AstNode* func_call_expr;
    Vec* params;
    enum FuncCallModifier modifier;
} AstNodeFuncCallExpr;

typedef struct {
    Buff* name;
    AstNode* type;
    bool is_alias;
    bool is_var_args;
} AstNodeParamDecls;

// break/continue
typedef struct {
    Buff* name;
    AstNode* expr;  // can be nullptr (`break`). always nullptr for `continue`
    Location* loc;
    enum {
        AstNodeBranchStatementBreak,
        AstNodeBranchStatementContinue
    } type;
} AstNodeBranchStatement;

typedef struct {
    AstNode* expr;
} AstNodeReturnExpr;

typedef struct {
    AstNode* expr;
} AstNodeDefer;

typedef struct {
    Buff* name;
    AstNode* type;    // can be null
    AstNode* expr;
    UInt64 tok_index; // token index

    bool is_const;
    bool is_export;
    bool is_mutable;  // This is false unless explicitly mentioned by the user
} AstNodeVarDecl;

enum IdentifierKind {
    IdentifierKindBlankIdent, // `_`
    IdentifierKindVariable,
    IdentifierKindConst,
    IdentifierKindFunction
};

typedef struct {
    AstNode* type;
    UInt64 tok_idx;

    bool is_const;
    bool is_export;
    bool is_mutable;  // This is false unless explicitly mentioned by the user
} AstNodeIdentifier;

// `{ ... }`
typedef struct {
    Buff* name;
    Vec* statements;
} AstNodeBlock;

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

enum BinaryOpKind {
    BinaryOpKindAssignmentInvalid,
    BinaryOpKindAssignmentPlus,    // =+
    BinaryOpKindAssignmentMinus,   // =-
    BinaryOpKindAssignmentMult,    // =*
    BinaryOpKindAssignmentDiv,     // =/
    BinaryOpKindAssignmentMod,     // =%
    BinaryOpKindAssignmentBitshiftLeft,  // =<<
    BinaryOpKindAssignmentBitshiftRight, // =>>
    BinaryOpKindAssignmentBitAnd,  // &
    BinaryOpKindAssignmentBitOr,   // |
    BinaryOpKindAssignmentBitXor,  // ^
    BinaryOpKindBoolAnd,
    BinaryOpKindBoolOr,
    BinaryOpKindCmpEqual,                 // ==
    BinaryOpKindCmpNotEqual,              // !=
    BinaryOpKindCmpLessThan,              // <
    BinaryOpKindCmpGreaterThan,           // >
    BinaryOpKindCmpLessThanorEqualTo,     // <=
    BinaryOpKindCmpGreaterThanorEqualTo,  // >=
    BinaryOpKindBinaryAnd,
    BinaryOpKindBinaryOr,
    BinaryOpKindBinaryXor,
    BinaryOpKindBitshitLeft,   // <<
    BinaryOpKindBitshitRight,  // >>
    BinaryOpKindAdd,      // +
    BinaryOpKindSubtract, // -
    BinaryOpKindMult,     // *
    BinaryOpKindDiv,      // /
    BinaryOpKindMod,      // %
    BinaryOpKindTensorSlice,
    BinaryOpKindTensorMult
};

typedef struct {
    AstNode* op1;
    enum BinaryOpKind binary_op;
    AstNode* op2;
} AstNodeBinaryOpExpr;

enum PrefixOpKind {
    PrefixOpKindInvalid,
    PrefixOpKindBoolNot,
    PrefixOpKindBinaryNot, 
    PrefixOpKindNegation,  // !var
    PrefixOpKindAddrOf     // &var
};

typedef struct {
    enum PrefixOpKind prefix_op;
    AstNode* expr;
} AstNodePrefixOpExpr;

typedef struct {
    Buff* symbol;
    AstNode* target_node;
    AstNode* then_node;
    AstNode* else_node;
    Buff* err_symbol;
} AstNodeTryExpr;

typedef struct {
    AstNode* op1;
    AstNode* symbol; // can be nullptr
    AstNode* op2;
} AstNodeCatchExpr;

typedef struct {
    AstNode* condition;
    AstNode* then_block;
    AstNode* else_node; // null, block node, or other `if expr` node
} AstNodeIfBoolExpr;

typedef struct {
    Buff* name;
    AstNode* condition;
    AstNode* continue_expr;
    AstNode* body;
} AstNodeForExpr;

typedef struct {
    AstNode* expr;
    Vec* patterns;
} AstNodeMatchExpr;

typedef struct {
    AstNode* start;
    AstNode* end;
} AstNodeMatchRange;

typedef struct {
    AstNode* expr;
} AstNodeCompileTime;

// enum / union
typedef struct {
    Vec* fields;
    Vec* decls;
    enum {
        ContainerKindEnum,
        ContainerKindUnion
    } kind;
    enum {
        ContainerLayoutKindAuto,
        ContainerLayoutKindPacked
    } layout;
} AstNodeContainerDecl;

typedef struct {
    Buff* value;
    Location* loc;
    // TODO (jasmcaus) - Come up with a workaround for this
    enum {
        AstNodeIntLiteral8,
        AstNodeIntLiteral16,
        AstNodeIntLiteral32, // default
        AstNodeIntLiteral64,
        // AstNodeIntLiteral128 // will be supported later
    } type;
} AstNodeIntLiteral;

typedef struct {
    Buff* value;
    Location* loc;
    // TODO (jasmcaus) - Come up with a workaround for this
    enum {
        AstNodeFloatLiteral32,    // default
        AstNodeFloatLiteral64,
        // AstNodeFloatLiteral128 // will be supported later
    } type;
} AstNodeFloatLiteral;

typedef struct {
    Buff* value;
    Location* loc;
    bool is_special;   // format / raw string
    enum {
        AstNodeStringLiteralNone,   // if `is_special` is false
        AstNodeStringLiteralRaw,    // `r"abc"`
        AstNodeStringLiteralFormat  // `f"name = {name}"`
    } type;
} AstNodeStringLiteral;

typedef struct {
    Buff* value;
    Location* loc;
} AstNodeCharLiteral;

typedef struct {
    Buff* value;
    Location* loc;
} AstNodeBoolLiteral;

struct AstNode {
    AstNodeKind type; // type of AST Node
    UInt64 tok_index; // token index

    union {
        AstNodeFuncDecl* func_def;
        AstNodeFuncPrototype* func_proto;
        AstNodeFuncCallExpr* func_call_expr;
        AstNodeParamDecls* param_decls;
        AstNodeReturnExpr* return_type;
        AstNodeDefer* defer;
        AstNodeVarDecl* var_decl;
        AstNodeIdentifier* identifier;
        AstNodeBlock* block;
        AstNodeTestDecl* test_decl;
        AstNodeTestExpr* test_expr;
        AstNodeBinaryOpExpr* binary_op_expr;
        AstNodePrefixOpExpr* prefix_op_expr;
        AstNodeTryExpr* try_expr;
        AstNodeCatchExpr* catch_expr;
        AstNodeIfBoolExpr* if_bool_expr;
        AstNodeForExpr* for_expr;
        AstNodeMatchExpr* match_expr;
        AstNodeMatchRange* match_range;
        AstNodeCompileTime* compile_time_expr;
        AstNodeContainerDecl* container_decl;
        AstNodeBranchStatement* branch_stmt;
        AstNodeIntLiteral* int_literal;
        AstNodeFloatLiteral* float_literal;
        AstNodeStringLiteral* str_literal;
        AstNodeCharLiteral* char_literal;
        AstNodeBoolLiteral* bool_literal;
    } data;
};

// The `import` statement
typedef struct {
    Buff* module;  // the module name
    Buff* alias;   // the `y` in `import x as y`
} AstImport;

// Each Adorad source file can be represented by one AstFile structure.
typedef struct {
    Buff* path;     // full path of the source file - `/path/to/file.ad`
    Buff* basepath; // file name - `file.ad` (useful for tracing)
    int num_lines;  // number of source code lines in the file (including comments)
    int num_bytes;  // number of processed source code bytes
    // TODO (jasmcaus) Change the type of `module` to an `AstNodeModule`
    Buff* module;   // name of the module
    bool is_test;   // true for test_*.ad files
} AstFile;

#endif // ADORAD_AST_H