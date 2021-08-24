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
    AstNodeKindFuncDef,      // `func hello()`

    // Literals
    AstNodeKindIntLiteral,    // `2`
    AstNodeKindFloatLiteral,  // `3.4224829`
    AstNodeKindCharLiteral,   // 'a', 'b'...
    AstNodeKindStringLiteral, // 'str` or "str"
    AstNodeKindBoolLiteral,   // `bool`
    AstNodeKindNilLiteral,    // `nil`

    // Declarations
    AstNodeKindEnumDecl,      // `enum Fruits { ... }`
    AstNodeKindVarDecl,       // `some_type var_name = ...`

    // Expressions
    AstNodeKindFuncCallExpr,  // `sayHello('Hello!')`
    AstNodeKindIfExpr,        // `if cond { ...}`
    AstNodeKindForExpr,       // `for {}`
    AstNodeKindForCExpr,      // `for i=0; i<10; i++ {}`
    AstNodeKindForInExpr,     // `for i in 0..10 {}`
    AstNodeKindMatchExpr,     // `match expr { ... }`
    AstNodeKindCatchExpr,     // `catch Error`
    AstNodeKindBinaryOpExpr,  // a binary expression like `&&` or `||`

    // Fields
    AstNodeKindTypeDecl,     // `type name T { ... }`  where T is one of {enum/struct}

    AstNodeKindBreak,         // `break`
    AstNodeKindContinue,      // `continue`
};

typedef enum VisibilityMode {
    VisibilityModePrivate, // default
    VisibilityModePublic   // if `export` is used
} VisibilityMode;

typedef enum AstAddressingMode {
    AstAddressingModeInvalid,   // invalid addressing mode
    AstAddressingModeCompile_time_value,  // value known at compile time
    AstAddressingModeVariable, // addressable variable (known at runtime)
    AstAddressingModeConstant, // constant
    AstAddressingModeType,     // type  
} AstAddressingMode;

typedef enum AstLanguage {
    AstLanguageAdorad,
    AstLanguageC,
    AstLanguageAmd64,
    AstLanguageI386,
    AstLanguageArm64,
    AstLanguageArm32,
    AstLanguageRv64,
    AstLanguageRv32,
} AstLanguage;

typedef struct AstNodeAliasDecl {
    Buff* name;
    Buff* parent_type;
    // AstNodeType* parent_type;
    Location* loc;
    Location* parent_type_loc;
} AstNodeAliasDecl;

typedef struct AstNodeConstantDecl {
    bool is_export;
    bool is_block;  // `const ( ... )`
    Location* loc;
    Vec* fields;    // various constant declarations
} AstNodeConstantDecl;

typedef struct AstNodeGlobalDecl {
    Buff* module;    // globals declared in a module, persist through that module
    bool is_block;   // `global ( ... )`
    Location* loc;
    Vec* fields;     // various global declarations
} AstNodeGlobalDecl;

typedef struct AstNodeSumTypeDecl {
    Buff* name;
    bool is_export;
    Vec* variant_types; // Vec<AstNodeType*>
    Location* loc;
} AstNodeSumTypeDecl;

typedef struct AstNodeTypeEnumDecl {
    Buff* name;
    bool is_export;
    Location* loc;
    Vec* fields;     // typedef enum value fields
    Vec* attributes; // typedef enum attributes 
} AstNodeTypeEnumDecl;

typedef struct AstNodeTypeStructDecl {
    Buff* name;
    Location* loc;
    Vec* fields;    // variables, etc
    Vec* methods;   // methods
} AstNodeTypeStructDecl;

// This can be one of:
//     | AstNodeTypeEnumDecl
//     | AstNodeTypeUnionDecl (disabled for now)
//     | AstNodeTypeStructDecl
typedef struct AstNodeTypeDecl {
    union {
        AstNodeTypeEnumDecl* enum_decl;
        // AstNodeTypeUnionDecl* union_decl;
        AstNodeTypeStructDecl* struct_decl;
    };
} AstNodeTypeDecl;

