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

#include <adorad/compiler/parser.h>

// Initialize a new Parser
static Parser* parser_init() {

}

static inline TokenKind parser_peek_token(Parser* parser) {
      return parser->curr_tok;
}

// Consumes a token and moves on to the next token
static inline TokenKind parser_chomp(Parser* parser) {
      TokenKind tok = parser_peek_token(parser);
      parser->curr_tok += 1;
      return tok;
}

// Consumes a token and moves on to the next, if the current token matches the expected token.
static inline TokenKind parser_chomp_if(Parser* parser, TokenKind tokenkind) {
      if(parser->curr_tok == tokenkind) {
            return parser_chomp(parser);
      } else {
            cstlColouredPrintf(CSTL_COLOUR_ERROR, "Unexpected token `%s` found. Expected `%s`", 
                                                  token_to_string(parser->curr_tok),
                                                  token_to_string(tokenkind));
            exit(1);
      }
      return -1;
}