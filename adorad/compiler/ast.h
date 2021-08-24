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
typedef enum VisibilityMode VisibilityMode;
typedef enum AstNodeKind AstNodeKind;
typedef enum AstAddressingMode AstAddressingMode;
typedef enum AstLanguage AstLanguage;
typedef struct AstNodeExpression AstNodeExpression;
typedef struct AstNodeStatement AstNodeStatement;
typedef struct AstNodeAliasDecl AstNodeAliasDecl;
typedef struct AstNodeConstantDecl AstNodeConstantDecl;
typedef struct AstNodeGlobalDecl AstNodeGlobalDecl;
typedef struct AstNodeSumTypeDecl AstNodeSumTypeDecl;
typedef struct AstNodeTypeEnumDecl AstNodeTypeEnumDecl;
typedef struct AstNodeTypeStructDecl AstNodeTypeStructDecl;
typedef struct AstNodeTypeDecl AstNodeTypeDecl;
typedef struct AstNodeDecl AstNodeDecl;
typedef struct AstNodeAsCast AstNodeAsCast;
typedef struct AstNodeCastExpr AstNodeCastExpr;
typedef struct AstNodeIfExpr AstNodeIfExpr;
typedef struct AstNodeForExpr AstNodeForExpr;
typedef struct AstNodeForCExpr AstNodeForCExpr;
typedef struct AstNodeForInExpr AstNodeForInExpr;
typedef struct AstNodeMatchExpr AstNodeMatchExpr;
typedef struct AstNodeReturnExpr AstNodeReturnExpr;
typedef struct AstNodeTypeOfExpr AstNodeTypeOfExpr;
typedef struct AstNodeCompileTimeValue AstNodeCompileTimeValue;
typedef struct AstNodeFuncDecl AstNodeFuncDecl;
typedef struct AstNodeFuncPrototype AstNodeFuncPrototype;
typedef struct AstNodeFuncCallExpr AstNodeFuncCallExpr;
typedef struct AstNodeParamDecls AstNodeParamDecls;
typedef struct AstNodeBranchStatement AstNodeBranchStatement;
typedef struct AstNodeDeferStatement AstNodeDeferStatement;
typedef struct AstNodeVarDecl AstNodeVarDecl;
typedef struct AstNodeIdentifier AstNodeIdentifier;
typedef struct AstNodeTestDecl AstNodeTestDecl;
typedef struct AstNodeTestExpr AstNodeTestExpr;
typedef struct AstNodeBinaryOpExpr AstNodeBinaryOpExpr;
typedef struct AstNodePrefixOpExpr AstNodePrefixOpExpr;
typedef struct AstNodeTryExpr AstNodeTryExpr;
typedef struct AstNodeCatchExpr AstNodeCatchExpr;
typedef struct AstNodeContainerDecl AstNodeContainerDecl;
typedef struct AstNodeCompileTime AstNodeCompileTime;
typedef struct AstNodeIntegerLiteral AstNodeIntegerLiteral;
typedef struct AstNodeFloatLiteral AstNodeFloatLiteral;
typedef struct AstNodeStringLiteral AstNodeStringLiteral;
typedef struct AstNodeCharLiteral AstNodeCharLiteral;
typedef struct AstNodeRuneLiteral AstNodeRuneLiteral;
typedef struct AstNodeBoolLiteral AstNodeBoolLiteral;
typedef struct AstNodeScope AstNodeScope;
typedef struct AstImport AstImport;
typedef struct AstFile AstFile;

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

enum VisibilityMode {
    VisibilityModePrivate, // default
    VisibilityModePublic   // if `export` is used
};

enum AstAddressingMode {
    AstAddressingModeInvalid,   // invalid addressing mode
    AstAddressingModeCompile_time_value,  // value known at compile time
    AstAddressingModeVariable, // addressable variable (known at runtime)
    AstAddressingModeConstant, // constant
    AstAddressingModeType,     // type  
};

enum AstLanguage {
    AstLanguageAdorad,
    AstLanguageC,
    AstLanguageAmd64,
    AstLanguageI386,
    AstLanguageArm64,
    AstLanguageArm32,
    AstLanguageRv64,
    AstLanguageRv32,
};

struct AstNodeAliasDecl {
    Buff* name;
    Buff* parent_type;
    // AstNodeType* parent_type;
    Location* loc;
    Location* parent_type_loc;
};

struct AstNodeConstantDecl {
    bool is_export;
    bool is_block;  // `const ( ... )`
    Location* loc;
    Vec* fields;    // various constant declarations
};