// This can be one of:
//     | AstNodeAliasDecl
//     | AstNodeTypeDecl (enum/struct)
//     | AstNodeFuncDecl
//     | AstNodeConstantDecl
//     | AstNodeGlobalDecl
//     | AstNodeSumTypeDecl 
typedef struct AstNodeDecl {
    union {
        AstNodeAliasDecl* alias_decl;
        AstNodeTypeDecl* type_decl;
        AstNodeFuncDecl* func_decl;
        AstNodeConstantDecl* const_decl;
        AstNodeGlobalDecl* global_decl;
        AstNodeSumTypeDecl* sumtype_decl;
    };
    Buff* name;
    bool is_export;
    Location* loc;
} AstNodeDecl;

typedef struct AstNodeAsCast {
    AstNodeExpression* expr;
    Location* loc;
} AstNodeAsCast;

typedef struct AstNodeCastExpr {
    AstNodeExpression* arg;
    AstNodeExpression* expr;
    Buff* type;
    bool has_arg;
    Location* loc;
} AstNodeCastExpr;

typedef struct AstNodeIfExpr {
    bool is_compiletime;
    bool is_expr;
    bool has_else;
    TokenKind tokenkind;
    AstNodeExpression* left;
    Vec* branches;
    Location* loc;
} AstNodeIfExpr;

typedef struct AstNodeForExpr {
    Buff* label;
    AstNodeExpression* cond;
    Vec* statements;
    Location* loc;
    AstNodeScope* scope;
} AstNodeForExpr;

typedef struct AstNodeForCExpr {
    Buff* label;
    AstNodeStatement* init;
    bool has_init;
    AstNodeExpression* cond;
    bool has_cond;
    AstNodeStatement* updation;  // increment/decrement
    bool has_updation;
    Vec* statements;
    Location* loc;
    AstNodeScope* scope;
} AstNodeForCExpr;

typedef struct AstNodeForInExpr {
    Buff* label;
    Buff* key_var;
    Buff* val_var;
    bool is_val_var_mutable;
    AstNodeExpression* cond;
    bool is_range;
    Vec* statements;
    TokenKind tokenkind;
    Location* loc;
    AstNodeScope* scope;
} AstNodeForInExpr;

typedef struct AstNodeMatchExpr {
    TokenKind tokenkind;
    AstNodeExpression* cond;
    Vec* branches;
    Location* loc;
    bool is_sumtype;
} AstNodeMatchExpr;

typedef struct AstNodeTypeOfExpr {
    AstNodeExpression* expr;
    Location* loc;
} AstNodeTypeOfExpr;

// This can be one of:
//     | AstNodeAsCast
//     | AstNodeCastExpr
//     | AstNodeIfExpr   
//     | AstNodeForExpr
//     | AstNodeForCExpr   
//     | AstNodeForInExpr
//     | AstNodeFuncCallExpr
//     | AstNodeMatchExpr
//     | AstNodeCatchExpr
//     | AstNodeBinaryOpExpr
//     | AstNodeTypeExpr
//     | AstNodeTypeOfExpr
typedef struct AstNodeExpression {
    union {
        AstNodeAsCast*  as_cast;
        AstNodeCastExpr* cast_expr;
        AstNodeIfExpr* if_expr;
        AstNodeForExpr* for_expr;
        AstNodeForCExpr* for_c_expr;
        AstNodeForInExpr* for_in_expr;
        AstNodeFuncCallExpr* func_call_expr;
        AstNodeMatchExpr* match_expr;
        AstNodeCatchExpr* catch_expr;
        AstNodeBinaryOpExpr* binary_op_expr;
        // AstNodeTypeExpr* type_expr;
        AstNodeTypeOfExpr* typeof_expr;
    };
} AstNodeExpression;

