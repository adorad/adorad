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

#include <Hazel/Core/Misc.h>
#include <Hazel/Core/Types.h> 


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
    TOKENKIND(TOK_ID,  "TOK_ID"),   \
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
    /* FLOAT conflicts with a typedef in <windows.h> */ \
    TOKENKIND(TOK_FLOAT,     "FLOAT"),        \
    TOKENKIND(FLOAT32_LIT,  "FLOAT32_LIT"),   \
    TOKENKIND(FLOAT64_LIT,  "FLOAT64_LIT"),   \
    TOKENKIND(FLOAT128_LIT, "FLOAT128_LIT"),  \
    TOKENKIND(IMAG,          "IMAG"),         \
    TOKENKIND(RUNE,          "RUNE"),         \
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
    TOKENKIND(COLON,     ":"),   \
    TOKENKIND(SEMICOLON, ";"),   \
    TOKENKIND(COMMA,     ","),   \
    TOKENKIND(DOT,       "."),   \
    TOKENKIND(DDOT,      ".."),  \
    TOKENKIND(ELLIPSIS,  "..."), \
    TOKENKIND(BACKSLASH, "\\"),  \
TOKENKIND(TOK___SEPARATORS_END, ""), \
\
    /* Keywords */                    \
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
    TOKENKIND(WHEN,      "when"),     \
    TOKENKIND(WHERE,     "where"),    \
    TOKENKIND(WHILE,     "while"),    \
    TOKENKIND(UNION,     "union"),    \
    TOKENKIND(USE,       "use"), /* aliasing purposes */ \
TOKENKIND(TOK___KEYWORDS_END, ""),    \
\
    TOKENKIND(TOK_COUNT, "")


typedef enum {
    #define TOKENKIND(tok, str)     tok
        ALLTOKENS
    #undef TOKENKIND
} TokenKind; 

/*
    Main Token Struct 
*/
typedef struct {
    TokenKind type;     // Token Type
    UInt32 offset;      // Offset of the first character of the Token
    UInt32 tok_bytes;   // Token length (in bytes)
    UInt32 tok_length;  // Token length (UTF-8)
    const char* value;  // Token value
    UInt32 lineno;     // the line number in the source where the token occured
    UInt32 colno;      // the column number
    const char* fname; // the file name
} Token;

// Create a basic (ILLEGAL) token
Token* token_init(void);
// Get the token value 
const char* value(Token token);
// Reset a Token instance
void token_reset_token(Token* token);

// Returns the token corresponding to a single character
static TokenKind token_onechar(char c);
// Returns the token corresponding to two characters
static TokenKind token_twochar(char c1, char c2);
// Returns the token corresponding to three characters
static TokenKind token_threechar(char c1, char c2, char c3);

const char* token_toString(Token* token);
static inline bool token_isJumpStatement(Token* token);
static inline bool token_isLoopStatement(Token* token);
static inline bool token_isFlowStatement(Token* token);
static inline bool token_isMatchStatement(Token* token);
static inline bool token_isExpressionStatement(Token* token);
static inline bool token_isPrimaryExpressionStatement(Token* token);
static inline bool token_isDeclStatement(Token* token);
static inline bool token_isSpecial(Token* token);
static inline bool token_isLiteral(Token* token);
static inline bool token_isKeyword(Token* token);
static inline bool token_isOperator(Token* token);
static inline bool token_isComparisonOperator(Token* token);
static inline bool token_isAssignmentOperator(Token* token);
static inline bool token_isDelimiter(Token* token);
static inline bool token_isArrow(Token* token);
static inline bool token_isBitwise(Token* token); 
static inline bool token_isSeparator(Token* token);
static inline bool token_isIdentifier(Token* token);
static inline bool token_isEOF(Token* token);
static inline bool token_isNULL(Token* token);
static inline bool token_isIllegal(Token* token);
static inline bool token_isMacro(Token* token);
static inline bool token_isImport(Token* token);
static inline bool token_isInclude(Token* token);
static inline bool token_isSemiColon(Token* token);

#endif // _HAZEL_TOKEN_H
