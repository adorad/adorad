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

#include <stdlib.h>
#include <adorad/compiler/parser.h>

// Shortcut to `parser->toklist`
#define pt  parser->toklist

// Initialize a new Parser
Parser* parser_init(Lexer* lexer) {
    Parser* parser = cast(Parser*)calloc(1, sizeof(Parser));
    parser->lexer = lexer;
    parser->toklist = lexer->toklist;
    parser->curr_tok = cast(Token*)vec_at(parser->toklist, 0);
    parser->tok_count = vec_size(parser->toklist);
    return parser;
}

inline Token* parser_peek_token(Parser* parser) {
    return parser->curr_tok;
}

// Consumes a token and moves on to the next token
inline Token* parser_chomp(Parser* parser) {
    Token* tok = parser_peek_token(parser);
    parser->curr_tok += 1;
    return tok;
}

// Consumes a token and moves on to the next, if the current token matches the expected token.
inline Token* parser_chomp_if(Parser* parser, TokenKind tokenkind) {
    if(parser->curr_tok->kind == tokenkind)
        return parser_chomp(parser);

    return null;
}

inline Token* parser_expect_token(Parser* parser, TokenKind tokenkind) {
    if(parser->curr_tok->kind == tokenkind)
        return parser_chomp(parser);
        
    adorad_panic(ErrorUnexpectedToken, "Expected `%s`; got `%s`", 
                                        token_to_buff(tokenkind)->data,
                                        token_to_buff(parser->curr_tok->kind)->data);
    abort();
}

AstNode* ast_create_node(AstNodeKind kind) {
    AstNode* node = cast(AstNode*)calloc(1, sizeof(AstNode));
    node->kind = kind;
    return node;
}

AstNode* ast_clone_node(AstNode* node) {
    if(!node)
        adorad_panic(ErrorUnexpectedNull, "Trying to clone a null AstNode?");
    AstNode* new = ast_create_node(node->kind);
    // TODO(jasmcaus): Add more struct members
    return new;
}