// This can be one of:
//     | AstNodeAssignmentStatement
//     | AstNodeBlock
//     | AstNodeBranchStatement
//     | AstNodeDeferStatement
//     | AstNodeEmptyStatement
//     | AstNodeExpression
//     | AstNodeFuncDecl (prototype only)
//     | AstNodeImportStatement
//     | AstNodeModuleStatement
//     | AstNodeReturnStatement
//     | AstNodeTypeDecl (prototype only)
typedef struct AstNodeStatement {
    union{
        AstNodeAssignmentStatement* assign_stmt;
        AstNodeBlock* block_stmt;
        AstNodeBranchStatement* branch_stmt;
        AstNodeDeferStatement* defer_stmt;
        AstNodeEmptyStatement* empty_stmt;
        AstNodeExpression* expr_stmt;
        AstNodeFuncDecl* func_proto_decl;
        AstNodeImportStatement* import_stmt;
        AstNodeModuleStatement* module_stmt;
        AstNodeReturnStatement* return_stmt;
        AstNodeTypeDecl* type_proto_decl;
    };
    Location* loc;
} AstNodeStatement;

// This can be one of:
//     | AstNodeConstField
//     | AstNodeGlobalField
//     | AstNodeVariable
typedef struct AstNodeScopeObject {
    union {
        AstNodeConstField* const_field;
        AstNodeGlobalField* global_field;
        AstNodeVariable* var;
    };
    Buff* name;
    // AstNodeType* type;
} AstNodeScopeObject;

// This can be one of:
//     | AstNodeEmptyExpression
//     | AstNodeBoolLiteral
//     | AstNodeByteLiteral
//     | AstNodeFloatLiteral (32/64)
//     | AstNodeIntegerLiteral (8/16/32/64 + unsigned versions) 
//     | AstNodeCharLiteral
//     | AstNodeStringLiteral
typedef struct AstNodeCompileTimeValue {
    union {
        AstNodeEmptyExpression* empty_expr;
        AstNodeBoolLiteral* bool_value;
        AstNodeByteLiteral* byte_value;
        AstNodeFloatLiteral* float_value;
        AstNodeIntegerLiteral* int_value;
        AstNodeCharLiteral* char_value;
        AstNodeStringLiteral* str_value;
    }
} AstNodeCompileTimeValue;

// // There are only 4 possible versions of a Type info you will use:
// //    1. Enum
// //    2. Union
// //    3. Sum Type
// //    4. Interface (can be generic)
// struct {
//     union {
//         AstNode
//     };
// };

