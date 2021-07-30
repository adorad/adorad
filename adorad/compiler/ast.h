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
    bool is_mutable;   // This is false unless explicitly mentioned
} AstNodeFuncPrototype;

typedef struct AstNodeParamDecls {
    char* name;
    AstNode* type;
    bool is_alias;
    bool is_var_args;
} AstNodeParamDecls;

// From Zig. Not sure if this will suit our purposes
enum ReturnKind {
    ReturnKindUnconditional,
    ReturnKindError
};

typedef struct AstNodeReturnExpr {
    enum ReturnKind kind;
    AstNode* expr;
} AstNodeReturnExpr;

typedef struct AstNodeDefer {
    enum ReturnKind kind;
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

struct AstNode {
    AstNodeKind type; // type of AST Node
    UInt64 tok_index; // token index

    union {
        AstNodeFuncDef* func_def;
        AstNodeParamDecls* param_decls;
        AstNodeReturnExpr* return_type;
        AstNodeDefer* defer;
        AstNodeVarDecl* var_decl;
        AstNodeIdentifier* identifier;
    } data;
};

#endif // ADORAD_AST_H