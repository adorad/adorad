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
#include <adorad/compiler/tokens.h>

typedef struct Parser {
    TokenKind curr_tok;
} Parser;

static Parser* parser_init();
static AstNode* ast_create_node(AstNodeKind type);

static inline TokenKind parser_peek_token(Parser* parser);

// Consumes a token and moves on to the next token
static inline TokenKind parser_chomp(Parser* parser);

// Consumes a token and moves on to the next, if the current token matches the expected token.
static inline TokenKind parser_chomp_if(Parser* parser, TokenKind tokenkind);

#endif // ADORAD_PARSER_H