// Function or Method Declaration
typedef struct AstNodeFuncDecl {
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

typedef enum FuncInline {
    FuncInlineAuto,
    FuncInlineInline,
    FuncInlineNoinline
} FuncInline;

typedef struct AstNodeFuncPrototype {
    Buff* name;
    Vec* params;  // vector of `AstNode*`s -> similar to Vec<AstNode*> if C had generics
    AstNode* return_type;
    AstNode* func_def;

    typedef enum FuncInline func_inline;
    bool is_export;
    bool is_extern;
    bool is_generic;
    bool is_var_args;  // variable arguments used?
} AstNodeFuncPrototype;

typedef enum FuncCallModifier {
    FuncCallModifierNone,
    FuncCallModifierAsync,
    FuncCallModifierNeverInline,
    FuncCallModifierAlwaysInline,
    FuncCallModifierCompileTime
} FuncCallModifier;

typedef struct AstNodeFuncCallExpr {
    AstNode* func_call_expr;
    Vec* params;
    typedef enum FuncCallModifier modifier;
} AstNodeFuncCallExpr;

typedef struct AstNodeParamDecls {
    Buff* name;
    AstNode* type;
    bool is_alias;
    bool is_var_args;
} AstNodeParamDecls;

// break/continue
typedef struct AstNodeBranchStatement {
    Buff* name;
    AstNode* expr;  // can be nullptr (`break`). always nullptr for `continue`
    Location* loc;
    typedef enum {
        AstNodeBranchStatementBreak,
        AstNodeBranchStatementContinue
    } type;
} AstNodeBranchStatement;

typedef struct AstNodeReturnExpr {
    AstNode* expr;
} AstNodeReturnExpr;

typedef struct AstNodeDeferStatement {
    AstNode* expr;
} AstNodeDeferStatement;

typedef struct AstNodeVarDecl {
    Buff* name;
    AstNode* type;    // can be null
    AstNode* expr;
    UInt64 tok_index; // token index

    bool is_const;
    bool is_export;
    bool is_mutable;  // This is false unless explicitly mentioned by the user
} AstNodeVarDecl;

typedef enum IdentifierKind {
    IdentifierKindUnresolved,
    IdentifierKindBlankident, // `_`
    IdentifierKindVariable,
    IdentifierKindConst,
    IdentifierKindFunction,
    IdentifierKindGlobal,     // if declared within a `global` scope
} IdentifierKind;

typedef struct AstNodeIdentifier {
    AstNode* type;
    UInt64 tok_idx;

    bool is_const;
    bool is_export;
    bool is_mutable;  // This is false unless explicitly mentioned by the user
} AstNodeIdentifier;

// `{ ... }`
typedef struct AstNodeBlock {
    Buff* name;
    Vec* statements;
} AstNodeBlock;

typedef struct AstNodeTestDecl {
    Buff* name;   // can be nullptr if no name
    AstNode* body;
} AstNodeTestDecl;

typedef struct AstNodeTestExpr {
    Buff* symbol;
    AstNode* target_node;
    AstNode* then_node;
    AstNode* else_node;  // null, block node, or an `if expr` node
} AstNodeTestExpr;

// TODO(jasmcaus): Work on this
typedef enum UnaryOpKind {
    UnaryOpKindNot,
    UnaryOpKindInvert,
} UnaryOpKind;

typedef struct AstNodeUnaryOpExpr {
    AstNode* op1;
    BinaryOpKind binary_op;
    AstNode* op2;
} AstNodeUnaryOpExpr;

typedef enum ComparisonOpKind {
    ComparisonOpKindEqual,
    ComparisonOpKindNotEqual,
    ComparisonOpKindLessThan,
    ComparisonOpKindLessThanOrEqualTo,
    ComparisonOpKindGreaterThan,
    ComparisonOpKindGreaterThanOrEqualTo,
    ComparisonOpKindIs,
    ComparisonOpKindIsNot,
    ComparisonOpKindIn,
    ComparisonOpKindNotIn,
} ComparisonOpKind;

typedef struct AstNodeComparisonOpExpr {
    AstNode* op1;
    BinaryOpKind binary_op;
    AstNode* op2;
} AstNodeComparisonOpExpr;

typedef enum BinaryOpKind {
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
} BinaryOpKind;

typedef struct AstNodeBinaryOpExpr {
    AstNode* op1;
    BinaryOpKind binary_op;
    AstNode* op2;
} AstNodeBinaryOpExpr;

// TODO(jasmcaus): Do we need an `OperatorKind` and `AstNodeOperator`?
typedef enum OperatorKind {
    OperatorKindAdd,
    OperatorKindSub,
    OperatorKindMult,
    OperatorKindMatMult,
    OperatorKindDiv,
    OperatorKindMod,
    OperatorKindPow,
    OperatorKindLShift,
    OperatorKindRShift,
    OperatorKindBitOr,
    OperatorKindBitXor,
    OperatorKindBitAnd,
    OperatorKindFloorDiv,
} OperatorKind;

typedef struct AstNodeOperator {
    Buff* delete_me;
} AstNodeOperator;

typedef enum PrefixOpKind {
    PrefixOpKindInvalid,
    PrefixOpKindBoolNot,
    PrefixOpKindBinaryNot, 
    PrefixOpKindNegation,  // !var
    PrefixOpKindAddrOf     // &var
} PrefixOpKind;

typedef struct AstNodePrefixOpExpr {
    PrefixOpKind prefix_op;
    AstNode* expr;
} AstNodePrefixOpExpr;

typedef struct AstNodeTryExpr {
    Buff* symbol;
    AstNode* target_node;
    AstNode* then_node;
    AstNode* else_node;
    Buff* err_symbol;
} AstNodeTryExpr;

typedef struct AstNodeCatchExpr {
    AstNode* op1;
    AstNode* symbol; // can be nullptr
    AstNode* op2;
} AstNodeCatchExpr;

typedef struct AstNodeCompileTime {
    AstNode* expr;
} AstNodeCompileTime;

// typedef enum / union
typedef struct AstNodeContainerDecl {
    Vec* fields;
    Vec* decls;
    typedef enum {
        ContainerKindEnum,
        ContainerKindUnion,
    } kind;
    typedef enum {
        ContainerLayoutKindAuto,
        ContainerLayoutKindPacked
    } layout;
} AstNodeContainerDecl;

typedef struct AstNodeIntegerLiteral {
    Buff* value;
    Location* loc;
    // TODO (jasmcaus) - Come up with a workaround for this
    typedef enum {
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

typedef struct AstNodeFloatLiteral {
    Buff* value;
    Location* loc;
    // TODO (jasmcaus) - Come up with a workaround for this
    typedef enum {
        AstNodeFloatLiteral32,    // default
        AstNodeFloatLiteral64,
        // AstNodeFloatLiteral128 // will be supported later
    } type;
} AstNodeFloatLiteral;

typedef struct AstNodeStringLiteral {
    Buff* value;
    Location* loc;
    bool is_special;   // format / raw string
    typedef enum {
        AstNodeStringLiteralNone,   // if `is_special` is false
        AstNodeStringLiteralRaw,    // `r"abc"`
        AstNodeStringLiteralFormat  // `f"name = {name}"`
    } type;
} AstNodeStringLiteral;

typedef struct AstNodeCharLiteral {
    Buff* value;
    Location* loc;
} AstNodeCharLiteral;

typedef struct AstNodeRuneLiteral {
    Buff* value;
    Location* loc;
} AstNodeRuneLiteral;

typedef struct AstNodeBoolLiteral {
    Buff* value;
    Location* loc;
} AstNodeBoolLiteral;

struct AstNode {
    AstNodeKind kind; // type of AST Node
    UInt64 tok_index; // token index

    union {
        AstNodeFuncDecl* func_def;
        AstNodeFuncPrototype* func_proto;
        AstNodeFuncCallExpr* func_call_expr;
        AstNodeParamDecls* param_decls;
        AstNodeReturnExpr* return_type;
        AstNodeDeferStatement* defer_stmt;
        AstNodeVarDecl* var_decl;
        AstNodeIdentifier* identifier;
        AstNodeBlock* block;
        AstNodeTestDecl* test_decl;
        AstNodeTestExpr* test_expr;
        AstNodeBinaryOpExpr* binary_op_expr;
        AstNodePrefixOpExpr* prefix_op_expr;
        AstNodeTryExpr* try_expr;
        AstNodeCatchExpr* catch_expr;
        AstNodeForExpr* for_expr;
        AstNodeMatchExpr* match_expr;
        AstNodeCompileTime* compile_time_expr;
        AstNodeContainerDecl* container_decl;
        AstNodeBranchStatement* branch_stmt;
        AstNodeIntegerLiteral* int_literal;
        AstNodeFloatLiteral* float_literal;
        AstNodeStringLiteral* str_literal;
        AstNodeCharLiteral* char_literal;
        AstNodeBoolLiteral* bool_literal;
    } data;
};

// The `import` statement
typedef struct AstImport {
    Buff* module;  // the module name
    Buff* alias;   // the `y` in `import x as y`
} AstImport;

// Each Adorad source file can be represented by one AstFile structure.
typedef struct AstFile {
    Buff* path;     // full path of the source file - `/path/to/file.ad`
    Buff* basepath; // file name - `file.ad` (useful for tracing)
    int num_lines;  // number of source code lines in the file (including comments)
    int num_bytes;  // number of processed source code bytes
    // TODO (jasmcaus) Change the type of `module` to an `AstNodeModule`
    Buff* module;   // name of the module
    bool is_test;   // true for test_*.ad files
} AstFile;

typedef struct AstNodeScope {
    Buff* scope;
} AstNodeScope;

#endif // ADORAD_AST_H