/*
          _____   ____  _____            _____
    /\   |  __ \ / __ \|  __ \     /\   |  __ \
   /  \  | |  | | |  | | |__) |   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /   / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \  / ____ \| |__| | https://github.com/adorad/adorad/
/_/    \_\_____/ \____/|_|  \_\/_/    \_\_____/

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021-22 Jason Dsouza <@jasmcaus>
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

};

typedef enum {
    VisibilityModePrivate, // default
    VisibilityModePublic   // if `export` is used
} VisibilityMode;

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

// `if x < y   { ... }`
//     ^----^  ^------^
//   condition  if_body
typedef struct {
    AstNode* condition;   // `x < y`
    AstNode* if_body;     // `{...}`
    AstNode* else_node;   // `null` if `has_else = false`
    bool is_comptime;
    bool is_expr;
    bool has_else;
} AstNodeIfExpr;


#endif // ADORAD_AST_H