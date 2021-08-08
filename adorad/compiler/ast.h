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

typedef struct AstNode__ AstNode__;

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
} AstNode__Kind;

typedef enum {
    AstAddressingMode__invalid,   // invalid addressing mode
    AstAddressingMode__compile_time_value,  // value known at compile time
    AstAddressingMode__variable, // addressable variable (known at runtime)
    AstAddressingMode__constant, // constant
    AstAddressingMode__type,     // type  
} AstAddressingMode;

typedef enum {
    AstLanguage__adorad,
    AstLanguage__c,
    AstLanguage__amd64,
    AstLanguage__i386,
    AstLanguage__arm64,
    AstLanguage__arm32,
    AstLanguage__rv64,
    AstLanguage__rv32,
} AstLanguage;

// This can be one of:
//     | AstNode__AliasDecl
//     | AstNode__TypeDecl (enum/union/interface)
//     | AstNode__FuncDecl
//     | AstNode__ConstantDecl
//     | AstNode__GlobalDecl
//     | AstNode__SumTypeDecl 
typedef struct {
    union {
        AstNode__AliasDecl* alias_decl;
        AstNode__TypeDecl* type_decl;
        AstNode__FuncDecl* func_decl;
        AstNode__ConstantDecl* const_decl;
        AstNode__GlobalDecl* global_decl;
        AstNode__SumTypeDecl* sumtype_decl;
    };
    Buff* name;
    bool is_export;
    Location* loc;
} AstNode__Decl;

// This can be one of:
//     | AstNode__AsCast
//     | AstNode__CallExpr
//     | AstNode__CastExpr
//     | AstNode__IfExpr   
//     | AstNode__ForExpr
//     | AstNode__ForCExpr   
//     | AstNode__ForInExpr
//     | AstNode__MatchExpr
//     | AstNode__CatchExpr
//     | AstNode__BinaryOpExpr
//     | AstNode__TypeExpr
//     | AstNode__TypeOfExpr
typedef struct {
    union {
        AstNode__AsCast*  as_cast;
        AstNode__CallExpr* call_expr;
        AstNode__CastExpr* cast_expr;
        AstNode__IfExpr* if_expr;
        AstNode__ForExpr* for_expr;
        AstNode__ForCExpr* for_c_expr;
        AstNode__ForInExpr* for_in_expr;
        AstNode__MatchExpr* match_expr;
        AstNode__CatchExpr* catch_expr;
        AstNode__BinaryOpExpr* binary_op_expr;
        AstNode__TypeExpr* type_expr;
        AstNode__TypeOfExpr* typeof_expr;
    }
} AstNode__Expression;

// This can be one of:
//     | AstNode__AssignmentStatement
//     | AstNode__Block
//     | AstNode__BranchStatement
//     | AstNode__DeferStatement
//     | AstNode__EmptyStatement
//     | AstNode__ExpressionStatement
//     | AstNode__FuncDecl (prototype only)
//     | AstNode__ImportStatement
//     | AstNode__ModuleStatement
//     | AstNode__ReturnStatement
//     | AstNode__TypeDecl (prototype only)
typedef struct {
    union{
        AstNode__AssignmentStatement* assign_stmt;
        AstNode__Block* block_stmt;
        AstNode__BranchStatement* branch_stmt;
        AstNode__DeferStatement* defer_stmt;
        AstNode__EmptyStatement* empty_stmt;
        AstNode__ExpressionStatement* expr_stmt;
        AstNode__FuncDecl* func_proto_decl;
        AstNode__ImportStatement* import_stmt;
        AstNode__ModuleStatement* module_stmt;
        AstNode__ReturnStatement* return_stmt;
        AstNode__TypeDecl* type_proto_decl;
    };
    Location* loc;
} AstNode__Statement;

// This can be one of:
//     | AstNode__ConstField
//     | AstNode__GlobalField
//     | AstNode__Variable
typedef struct {
    union {
        AstNode__ConstField* const_field;
        AstNode__GlobalField* global_field;
        AstNode__Variable* var;
    };
    Buff* name;
    // AstNode__Type* type;
} AstNode__ScopeObject;

// This can be one of:
//     | AstNode__EmptyExpression
//     | AstNode__BoolLiteral
//     | AstNode__ByteLiteral
//     | AstNode__FloatLiteral (32/64)
//     | AstNode__IntegerLiteral (8/16/32/64 + unsigned versions) 
//     | AstNode__CharLiteral
//     | AstNode__StringLiteral
typedef struct {
    union {
        AstNode__EmptyExpression* empty_expr;
        AstNode__BoolLiteral* bool_value;
        AstNode__ByteLiteral* byte_value;
        AstNode__FloatLiteral* float_value;
        AstNode__IntegerLiteral* int_value;
        AstNode__CharLiteral* char_value;
        AstNode__StringLiteral* str_value;
    }
} AstNode__CompileTimeConstantValue;

