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

#ifndef ADORAD_TOKEN_H
#define ADORAD_TOKEN_H

#include <adorad/core/misc.h>
#include <adorad/core/types.h> 
#include <adorad/compiler/location.h>

/*
    `tokens.h` defines constants representing the lexical tokens of the Adorad programming language and basic operations
    on tokens (printing, predicates).

    NOTE: 
    Any changes made to this function _MUST_ be reflect in the token_to_buff() (in <adorad/compiler/tokens.c>)
    as well as in Syntax.toml (adorad/compiler/syntax/syntax.toml)
*/
#define ALLTOKENS \
    /* Special (internal usage only) */      \
    TOKENKIND(TOK_NULL = 0, ""),             \
    TOKENKIND(TOK_ILLEGAL,  ""),             \
    TOKENKIND(TOK_EOF,      "TOK_EOF"),  /* End of Input */ \
    TOKENKIND(COMMENT,      "COMMENT"),      \
    TOKENKIND(DOCS_COMMENT, "DOCS_COMMENT"), \
    TOKENKIND(UNREACHABLE,  "unreachable"),  \
    TOKENKIND(BUILTIN,      "builtin"),      \
\
    /* Literals */ \
TOKENKIND(TOK___LITERALS_BEGIN, ""), \
    TOKENKIND(IDENTIFIER,   "IDENTIFIER"),    \
    TOKENKIND(INTEGER,      "INTEGER"),       \
    TOKENKIND(BIN_INT,      "BIN_INT"),       \
    TOKENKIND(HEX_INT,      "HEX_INT"),       \
    TOKENKIND(OCT_INT,      "OCT_INT"),       \
    TOKENKIND(UINT_LIT,     "UINT_LIT"),      \
    /* FLOAT conflicts with a typedef in <windows.h> */ \
    TOKENKIND(FLOAT_LIT,     "FLOAT"),        \
    TOKENKIND(IMAG,          "IMAG"),         \
    TOKENKIND(RUNE,          "RUNE"),         \
    TOKENKIND(CHAR_LIT,       "CHAR"),         \
    TOKENKIND(STRING,        "STRING"),       \
    TOKENKIND(RAW_STRING,    "RAW_STRING"),   \
    TOKENKIND(TRIPLE_STRING, "TRIPLE_STRING"), \
    TOKENKIND(TOK_TRUE,      "TRUE"),         \
    TOKENKIND(TOK_FALSE,     "FALSE"),        \
TOKENKIND(TOK___LITERALS_END, ""), \
\
    /* Operators */ \
TOKENKIND(TOK___OPERATORS_BEGIN, ""), \
    TOKENKIND(OPERATOR,    ""), /* Token Classification*/ \
    TOKENKIND(PLUS,        "+"),  \
    TOKENKIND(MINUS,       "-"),  \
    TOKENKIND(MULT,        "*"),  \
    TOKENKIND(SLASH,       "/"),  \
    TOKENKIND(MOD,         "%"),  \
    TOKENKIND(MOD_MOD,     "%%"), \
    TOKENKIND(PLUS_PLUS,   "++"), \
    TOKENKIND(MINUS_MINUS, "--"), \
    TOKENKIND(MULT_MULT,   "**"), \
    TOKENKIND(SLASH_SLASH, "//"), \
    TOKENKIND(AT_SIGN,     "@"),  \
    TOKENKIND(HASH_SIGN,   "#"),  \
    TOKENKIND(QUESTION,    "?"),  \
\
    /* Comparison Operators */ \
TOKENKIND(TOK___COMP_OPERATORS_BEGIN, ""), \
    TOKENKIND(GREATER_THAN,             ">"),  \
    TOKENKIND(LESS_THAN,                "<"),  \
    TOKENKIND(GREATER_THAN_OR_EQUAL_TO, ">="), \
    TOKENKIND(LESS_THAN_OR_EQUAL_TO,    "<="), \
    TOKENKIND(EQUALS_EQUALS,            "=="), \
    TOKENKIND(EXCLAMATION_EQUALS,       "!="), \
TOKENKIND(TOK___COMP_OPERATORS_END, ""), \
\
    /* Assignment Operators */ \
TOKENKIND(TOK___ASSIGNMENT_OPERATORS_BEGIN, ""), \
    TOKENKIND(EQUALS,           "="),   \
    TOKENKIND(PLUS_EQUALS,      "+="),  \
    TOKENKIND(MINUS_EQUALS,     "-="),  \
    TOKENKIND(MULT_EQUALS,      "*="),  \
    TOKENKIND(SLASH_EQUALS,     "/="),  \
    TOKENKIND(MOD_EQUALS,       "%="),  \
    TOKENKIND(AND_EQUALS,       "&="),  \
    TOKENKIND(OR_EQUALS,        "|="),  \
    TOKENKIND(XOR_EQUALS,       "^="),  \
    TOKENKIND(LBITSHIFT_EQUALS, "<<="), \
    TOKENKIND(RBITSHIFT_EQUALS, ">>="), \
    TOKENKIND(TILDA,            "~"),   \
    TOKENKIND(TILDA_EQUALS,     "~="),  \
TOKENKIND(TOK___ASSIGNMENT_OPERATORS_END, ""), \
\
    /* Arrows */ \
TOKENKIND(TOK___ARROW_OPERATORS_BEGIN, ""), \
    TOKENKIND(EQUALS_ARROW, "=>"), \
    TOKENKIND(RARROW,       "->"), \
    TOKENKIND(LARROW,       "<-"), \
TOKENKIND(TOK___ARROW_OPERATORS_END, ""), \
\
    /* Delimiters */ \
