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

#ifndef _HAZEL_TOKEN
#define _HAZEL_TOKEN 

#include <string>
#include <iostream>

#include <Hazel/Core/Types.h> 
#include <Hazel/Compiler/Lexer/Location.h>

namespace Hazel {

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
    Main Token Class 
*/
class Token {
public:
    // Default constructor (create a NO_TOKEN instance)
    Token() {
        reset_();
    }

    // Make a token from a TokenType
    Token(const TokenType __tok_type) {
        reset_();
        this->type = __tok_type;
    }

    // Copy constructor 
    Token(const Token& other) {
        this->type = other.type; 
        this->offset = other.offset; 
        this->tok_bytes = other.tok_bytes; 
        this->tok_length = other.tok_length; 
        this->__location = other.__location;
        this->__value = other.__value;
    }

    // Make a token for an invalid value
    Token make_illegal_tok() {
        Token token; 
        token.type = TOK_ILLEGAL; 
        return token;
    }

    // Make a token representing the end of file
    Token make_eof_tok() {
        Token token; 
        token.type = TOK_EOF; 
        return token;
    }

    // Clone a Token
    Token clone(Token& other) {
        Token token; 
        token.type = other.type; 
        token.offset = other.offset; 
        token.tok_bytes = other.tok_bytes; 
        token.tok_length = other.tok_length; 
        token.__value = other.__value;
        token.__location = other.__location;
        return token;
    }

    // Set a token type 
    void set_tok_type(TokenType tok_type) { this->type = tok_type; }
    // Get token type 
    TokenType tok_type() { return this->type; }
    // Returns the location of the token
    Location location() { return this->__location; }
    // Get the token value 
    std::string value() { return this->__value; }

    // Print the Token for debugging
    void print() {
        std::cout << "DEBUG! CURRENT TOKEN  = \"" << toString() << "\"\n";
    }

    // Reset the Token
    void reset_() {
        this->type = TOK_ILLEGAL; 
        this->offset = 0; 
        this->tok_bytes = 0; 
        this->tok_length = 0; 
        this->__value = "";
        this->__location.reset_();
    }

    Token& operator=(const Token&);
    std::string toString();
    bool isJumpStatement();
    bool isLoopStatement();
    bool isFlowStatement();
    bool isMatchStatement();
    bool isExpressionStatement();
    bool isPrimaryExpressionStatement();
    bool isDeclStatement();
    bool isSpecial() ;
    bool isLiteral();
    bool isKeyword();
    bool isOperator();
    bool isComparisonOperator();
    bool isAssignmentOperator();
    bool isDelimiter();
    bool isArrow();
    bool isBitwise();
    bool isSeparator();
    bool isIdentifier();
    bool isEOF();
    bool isNULL();
    bool isIllegal();
    bool isMacro();
    bool isImport();
    bool isInclude();
    bool isSemiColon();

public:
    TokenType type;     // Token Type
    UInt32 offset;      // Offset of the first character of the Token
    UInt32 tok_bytes;   // Token length (in bytes)
    UInt32 tok_length;  // Token length (UTF-8)
    std::string __value;  // Token value
    Location __location;  // Location of the source file
    friend class Lexer;
}; // class Token

} // namespace Hazel 

#endif // _HAZEL_TOKEN