// // There are only 4 possible versions of a Type info you will use:
// //    1. Enum
// //    2. Union
// //    3. Sum Type
// //    4. Interface (can be generic)
// typedef struct {
//     union {
//         AstNode__
//     };
// } AstNode__TypeInfo;

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
} AstNode__FuncDecl;

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
} AstNode__FuncPrototype;

enum FuncCallModifier {
    FuncCallModifier__None,
    FuncCallModifier__Async,
    FuncCallModifier__NeverInline,
    FuncCallModifier__AlwaysInline,
    FuncCallModifier__CompileTime
};

typedef struct {
    AstNode* func_call_expr;
    Vec* params;
    enum FuncCallModifier modifier;
} AstNode__FuncCallExpr;

typedef struct {
    Buff* name;
    AstNode* type;
    bool is_alias;
    bool is_var_args;
} AstNode__ParamDecls;

// break/continue
typedef struct {
    Buff* name;
    AstNode* expr;  // can be nullptr (`break`). always nullptr for `continue`
    Location* loc;
    enum {
        AstNode__BranchStatementBreak,
        AstNode__BranchStatementContinue
    } type;
} AstNode__BranchStatement;

typedef struct {
    AstNode* expr;
} AstNode__ReturnExpr;

typedef struct {
    AstNode* expr;
} AstNode__Defer;

typedef struct {
    Buff* name;
    AstNode* type;    // can be null
    AstNode* expr;
    UInt64 tok_index; // token index

    bool is_const;
    bool is_export;
    bool is_mutable;  // This is false unless explicitly mentioned by the user
} AstNode__VarDecl;

enum IdentifierKind {
    IdentifierKind__unresolved,
    IdentifierKind__blankident, // `_`
    IdentifierKind__variable,
    IdentifierKind__const,
    IdentifierKind__function,
    IdentifierKind__global,     // if declared within a `global` scope
};

typedef struct {
    AstNode* type;
    UInt64 tok_idx;

    bool is_const;
    bool is_export;
    bool is_mutable;  // This is false unless explicitly mentioned by the user
} AstNode__Identifier;

// `{ ... }`
typedef struct {
    Buff* name;
    Vec* statements;
} AstNode__Block;

typedef struct {
    Buff* name;   // can be nullptr if no name
    AstNode* body;
} AstNode__TestDecl;

typedef struct {
    Buff* symbol;
    AstNode* target_node;
    AstNode* then_node;
    AstNode* else_node;  // null, block node, or an `if expr` node
} AstNode__TestExpr;

enum BinaryOpKind {
    BinaryOpKind__AssignmentInvalid,
    BinaryOpKind__AssignmentPlus,    // =+
    BinaryOpKind__AssignmentMinus,   // =-
    BinaryOpKind__AssignmentMult,    // =*
    BinaryOpKind__AssignmentDiv,     // =/
    BinaryOpKind__AssignmentMod,     // =%
    BinaryOpKind__AssignmentBitshiftLeft,  // =<<
    BinaryOpKind__AssignmentBitshiftRight, // =>>
    BinaryOpKind__AssignmentBitAnd,  // &
    BinaryOpKind__AssignmentBitOr,   // |
    BinaryOpKind__AssignmentBitXor,  // ^
    BinaryOpKind__BoolAnd,
    BinaryOpKind__BoolOr,
    BinaryOpKind__CmpEqual,                 // ==
    BinaryOpKind__CmpNotEqual,              // !=
    BinaryOpKind__CmpLessThan,              // <
    BinaryOpKind__CmpGreaterThan,           // >
    BinaryOpKind__CmpLessThanorEqualTo,     // <=
    BinaryOpKind__CmpGreaterThanorEqualTo,  // >=
    BinaryOpKind__BinaryAnd,
    BinaryOpKind__BinaryOr,
    BinaryOpKind__BinaryXor,
    BinaryOpKind__BitshitLeft,   // <<
    BinaryOpKind__BitshitRight,  // >>
    BinaryOpKind__Add,      // +
    BinaryOpKind__Subtract, // -
    BinaryOpKind__Mult,     // *
    BinaryOpKind__Div,      // /
    BinaryOpKind__Mod,      // %
    BinaryOpKind__TensorSlice,
    BinaryOpKind__TensorMult
};

typedef struct {
    AstNode* op1;
    enum BinaryOpKind binary_op;
    AstNode* op2;
} AstNode__BinaryOpExpr;

