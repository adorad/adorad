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

// Token constructor
Token* token_init() {
    Token* token = calloc(1, sizeof(Token));
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

// Convert a Token to its respective String representation
Buff* token_to_buff(TokenKind kind) {
    Buff* buf = buff_new(null);
    char* value;

    switch(kind) {
        // Special (internal usage only)
        case TOK_EOF: value = "EOF"; break;
        case TOK_NULL: value = "TOK_NULL"; break;
        case TOK_ILLEGAL: value = "ILLEGAL"; break;
        case COMMENT: value = "COMMENT"; break;
        case DOCS_COMMENT: value = "DOCS_COMMENT"; break;

        // Literals
        case IDENTIFIER: value = "IDENTIFIER"; break;
        case INTEGER: value = "INTEGER"; break;
        case BIN_INT: value = "BIN_INT"; break;
        case HEX_INT: value = "HEX_INT"; break;
        case OCT_INT: value = "OCT_INT"; break;
        case INT8_LIT: value = "INT8_LIT"; break;
        case INT16_LIT: value = "INT16_LIT"; break;
        case INT32_LIT: value = "INT32_LIT"; break;
        case INT64_LIT: value = "INT64_LIT"; break;
        case UINT_LIT: value = "UINT_LIT"; break;
        case UINT8_LIT: value = "UINT8_LIT"; break;
        case UINT16_LIT: value = "UINT16_LIT"; break;
        case UINT32_LIT: value = "UINT32_LIT"; break;
        case UINT64_LIT: value = "UINT64_LIT"; break;
        case TOK_FLOAT: value = "FLOAT"; break;
        case FLOAT32_LIT: value = "FLOAT32_LIT"; break;
        case FLOAT64_LIT: value = "FLOAT64_LIT"; break;
        case FLOAT128_LIT: value = "FLOAT128_LIT"; break;
        case IMAG: value = "IMAG_LIT"; break;
        case RUNE: value = "RUNE_LIT"; break;
        case STRING: value = "STRING_LIT"; break;
        case RAW_STRING: value = "RAW_STRING_LIT"; break;
        case TRIPLE_STRING: value = "TRIPLE_STRING_LIT";  break;
        case TOK_TRUE: value = "TRUE"; break;
        case TOK_FALSE: value = "FALSE"; break;

        // Operators 
        case PLUS: value = "+"; break;
        case MINUS: value = "-"; break;
        case MULT: value = "*"; break;
        case SLASH: value = "/"; break;
        case MOD: value = "%"; break;
        case MOD_MOD: value = "%%"; break;
        case PLUS_PLUS: value = "++"; break;
        case MINUS_MINUS: value = "--"; break;
        case MULT_MULT: value = "**"; break;
        case SLASH_SLASH: value = "//"; break;
        case AT_SIGN: value = "@"; break;
        case HASH_SIGN: value = "#"; break;
        case QUESTION: value = "?"; break;

        // Comparison Operators
        case GREATER_THAN: value = ">"; break;
        case LESS_THAN: value = "<"; break;
        case GREATER_THAN_OR_EQUAL_TO: value = ">="; break;
        case LESS_THAN_OR_EQUAL_TO: value = "<="; break;
        case EQUALS_EQUALS: value = "=="; break;
        case EXCLAMATION_EQUALS: value = "!="; break;

        // Assignment Operators
        case EQUALS: value = "="; break;
        case PLUS_EQUALS: value = "+="; break;
        case MINUS_EQUALS: value = "-="; break;
        case MULT_EQUALS: value = "*="; break;
        case SLASH_EQUALS: value = "/="; break;
        case MOD_EQUALS: value = "%="; break;
        case AND_EQUALS: value = "&="; break;
        case OR_EQUALS: value = "|="; break;
        case XOR_EQUALS: value = "^="; break;
        case LBITSHIFT_EQUALS: value = "<<="; break;
        case RBITSHIFT_EQUALS: value = ">>="; break;
        case TILDA: value = "~"; break;
        case TILDA_EQUALS: value = "~="; break;

        // Arrows
        case EQUALS_ARROW: value = "=>"; break;
        case RARROW: value = "->"; break;
        case LARROW: value = "<-"; break;

        // Delimiters
        case LSQUAREBRACK: value = "["; break;
        case RSQUAREBRACK: value = "]"; break;
        case LBRACE: value = "{"; break;
        case RBRACE: value = "}"; break;
        case LPAREN: value = "("; break;
        case RPAREN: value = ")"; break;

        // Bitwise
        case LBITSHIFT: value = "<<"; break;
        case RBITSHIFT: value = ">>"; break;
        case AND: value = "&"; break;
        case OR: value = "|"; break;
        case EXCLAMATION: value = "!"; break;
        case XOR: value = "^"; break;
        case AND_NOT: value = "&^"; break;
        case AND_AND: value = "&&"; break;
        case OR_OR: value = "||"; break;

        // Separators
        case COLON: value = ":"; break;
        case COLON_COLON: value = "::"; break;
        case SEMICOLON: value = ";"; break;
        case COMMA: value = ":"; break;
        case DOT: value = "."; break;
        case DDOT: value = ".."; break;
        case ELLIPSIS: value = "..."; break;
        case BACKSLASH: value = "\\"; break;

        // Keywords
        case ANY: value = "any"; break;
        case AS: value = "as"; break;
        case ATOMIC: value = "atomic"; break;
        case BEGIN: value = "begin"; break;
        case BREAK: value = "break"; break;
        case CASE: value = "case"; break;
        case CAST: value = "cast"; break;
        case CATCH: value = "catch"; break;
        case CLASS: value = "class"; break;
        case CONST: value = "const"; break;
        case CONTINUE: value = "continue"; break;
        case DO: value = "do"; break;
        case DECL: value = "decl"; break;
        case DEFAULT: value = "default"; break;
        case ENUM: value = "enum"; break;
        case ELSE: value = "else"; break;
        case ELSEIF: value = "elseif"; break;
        case EXCEPT: value = "except"; break;
        case EXPORT: value = "export"; break;
        case EXTERN: value = "inline"; break;
        case FINALLY: value = "finally"; break;
        case FOR: value = "for"; break;
        case FROM: value = "from"; break;
        case FUNC: value = "func"; break;
        case IF: value = "if"; break;
        case IMPORT: value = "import"; break;
        case IN: value = "in"; break;
        case INCLUDE: value = "include"; break;
        case INLINE: value = "inline"; break;
        case ISA: value = "isa";  break;
        case MACRO: value = "macro"; break;
        case MAP: value = "map"; break;
        case MATCH: value = "match"; break;
        case MIXIN: value = "mixin"; break;
        case MODULE: value = "module"; break;
        case MUTABLE: value = "mutable"; break;
        case NO_INLINE: value = "noinline"; break;
        case NOT: value = "not"; break;
        case NOT_IN: value = "notin"; break;
        case PRAGMA: value = "pragma";   break;
        case RAISE: value = "raise"; break;
        case RANGE: value = "range"; break;
        case RETURN: value = "value ="; break;
        case STRUCT: value = "struct"; break;
        case TRY: value = "try"; break;
        case TUPLE: value = "tuple"; break;
        case TYPE: value = "type"; break;
        case TYPEOF: value = "typeof"; break;
        case WHEN: value = "when"; break;
        case WHERE: value = "where"; break;
        case WHILE: value = "while"; break;
        case UNION: value = "union"; break;
        case USE: value = "use"; break;
        // We should _never_ reach here
        default: value = "ILLEGAL"; break;
    }

    buff_set(buf, value);
    return buf;
}