/*
_ _    _           ______   _______        
| |  | |    /\    /___  /   |  ____|| |    
| |__| |   /  \      / /    | |__   | |       Hazel - The Fast, Expressive & Elegant Programming Language
|  __  |  / /\ \    / /     |  __|  | |       Languages: C, C++, and Assembly
| |  | | / ____ \  / /___   | |____ | |____   https://github.com/HazelLang/Hazel/
|_|_ |_|/_/    \_\/_______\ |______|_\______|

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/

#ifndef _HAZEL_TOKEN_H
#define _HAZEL_TOKEN_H

#include <stdlib.h>

#include <Hazel/Core/Types.h> 
#include <Hazel/Compiler/Lexer/Location.h>


// Tokens.h defines constants representing the lexical tokens of the Hazel programming language and basic operations on 
// tokens (printing, predicates).

// Set of lexical tokens understood by the Compiler
// 
// NOTE: 
// Any changes made to this function _MUST_ reflect in the toString() (in <Tokens.c>)
// as well as in Syntax.toml (Hazel/Compiler/Syntax/Syntax.toml)
#define ALLTOKENS \
    /* Special (internal usage only) */ \
    TOKENKIND(TOK_ILLEGAL = 0,  ""),   \
    TOKENKIND(TOK_ID = 0,  "TOK_ID"),   \
    TOKENKIND(TOK_EOF, "TOK_EOF"),  /* End of Input */ \
    TOKENKIND(TOK_NULL,"TOK_NULL"), \
    TOKENKIND(COMMENT, "COMMENT"),  \
\
    /* Literals */ \
TOKENKIND(TOK___LITERALS_BEGIN, ""), \
    TOKENKIND(IDENTIFIER,   "IDENTIFIER"),    \
    TOKENKIND(INTEGER,      "INTEGER"),       \
    TOKENKIND(BIN_INT,      "BIN_INT"),       \
    TOKENKIND(HEX_INT,      "HEX_INT"),       \
    TOKENKIND(INT8_LIT,     "INT8_LIT"),      \
    TOKENKIND(INT16_LIT,    "INT16_LIT"),     \
    TOKENKIND(INT32_LIT,    "INT32_LIT"),     \
    TOKENKIND(INT64_LIT,    "INT64_LIT"),     \
    TOKENKIND(UINT_LIT,     "UINT_LIT"),      \
    TOKENKIND(UINT8_LIT,    "UINT8_LIT"),     \
    TOKENKIND(UINT16_LIT,   "UINT16_LIT"),    \
    TOKENKIND(UINT32_LIT,   "UINT32_LIT"),    \
    TOKENKIND(UINT64_LIT,   "UINT64_LIT"),    \
    TOKENKIND(FLOAT,        "FLOAT"),         \
    TOKENKIND(FLOAT32_LIT,  "FLOAT32_LIT"),   \
    TOKENKIND(FLOAT64_LIT,  "FLOAT64_LIT"),   \
    TOKENKIND(FLOAT128_LIT, "FLOAT128_LIT"),  \
    TOKENKIND(IMAG,          "IMAG"),         \
    TOKENKIND(RUNE,          "RUNE"),         \
    TOKENKIND(STRING,        "STRING"),       \
    TOKENKIND(RAW_STRING,    "RAW_STRING"),   \
    TOKENKIND(TRIPLE_STRING, "TRIPLE_STRING"), \
    TOKENKIND(TRUE,          "TRUE"),         \
    TOKENKIND(FALSE,         "FALSE"),        \
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
    TOKENKIND(COLON,     ":"),   \
    TOKENKIND(SEMICOLON, ";"),   \
    TOKENKIND(COMMA,     ","),   \
    TOKENKIND(DOT,       "."),   \
    TOKENKIND(DDOT,      ".."),  \
    TOKENKIND(ELLIPSIS,  "..."), \
    TOKENKIND(BACKSLASH, "\\"),  \
TOKENKIND(TOK___SEPARATORS_END, ""), \
\
    /* Keywords */ \