struct AstNodeGlobalDecl {
    Buff* module;    // globals declared in a module, persist through that module
    bool is_block;   // `global ( ... )`
    Location* loc;
    Vec* fields;     // various global declarations
};

struct AstNodeSumTypeDecl {
    Buff* name;
    bool is_export;
    Vec* variant_types; // Vec<AstNodeType*>
    Location* loc;
};

struct AstNodeTypeEnumDecl {
    Buff* name;
    bool is_export;
    Location* loc;
    Vec* fields;     // enum value fields
    Vec* attributes; // enum attributes 
};

struct AstNodeTypeStructDecl {
    Buff* name;
    Location* loc;
    Vec* fields;    // variables, etc
    Vec* methods;   // methods
};

// This can be one of:
//     | AstNodeTypeEnumDecl
//     | AstNodeTypeUnionDecl (disabled for now)
//     | AstNodeTypeStructDecl
struct AstNodeTypeDecl {
    union {
        AstNodeTypeEnumDecl* enum_decl;
        // AstNodeTypeUnionDecl* union_decl;
        AstNodeTypeStructDecl* struct_decl;
    };
};

// This can be one of:
//     | AstNodeAliasDecl
//     | AstNodeTypeDecl (enum/struct)
//     | AstNodeFuncDecl
//     | AstNodeConstantDecl
//     | AstNodeGlobalDecl
//     | AstNodeSumTypeDecl 
struct AstNodeDecl {
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
};

struct AstNodeAsCast {
    AstNodeExpression* expr;
    Location* loc;
};

struct AstNodeCastExpr {
    AstNodeExpression* arg;
    AstNodeExpression* expr;
    Buff* type;
    bool has_arg;
    Location* loc;
};

struct AstNodeIfExpr {
    bool is_compiletime;
    bool is_expr;
    bool has_else;
    TokenKind tokenkind;
    AstNodeExpression* left;
    Vec* branches;
    Location* loc;
};

struct AstNodeForExpr {
    Buff* label;
    AstNodeExpression* cond;
    Vec* statements;
    Location* loc;
    AstNodeScope* scope;
};

struct AstNodeForCExpr {
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
};

struct AstNodeForInExpr {
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
};

struct AstNodeMatchExpr {
    TokenKind tokenkind;
    AstNodeExpression* cond;
    Vec* branches;
    Location* loc;
    bool is_sumtype;
};

struct AstNodeTypeOfExpr {
    AstNodeExpression* expr;
    Location* loc;
};

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
struct AstNodeExpression {
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
};

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
struct AstNodeStatement {
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
};

// This can be one of:
//     | AstNodeConstField
//     | AstNodeGlobalField
//     | AstNodeVariable
struct AstNodeScopeObject {
    union {
        AstNodeConstField* const_field;
        AstNodeGlobalField* global_field;
        AstNodeVariable* var;
    };
    Buff* name;
    // AstNodeType* type;
};

// This can be one of:
//     | AstNodeEmptyExpression
//     | AstNodeBoolLiteral
//     | AstNodeByteLiteral
//     | AstNodeFloatLiteral (32/64)
//     | AstNodeIntegerLiteral (8/16/32/64 + unsigned versions) 
//     | AstNodeCharLiteral
//     | AstNodeStringLiteral
struct AstNodeCompileTimeValue {
    union {
        AstNodeEmptyExpression* empty_expr;
        AstNodeBoolLiteral* bool_value;
        AstNodeByteLiteral* byte_value;
        AstNodeFloatLiteral* float_value;
        AstNodeIntegerLiteral* int_value;
        AstNodeCharLiteral* char_value;
        AstNodeStringLiteral* str_value;
    }
};

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
struct AstNodeFuncDecl {
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
};

enum FuncInline {
    FuncInline__auto,
    FuncInline__inline,
    FuncInline__noinline
};

struct AstNodeFuncPrototype {
    Buff* name;
    Vec* params;  // vector of `AstNode*`s -> similar to Vec<AstNode*> if C had generics
    AstNode* return_type;
    AstNode* func_def;

    enum FuncInline func_inline;
    bool is_export;
    bool is_extern;
    bool is_generic;
    bool is_var_args;  // variable arguments used?
};

enum FuncCallModifier {
    FuncCallModifier__None,
    FuncCallModifier__Async,
    FuncCallModifier__NeverInline,
    FuncCallModifier__AlwaysInline,
    FuncCallModifier__CompileTime
};

struct AstNodeFuncCallExpr {
    AstNode* func_call_expr;
    Vec* params;
    enum FuncCallModifier modifier;
};