TOKENKIND(TOK___DELIMITERS_OPERATORS_BEGIN, ""), \
    TOKENKIND(LSQUAREBRACK, "["), \
    TOKENKIND(RSQUAREBRACK, "]"), \
    TOKENKIND(LBRACE,       "{"), \
    TOKENKIND(RBRACE,       "}"), \
    TOKENKIND(LPAREN,       "("), \
    TOKENKIND(RPAREN,       ")"), \
TOKENKIND(TOK___DELIMITERS_OPERATORS_END, ""), \
\
    /* Bitwise Operators */ \
TOKENKIND(TOK___BITWISE_OPERATORS_BEGIN, ""), \
    TOKENKIND(LBITSHIFT,   "<<"), \
    TOKENKIND(RBITSHIFT,   ">>"), \
    TOKENKIND(AND,         "&"),  \
    TOKENKIND(OR,          "|"),  \
    TOKENKIND(EXCLAMATION, "!"),  \
    TOKENKIND(XOR,         "^"),  \
    TOKENKIND(AND_NOT,     "&^"), \
    TOKENKIND(AND_AND,     "&&"), \
    TOKENKIND(OR_OR,       "||"), \
TOKENKIND(TOK___BITWISE_OPERATORS_END, ""), \
TOKENKIND(TOK___OPERATORS_END, ""), \
\
    /* Separators */ \
TOKENKIND(TOK___SEPARATORS_BEGIN, ""), \
    TOKENKIND(COLON,       ":"),   \
    TOKENKIND(COLON_COLON, "::"),  \
    TOKENKIND(SEMICOLON,   ";"),   \
    TOKENKIND(COMMA,       ","),   \
    TOKENKIND(DOT,         "."),   \
    TOKENKIND(DDOT,        ".."),  \
    TOKENKIND(ELLIPSIS,    "..."), \
    TOKENKIND(BACKSLASH,   "\\"),  \
TOKENKIND(TOK___SEPARATORS_END, ""), \
\
    /* Attributes */ \
TOKENKIND(TOK___ATTRIBUTES_BEGIN, ""),        \
    TOKENKIND(ATTR_COMPTIME, "[comptime]"),   \
    TOKENKIND(ATTR_INLINE,   "[inline]"),     \
    TOKENKIND(ATTR_NOINLINE, "[noinline]"),   \
    TOKENKIND(ATTR_NORETURN, "[noreturn]"),   \
    TOKENKIND(ATTR_LIKELY,   "[likely]"),     \
    TOKENKIND(ATTR_UNLIKELY, "[unlikely]"),   \
TOKENKIND(TOK___ATTRIBUTES_END, ""),          \
\
    /* Keywords */ \
TOKENKIND(TOK___KEYWORDS_BEGIN, ""),  \
    TOKENKIND(KEYWORD,       ""), /* Token Classification*/ \
    TOKENKIND(AS,            "as"),       \
    TOKENKIND(ALIAS,         "alias"),    \
    TOKENKIND(BREAK,         "break"),    \
    TOKENKIND(CONST,         "const"),    \
    TOKENKIND(CONTINUE,      "continue"), \
    TOKENKIND(DEFAULT,       "default"),  \
    TOKENKIND(DEFER,         "defer"),    \
    TOKENKIND(ENUM,          "enum"),     \
    TOKENKIND(ELSE,          "else"),     \
    TOKENKIND(ELSEIF,        "elseif"),   \
    TOKENKIND(EXPORT,        "export"),   \
    TOKENKIND(FALLTHROUGH,   "fallthrough"),  \
    TOKENKIND(FROM,          "from"),     \
    TOKENKIND(FUNC,          "func"),     \
    TOKENKIND(GLOBAL,        "global"),   \
    TOKENKIND(IF,            "if"),       \
    TOKENKIND(IN,            "in"),       \
    TOKENKIND(LOOP,          "loop"),     \
    TOKENKIND(MACRO,         "macro"),    \
    TOKENKIND(MATCH,         "match"),    \
    TOKENKIND(MODULE,        "module"),   \
    TOKENKIND(MUTABLE,       "mutable"),  \
    TOKENKIND(NOT,           "not"),      \
    TOKENKIND(ORELSE,        "orelse"),   \
    TOKENKIND(PUT,           "put"),      \
    TOKENKIND(RAISE,         "raise"),    \
    TOKENKIND(RANGE,         "range"),    \
    TOKENKIND(RETURN,        "return"),   \
    TOKENKIND(STRUCT,        "struct"),   \
    TOKENKIND(TRY,           "try"),      \
    TOKENKIND(TYPEOF,        "typeof"),   \
    TOKENKIND(WHEN,          "when"),     \
    TOKENKIND(WHERE,         "where"),    \
    TOKENKIND(UNION,         "union"),    \
    TOKENKIND(USE,           "use"),      \
TOKENKIND(TOK___KEYWORDS_END, ""),    \
\
    TOKENKIND(TOK_COUNT, "")

typedef enum TokenKind {
    #define TOKENKIND(kind, str)     kind
        ALLTOKENS
    #undef TOKENKIND
} TokenKind;

extern const char* tokenHash[TOK_COUNT + 1];

// Main Token Struct 
typedef struct Token {
    TokenKind kind;     // Token Kind
    UInt32 offset;      // Offset of the first character of the Token
    Buff* value;        // Token value
    Loc* loc;      // Location of the token in the source code
} Token;

// Create a basic (ILLEGAL) token
Token* token_init();
// Reset a Token instance
void token_reset_token(Token* token);
// Convert a Token to its respective String representation
Buff* token_to_buff(TokenKind kind);
// Is an attribute?
bool token_is_attribute(TokenKind kind);

#endif // ADORAD_TOKEN_H