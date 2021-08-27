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

#ifndef ADORAD_PARSER_H
#define ADORAD_PARSER_H

#include <adorad/compiler/ast.h>
#include <adorad/compiler/lexer.h>
#include <adorad/compiler/tokens.h>

// Each Adorad source file can be represented as a `Parser` structure.
// This means if there are `n` source files, there will be `n` Parser instances (one for each file).
typedef struct Parser {
    UInt32 id;
    Buff* fullpath;    // path/to/file.ad
    Buff* basename;    // file.ad
    Lexer* lexer;
    Vec* toklist;       // shortcut to `lexer->toklist`
    Token* curr_tok;
    UInt64 tok_count;
} Parser;

Parser* parser_init(Lexer* lexer);
AstNode* ast_create_node(AstNodeKind type);

#endif // ADORAD_PARSER_H