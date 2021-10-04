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
#include <adorad/compiler/tokens.h>

const char* tokenHash[TOK_COUNT] = {
    #define TOKENKIND(kind, str)    str
        ALLTOKENS
    #undef TOKENKIND
};

// Token constructor
Token* token_init() {
    Token* token = cast(Token*)calloc(1, sizeof(Token));
    token->kind = TOK_ILLEGAL;
    token->offset = 0;
    token->value = buff_new(null);
    token->loc = loc_new(null);

    return token;
}

// Reset a Token instance
void token_reset_token(Token* token) {
    token->kind = TOK_ILLEGAL; 
    token->offset = 0; 
    buff_set(token->value, "");
    loc_reset(token->loc);
}

// Convert a Token to its respective string representation
Buff* token_to_buff(TokenKind kind) {
    Buff* buf = buff_new(tokenHash[kind]);
    return buf;
}

// Is `kind` an attribute?
bool token_is_attribute(TokenKind kind) {
    return kind > TOK___ATTRIBUTES_BEGIN and kind < TOK___ATTRIBUTES_END;
}