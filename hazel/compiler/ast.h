/*
_ _    _           ______   _______        
| |  | |    /\    /___  /   |  ____|| |    
| |__| |   /  \      / /    | |__   | |       Hazel - The Fast, Expressive * Elegant Programming Language
|  __  |  / /\ \    / /     |  __|  | |       Languages: C, C++, and Assembly
| |  | | / ____ \  / /___   | |____ | |____   https://github.com/HazelLang/hazel/
|_|_ |_|/_/    \_\/_______\ |______|_\______|

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/

#ifndef HAZEL_AST_H
#define HAZEL_AST_H

#include <hazel/core/types.h>
#include <hazel/core/vector.h>

typedef struct AstNode AstNode;

typedef enum AstNodeType {

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
    bool is_var_args;    // variable arguments used?
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

typedef struct AstNodeReturnType {
    enum ReturnKind kind;
} AstNodeReturnType;

struct AstNode {
    AstNodeType type; // type of AST Node
    UInt64 tok_index; // token index

    union {
        AstNodeFuncDef* func_def;
        AstNodeParamDecls* param_decls;
        AstNodeReturnType* return_type;
    } data;
};


#endif // HAZEL_AST_H