TOKENKIND(TOK___KEYWORDS_BEGIN, ""),  \
    TOKENKIND(KEYWORD,    ""), /* Token Classification*/ \
    TOKENKIND(ANY,       "any"),      \
    TOKENKIND(AS,        "as"),       \
    TOKENKIND(BEGIN,     "begin"),    \
    TOKENKIND(BREAK,     "break"),    \
    TOKENKIND(CASE,      "case"),     \
    TOKENKIND(CAST,      "cast"),     \
    TOKENKIND(CATCH,     "catch"),    \
    TOKENKIND(CLASS,     "class"),    \
    TOKENKIND(CONST,     "const"),    \
    TOKENKIND(CONTINUE,  "continue"), \
    TOKENKIND(DO,        "do"),       \
    /* TOKENKIND(DEF, "def"), */      \
    TOKENKIND(DECL,      "decl"),  \
    TOKENKIND(DEFAULT,   "default"),  \
    TOKENKIND(ENUM,      "enum"),     \
    TOKENKIND(ELSE,      "else"),     \
    TOKENKIND(ELSEIF,    "elseif"),   \
    TOKENKIND(EXCEPT,    "except"),   \
    TOKENKIND(EXPORT,    "export"),   \
    TOKENKIND(EXTERN,    "extern"),   \
    TOKENKIND(FINALLY,   "finally"),  \
    TOKENKIND(FOR,       "for"),      \
    TOKENKIND(FROM,      "from"),     \
    TOKENKIND(FUNC,      "func"),     \
    /* GLOBAL is not required tbh coz we follow a no-globals approach */ \
    /* TOKENKIND(GLOBAL, "global"), */\
    TOKENKIND(IF,        "if"),       \
    TOKENKIND(IMPORT,    "import"),   \
    TOKENKIND(IN,        "in"),       \
    TOKENKIND(INCLUDE,   "include"),  \
    TOKENKIND(INLINE,    "inline"),   \
    TOKENKIND(ISA,       "isa"),      \
    TOKENKIND(MACRO,     "macro"),    \
    TOKENKIND(MAP,       "map"),      \
    TOKENKIND(MATCH,     "match"), /* similar to 'switch' in C++, Java and others */ \
    TOKENKIND(MIXIN,     "mixin"),    \
    TOKENKIND(MODULE,    "module"),   \
    TOKENKIND(MUTABLE,   "mutable"),  \
    /* TOKENKIND(NEW, "new"), */ /* if we go for a memory-safe approach like Rust */ \
    TOKENKIND(NO_INLINE, "noinline"), \
    TOKENKIND(NOT,       "not"),      \
    TOKENKIND(NOT_IN,    "notin"),    \
    TOKENKIND(RAISE,     "raise"),    \
    TOKENKIND(RANGE,     "range"),    \
    TOKENKIND(RETURN,    "return"),   \
    TOKENKIND(STRUCT,    "struct"),   \
    TOKENKIND(TRY,       "try"),      \
    TOKENKIND(TUPLE,     "tuple"),    \
    TOKENKIND(TYPE,      "type"),     \
    TOKENKIND(TYPEOF,    "typeof"),   \
    TOKENKIND(USE,       "use"), /* aliasing purposes */ \
    TOKENKIND(WHEN,      "when"),     \
    TOKENKIND(WHERE,     "where"),    \
    TOKENKIND(WHILE,     "while"),    \
    TOKENKIND(UNION,     "union"),    \
TOKENKIND(TOK___KEYWORDS_END, ""), \
\
    TOKENKIND(TOK_COUNT, "")


typedef enum {
    #define TOKENKIND(tok, str)     tok
        ALLTOKENS
    #undef TOKENKIND
} TokenType; 

/*
    Main Token Struct 
*/
typedef struct {
    TokenType type__;     // Token Type
    UInt32 offset__;      // Offset of the first character of the Token
    UInt32 tok_bytes__;   // Token length (in bytes)
    UInt32 tok_length__;  // Token length (UTF-8)
    const char* value__;  // Token value
    Location location__;  // Location of the source file
} Token;

// Create a basic (ILLEGAL) token
Token* token_init() {
    Token* token = calloc(1, sizeof(Token));
    token->type__ = TOK_ILLEGAL; 
    token->offset__ = 0; 
    token->tok_bytes__ = 0; 
    token->tok_length__ = 0; 
    token->value__ = "";
    token_location_init(token);

    return token;
}

void token_location_init(Token* token) {
    token->location__.lineno__ = 0; 
    token->location__.colno__ = 0; 
    token->location__.fname__ = "";
}

Token* token_from_tok_type(const TokenType __tok_type) {
    Token* token = token_init();
    token->type__ = __tok_type;
    return token;
}

Token* token_make_illegal_tok() {
    return token_init();
}

Token* token_make_eof_tok() {
    Token* token = token_init();
    token->type__ = TOK_EOF;
    return token;
}

// Clone a Token
Token* token_clone(Token* other) {
    Token* token = token_init();
    token->type__ = other->type__; 
    token->offset__ = other->offset__; 
    token->tok_bytes__ = other->tok_bytes__; 
    token->tok_length__ = other->tok_length__; 
    token->value__ = other->value__;
    token->location__ = other->location__;
    return token;
}

// Get token type 
TokenType token_get_tok_type(Token* token) { return token->type__; }
// Returns the location of the token
Location token_location(Token* token) { return token->location__; }
// Get the token value 
const char* value(Token* token) { return token->value__; }


const char* token_toString(Token* token);
inline bool token_isJumpStatement(Token* token);
inline bool token_isLoopStatement(Token* token);
inline bool token_isFlowStatement(Token* token);
inline bool token_isMatchStatement(Token* token);
inline bool token_isExpressionStatement(Token* token);
inline bool token_isPrimaryExpressionStatement(Token* token);
inline bool token_isDeclStatement(Token* token);
inline bool token_isSpecial(Token* token);
inline bool token_isLiteral(Token* token);
inline bool token_isKeyword(Token* token);
inline bool token_isOperator(Token* token);
inline bool token_isComparisonOperator(Token* token);
inline bool token_isAssignmentOperator(Token* token);
inline bool token_isDelimiter(Token* token);
inline bool token_isArrow(Token* token);
inline bool token_isBitwise(Token* token);
inline bool token_isSeparator(Token* token);
inline bool token_isIdentifier(Token* token);
inline bool token_isEOF(Token* token);
inline bool token_isNULL(Token* token);
inline bool token_isIllegal(Token* token);
inline bool token_isMacro(Token* token);
inline bool token_isImport(Token* token);
inline bool token_isInclude(Token* token);
inline bool token_isSemiColon(Token* token);


#endif // _HAZEL_TOKEN_H