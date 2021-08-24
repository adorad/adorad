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



#endif // ADORAD_AST_H