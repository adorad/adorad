/*
          _____         _____           _____  
    /\   |  __ \ / __ \|  __ \    /\   |  __ \
   /  \  | |  | | |  | | |__)|   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /  / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \ / ____ \| |__| | https://github.com/adorad/adorad/
/_/_ __\_\_____/_\____/|_|__\_/_/_ __\_\_____/

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/

#ifndef HAZEL_AST_H
#define HAZEL_AST_H

#include <adorad/core/types.h>
#include <adorad/core/vector.h>

typedef struct AstNode AstNode;

// Change values later
typedef enum AstNodeType {
    AST_NODE_HELLO,
    AST_NODE_HELLO2,
} AstNodeType;

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
    cstlVector* params;  // vector of `AstNode*`s -> similar to cstlVector<AstNode*> if C has generics
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

struct AstNode {
    AstNodeType type; // type of AST Node
    UInt64 tok_index; // token index

    union {
        AstNodeFuncDef* func_def;
        AstNodeParamDecls* param_decls;
        AstNodeReturnExpr* return_type;
        AstNodeDefer* defer;
        AstNodeVarDecl* var_decl;
    } data;
};

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

#endif // HAZEL_AST_H