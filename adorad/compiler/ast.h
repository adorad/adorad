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
#include <adorad/core/vector.h>

typedef struct AstNode AstNode;

typedef enum AstNodeKind {
    NodeKindIdentifier = 0,
    NodeKindBlock,

    // Functions
    NodeKindFuncPrototype,
    NodeKindFuncDef,      // 

    // Literals
    NodeKindIntLiteral,    // `2`
    NodeKindFloatLiteral,  // `3.4224829`
    NodeKindCharLiteral,   // 'a', 'b'...
    NodeKindStringLiteral, // 'str` or "str"
    NodeKindBoolLiteral,   // `bool`
    NodeKindNillLiteral,   // `nil`

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

typedef struct AstNodeFuncDef {
    AstNode* prototype;
    AstNode* body;
} AstNodeFuncDef;

enum FuncInline {
    FI_AUTO,
    FI_INLINE,
    FI_NOINLINE
};

typedef struct AstNodeFuncPrototype {
    char* name;
    cstlVector* params;  // vector of `AstNode*`s -> similar to cstlVector<AstNode*> if C had generics
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

typedef struct AstNodeFuncCallExpr {
    AstNode* func_call_expr;
    cstlVector* params;
    enum FuncCallModifier modifier;
} AstNodeFuncCallExpr;

typedef struct AstNodeParamDecls {
    char* name;
    AstNode* type;
    bool is_alias;
    bool is_var_args;
} AstNodeParamDecls;

typedef struct AstNodeReturnExpr {
    AstNode* expr;
} AstNodeReturnExpr;

typedef struct AstNodeDefer {
    AstNode* expr;
} AstNodeDefer;

typedef struct AstNodeVarDecl {
    char* name;
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

typedef struct AstNodeIdentifier {
    AstNode* type;
    UInt64 tok_idx;

    bool is_const;
    bool is_export;
    bool is_mutable;  // This is false unless explicitly mentioned by the user
} AstNodeIdentifier;

typedef struct AstNodeBlock {
    char* name;
    cstlVector* statements;
} AstNodeBlock;

typedef struct AstNodeTestDecl {
    char* name;   // can be nullptr if no name
    AstNode* body;
} AstNodeTestDecl;

typedef struct AstNodeTestExpr {
    char* symbol;
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

typedef struct AstNodeBinaryOpExpr {
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

typedef struct AstNodePrefixOpExpr {
    enum PrefixOpKind prefix_op;
    AstNode* expr;
} AstNodePrefixOpExpr;

typedef struct AstNodeTryExpr {
    char* symbol;
    AstNode* target_node;
    AstNode* then_node;
    AstNode* else_node;
    char* err_symbol;
} AstNodeTryExpr;

typedef struct AstNodeCatchExpr {
    AstNode* op1;
    AstNode* symbol; // can be nullptr
    AstNode* op2;
} AstNodeCatchExpr;

typedef struct AstNodeIfBoolExpr {
    AstNode* condition;
    AstNode* then_block;
    AstNode* else_node; // null, block node, or other `if expr` node
} AstNodeIfBoolExpr;

typedef struct AstNodeForExpr {
    char* name;
    AstNode* condition;
    AstNode* continue_expr;
    AstNode* body;
} AstNodeForExpr;

typedef struct AstNodeMatchExpr {
    AstNode* expr;
    cstlVector* patterns;
} AstNodeMatchExpr;

typedef struct AstNodeMatchRange {
    AstNode* start;
    AstNode* end;
} AstNodeMatchRange;

typedef struct AstNodeCompileTime {
    AstNode* expr;
} AstNodeCompileTime;

enum ContainerKind {
    ContainerKindEnum,
    ContainerKindUnion
};

enum ContainerLayoutKind {
    ContainerLayoutKindAuto,
    ContainerLayoutKindPacked
};

typedef struct AstNodeContainerDecl {
    cstlVector* fields;
    cstlVector* decls;
    enum ContainerKind kind;
    enum ContainerLayoutKind layout;
} AstNodeContainerDecl;

typedef struct AstNodeBoolLiteral {
    bool value;
} AstNodeBoolLiteral;

typedef struct AstNodeBreakLiteral {
    char* name;
    AstNode* expr; // can be nullptr
} AstNodeBreakLiteral;

typedef struct AstNodeContinueLiteral {
    char* name;
} AstNodeContinueLiteral;

struct AstNode {
    AstNodeKind type; // type of AST Node
    UInt64 tok_index; // token index

    union {
        AstNodeFuncDef* func_def;
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
        AstNodeBoolLiteral* bool_literal;
        AstNodeBreakLiteral* break_literal;
        AstNodeContinueLiteral* continue_literal;
    } data;
};

// The `import` statement
typedef struct AstImport {
    char* module;  // the module name
    char* alias;   // the `y` in `import x as y`
} AstImport;

// Each Adorad source file can be represented by one AstFile structure.
typedef struct AstFile {
    char* path;     // full path of the source file - `/path/to/file.ad`
    char* basepath; // file name - `file.ad` (useful for tracing)
    int num_lines;  // number of source code lines in the file (including comments)
    int num_bytes;  // number of processed source code bytes
    // TODO (jasmcaus) Change the type of `module` to an `AstNodeModule`
    char* module;   // name of the module
    bool is_test;   // true for test_*.ad files
} AstFile;

#endif // ADORAD_AST_H