enum PrefixOpKind {
    PrefixOpKind__Invalid,
    PrefixOpKind__BoolNot,
    PrefixOpKind__BinaryNot, 
    PrefixOpKind__Negation,  // !var
    PrefixOpKind__AddrOf     // &var
};

typedef struct {
    enum PrefixOpKind prefix_op;
    AstNode* expr;
} AstNode__PrefixOpExpr;

typedef struct {
    Buff* symbol;
    AstNode* target_node;
    AstNode* then_node;
    AstNode* else_node;
    Buff* err_symbol;
} AstNode__TryExpr;

typedef struct {
    AstNode* op1;
    AstNode* symbol; // can be nullptr
    AstNode* op2;
} AstNode__CatchExpr;

typedef struct {
    AstNode* condition;
    AstNode* then_block;
    AstNode* else_node; // null, block node, or other `if expr` node
} AstNode__IfBoolExpr;

typedef struct {
    Buff* name;
    AstNode* condition;
    AstNode* continue_expr;
    AstNode* body;
} AstNode__ForExpr;

typedef struct {
    AstNode* expr;
    Vec* patterns;
} AstNode__MatchExpr;

typedef struct {
    AstNode* start;
    AstNode* end;
} AstNode__MatchRange;

typedef struct {
    AstNode* expr;
} AstNode__CompileTime;

// enum / union
typedef struct {
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
} AstNode__ContainerDecl;

typedef struct {
    Buff* value;
    Location* loc;
    // TODO (jasmcaus) - Come up with a workaround for this
    enum {
        AstNode__IntLiteral8,
        AstNode__IntLiteral16,
        AstNode__IntLiteral32, // default
        AstNode__IntLiteral64,
        // AstNode__IntLiteral128 // will be supported later
        AstNode__UIntLiteral8,
        AstNode__UIntLiteral16,
        AstNode__UIntLiteral32, // default
        AstNode__UIntLiteral64,
        // AstNode__UIntLiteral128 // will be supported later
    } type;
} AstNode__IntLiteral;

typedef struct {
    Buff* value;
    Location* loc;
    // TODO (jasmcaus) - Come up with a workaround for this
    enum {
        AstNode__FloatLiteral32,    // default
        AstNode__FloatLiteral64,
        // AstNode__FloatLiteral128 // will be supported later
    } type;
} AstNode__FloatLiteral;

typedef struct {
    Buff* value;
    Location* loc;
    bool is_special;   // format / raw string
    enum {
        AstNode__StringLiteralNone,   // if `is_special` is false
        AstNode__StringLiteralRaw,    // `r"abc"`
        AstNode__StringLiteralFormat  // `f"name = {name}"`
    } type;
} AstNode__StringLiteral;

typedef struct {
    Buff* value;
    Location* loc;
} AstNode__CharLiteral;

typedef struct {
    Buff* value;
    Location* loc;
} AstNode__BoolLiteral;

struct AstNode__ {
    AstNode__Kind type; // type of AST Node
    UInt64 tok_index; // token index

    union {
        AstNode__FuncDecl* func_def;
        AstNode__FuncPrototype* func_proto;
        AstNode__FuncCallExpr* func_call_expr;
        AstNode__ParamDecls* param_decls;
        AstNode__ReturnExpr* return_type;
        AstNode__Defer* defer;
        AstNode__VarDecl* var_decl;
        AstNode__Identifier* identifier;
        AstNode__Block* block;
        AstNode__TestDecl* test_decl;
        AstNode__TestExpr* test_expr;
        AstNode__BinaryOpExpr* binary_op_expr;
        AstNode__PrefixOpExpr* prefix_op_expr;
        AstNode__TryExpr* try_expr;
        AstNode__CatchExpr* catch_expr;
        AstNode__IfBoolExpr* if_bool_expr;
        AstNode__ForExpr* for_expr;
        AstNode__MatchExpr* match_expr;
        AstNode__MatchRange* match_range;
        AstNode__CompileTime* compile_time_expr;
        AstNode__ContainerDecl* container_decl;
        AstNode__BranchStatement* branch_stmt;
        AstNode__IntLiteral* int_literal;
        AstNode__FloatLiteral* float_literal;
        AstNode__StringLiteral* str_literal;
        AstNode__CharLiteral* char_literal;
        AstNode__BoolLiteral* bool_literal;
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
    // TODO (jasmcaus) Change the type of `module` to an `AstNode__Module`
    Buff* module;   // name of the module
    bool is_test;   // true for test_*.ad files
} AstFile;

#endif // ADORAD_AST_H