struct AstNodeParamDecls {
    Buff* name;
    AstNode* type;
    bool is_alias;
    bool is_var_args;
};

// break/continue
struct AstNodeBranchStatement {
    Buff* name;
    AstNode* expr;  // can be nullptr (`break`). always nullptr for `continue`
    Location* loc;
    enum {
        AstNodeBranchStatementBreak,
        AstNodeBranchStatementContinue
    } type;
};

struct AstNodeReturnExpr {
    AstNode* expr;
};

struct AstNodeDeferStatement {
    AstNode* expr;
};

struct AstNodeVarDecl {
    Buff* name;
    AstNode* type;    // can be null
    AstNode* expr;
    UInt64 tok_index; // token index

    bool is_const;
    bool is_export;
    bool is_mutable;  // This is false unless explicitly mentioned by the user
};

enum IdentifierKind {
    IdentifierKind__unresolved,
    IdentifierKind__blankident, // `_`
    IdentifierKind__variable,
    IdentifierKind__const,
    IdentifierKind__function,
    IdentifierKind__global,     // if declared within a `global` scope
};

struct AstNodeIdentifier {
    AstNode* type;
    UInt64 tok_idx;

    bool is_const;
    bool is_export;
    bool is_mutable;  // This is false unless explicitly mentioned by the user
};

// `{ ... }`
struct {
    Buff* name;
    Vec* statements;
};

struct AstNodeTestDecl {
    Buff* name;   // can be nullptr if no name
    AstNode* body;
};

struct AstNodeTestExpr {
    Buff* symbol;
    AstNode* target_node;
    AstNode* then_node;
    AstNode* else_node;  // null, block node, or an `if expr` node
};

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

struct AstNodeBinaryOpExpr {
    AstNode* op1;
    enum BinaryOpKind binary_op;
    AstNode* op2;
};

enum PrefixOpKind {
    PrefixOpKind__Invalid,
    PrefixOpKind__BoolNot,
    PrefixOpKind__BinaryNot, 
    PrefixOpKind__Negation,  // !var
    PrefixOpKind__AddrOf     // &var
};

struct AstNodePrefixOpExpr {
    enum PrefixOpKind prefix_op;
    AstNode* expr;
};

struct AstNodeTryExpr {
    Buff* symbol;
    AstNode* target_node;
    AstNode* then_node;
    AstNode* else_node;
    Buff* err_symbol;
};

struct AstNodeCatchExpr {
    AstNode* op1;
    AstNode* symbol; // can be nullptr
    AstNode* op2;
};

struct AstNodeCompileTime {
    AstNode* expr;
};

// enum / union
struct AstNodeContainerDecl {
    Vec* fields;
    Vec* decls;
    enum {
        ContainerKind__Enum,
        ContainerKind__Union,
    } kind;
    enum {
        ContainerLayoutKind__Auto,
        ContainerLayoutKind__Packed
    } layout;
};

struct AstNodeIntegerLiteral {
    Buff* value;
    Location* loc;
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
};

struct AstNodeFloatLiteral {
    Buff* value;
    Location* loc;
    // TODO (jasmcaus) - Come up with a workaround for this
    enum {
        AstNodeFloatLiteral32,    // default
        AstNodeFloatLiteral64,
        // AstNodeFloatLiteral128 // will be supported later
    } type;
};

struct AstNodeStringLiteral {
    Buff* value;
    Location* loc;
    bool is_special;   // format / raw string
    enum {
        AstNodeStringLiteralNone,   // if `is_special` is false
        AstNodeStringLiteralRaw,    // `r"abc"`
        AstNodeStringLiteralFormat  // `f"name = {name}"`
    } type;
};

struct AstNodeCharLiteral {
    Buff* value;
    Location* loc;
};

struct AstNodeRuneLiteral {
    Buff* value;
    Location* loc;
};

struct AstNodeBoolLiteral {
    Buff* value;
    Location* loc;
};

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
struct AstImport {
    Buff* module;  // the module name
    Buff* alias;   // the `y` in `import x as y`
};

// Each Adorad source file can be represented by one AstFile structure.
struct AstFile {
    Buff* path;     // full path of the source file - `/path/to/file.ad`
    Buff* basepath; // file name - `file.ad` (useful for tracing)
    int num_lines;  // number of source code lines in the file (including comments)
    int num_bytes;  // number of processed source code bytes
    // TODO (jasmcaus) Change the type of `module` to an `AstNodeModule`
    Buff* module;   // name of the module
    bool is_test;   // true for test_*.ad files
};

struct AstNodeScope {
    Buff* scope;
};

#endif // ADORAD_AST_H