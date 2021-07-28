/*
          _____         _____           _____  
    /\   |  __ \ / __ \|  __ \    /\   |  __ \
   /  \  | |  | | |  | | |__)|   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /  / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \ / ____ \| |__| | https://github.com/adorad/adorad/
/_/_ __\_\_____/_\____/|_|__\_/_/_ __\_\_____/

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <@jasmcaus>
*/

#include <adorad/compiler/tokens.h>

// Token constructor
Token* token_init() {
    Token* token = calloc(1, sizeof(Token));
    token->kind = TOK_ILLEGAL;
    token->offset = 0;
    token->value = "";
    token->start = 0;
    token->lineno = 1;
    token->colno = 1;
    token->fname = "";

    return token;
}

// Reset a Token instance
void token_reset_token(Token* token) {
    token->kind = TOK_ILLEGAL; 
    token->offset = 0; 
    token->value = "";
    token->lineno = 0; 
    token->colno = 0; 
    token->fname = "";
}

// Convert a Token to its respective String representation
char* token_to_string(TokenKind kind) {
    switch(kind) {
        // Special (internal usage only)
        case TOK_EOF: return "EOF";
        case TOK_NULL: return "TOK_NULL";
        case TOK_ILLEGAL: return "ILLEGAL";
        case COMMENT: return "COMMENT";
        case DOCS_COMMENT: return "DOCS_COMMENT";

        // Literals
        case IDENTIFIER: return "IDENTIFIER";
        case INTEGER: return "INTEGER";
        case BIN_INT: return "BIN_INT";
        case HEX_INT: return "HEX_INT";
        case OCT_INT: return "OCT_INT";
        case INT8_LIT: return "INT8_LIT";
        case INT16_LIT: return "INT16_LIT";
        case INT32_LIT: return "INT32_LIT";
        case INT64_LIT: return "INT64_LIT";
        case UINT_LIT: return "UINT_LIT";
        case UINT8_LIT: return "UINT8_LIT";
        case UINT16_LIT: return "UINT16_LIT";
        case UINT32_LIT: return "UINT32_LIT";
        case UINT64_LIT: return "UINT64_LIT";
        case TOK_FLOAT: return "FLOAT";
        case FLOAT32_LIT: return "FLOAT32_LIT";
        case FLOAT64_LIT: return "FLOAT64_LIT";
        case FLOAT128_LIT: return "FLOAT128_LIT";
        case IMAG: return "IMAG";
        case RUNE: return "RUNE";
        case STRING: return "STRING";
        case RAW_STRING: return "RAW_STRING";
        case TRIPLE_STRING: return "TRIPLE_STRING"; 
        case TOK_TRUE: return "TRUE";
        case TOK_FALSE: return "FALSE";

        // Operators 
        case PLUS: return "+";
        case MINUS: return "-";
        case MULT: return "*";
        case SLASH: return "/";
        case MOD: return "%";
        case MOD_MOD: return "%%";
        case PLUS_PLUS: return "++";
        case MINUS_MINUS: return "--";
        case MULT_MULT: return "**";
        case SLASH_SLASH: return "//";
        case AT_SIGN: return "@";
        case HASH_SIGN: return "#";
        case QUESTION: return "?";

        // Comparison Operators
        case GREATER_THAN: return ">";
        case LESS_THAN: return "<";
        case GREATER_THAN_OR_EQUAL_TO: return ">=";
        case LESS_THAN_OR_EQUAL_TO: return "<=";
        case EQUALS_EQUALS: return "==";
        case EXCLAMATION_EQUALS: return "!=";

        // Assignment Operators
        case EQUALS: return "=";
        case PLUS_EQUALS: return "+=";
        case MINUS_EQUALS: return "-=";
        case MULT_EQUALS: return "*=";
        case SLASH_EQUALS: return "/=";
        case MOD_EQUALS: return "%=";
        case AND_EQUALS: return "&=";
        case OR_EQUALS: return "|=";
        case XOR_EQUALS: return "^=";
        case LBITSHIFT_EQUALS: return "<<=";
        case RBITSHIFT_EQUALS: return ">>=";
        case TILDA: return "~";
        case TILDA_EQUALS: return "~=";

        // Arrows
        case EQUALS_ARROW: return "=>";
        case RARROW: return "->";
        case LARROW: return "<-";

        // Delimiters
        case LSQUAREBRACK: return "[";
        case RSQUAREBRACK: return "]";
        case LBRACE: return "{";
        case RBRACE: return "}";
        case LPAREN: return "(";
        case RPAREN: return ")";

        // Bitwise
        case LBITSHIFT: return "<<";
        case RBITSHIFT: return ">>";
        case AND: return "&";
        case OR: return "|";
        case EXCLAMATION: return "!";
        case XOR: return "^";
        case AND_NOT: return "&^";
        case AND_AND: return "&&";
        case OR_OR: return "||";

        // Separators
        case COLON: return ":";
        case COLON_COLON: return "::";
        case SEMICOLON: return ";";
        case COMMA: return ":";
        case DOT: return ".";
        case DDOT: return "..";
        case ELLIPSIS: return "...";
        case BACKSLASH: return "\\";

        // Keywords
        case ANY: return "any";
        case AS: return "as";
        case ATOMIC: return "atomic";
        case BEGIN: return "begin";
        case BREAK: return "break";
        case CASE: return "case";
        case CAST: return "cast";
        case CATCH: return "catch";
        case CLASS: return "class";
        case CONST: return "const";
        case CONTINUE: return "continue";
        case DO: return "do";
        case DECL: return "decl";
        case DEFAULT: return "default";
        case ENUM: return "enum";
        case ELSE: return "else";
        case ELSEIF: return "elseif";
        case EXCEPT: return "except";
        case EXPORT: return "export";
        case EXTERN: return "inline";
        case FINALLY: return "finally";
        case FOR: return "for";
        case FROM: return "from";
        case FUNC: return "func";
        case IF: return "if";
        case IMPORT: return "import";
        case IN: return "in";
        case INCLUDE: return "include";
        case INLINE: return "inline";
        case ISA: return "isa"; 
        case MACRO: return "macro";
        case MAP: return "map";
        case MATCH: return "match";
        case MIXIN: return "mixin";
        case MODULE: return "module";
        case MUTABLE: return "mutable";
        case NO_INLINE: return "noinline";
        case NOT: return "not";
        case NOT_IN: return "notin";
        case PRAGMA: return "pragma";  
        case RAISE: return "raise";
        case RANGE: return "range";
        case RETURN: return "return";
        case STRUCT: return "struct";
        case TRY: return "try";
        case TUPLE: return "tuple";
        case TYPE: return "type";
        case TYPEOF: return "typeof";
        case WHEN: return "when";
        case WHERE: return "where";
        case WHILE: return "while";
        case UNION: return "union";
        case USE: return "use";
        // We should _never_ reach here
        default: return "ILLEGAL";
    }
}
