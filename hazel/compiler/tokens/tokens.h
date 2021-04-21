#ifndef HAZEL_TOKEN
#define HAZEL_TOKEN 

#include <stdlib.h> 
#include <hazel/core/hcore.h> 

// token.h defines constants representing the lexical tokens of the Hazel programming language and basic operations on 
// tokens (printing, predicates).

// Set of lexical tokens in the Hazel Programming Language
// 
// NOTE: 
// Any changes made to this function _MUST_ reflect in the token_toString() in <tokens.c> as well 
#define ALLTOKENS \ 
    /* Special (internal usage only) */ \
    TOKENKIND(TOK_ID,  "TOK_ID"), \
    TOKENKIND(TOK_EOF, "TOK_EOF"), \
    TOKENKIND(TOK_NULL,"TOK_NULL"), \
    TOKENKIND(ILLEGAL, "ILLEGAL"), \
    TOKENKIND(COMMENT, "COMMENT"), \
\
    /* Literals */ \
    TOKENKIND(TOK___LITERALS_BEGIN, ""), \
    TOKENKIND(IDENTIFIER, "IDENTIFIER"), \
    TOKENKIND(INTEGER,    "INTEGER"), \
    TOKENKIND(BIN_INT,    "BIN_INT"), \
    TOKENKIND(HEX_INT,    "HEX_INT"), \
    TOKENKIND(IMAG,       "IMAG"), \
    TOKENKIND(FLOAT,      "FLOAT"), \
    TOKENKIND(RUNE,       "RUNE"), \
    TOKENKIND(STRING,     "STRING"), \
    /*TOKENKIND(TRIPLE_STRING, "TRIPLE_STRING"), \*/ \
    TOKENKIND(TRUE,       "TRUE"), \
    TOKENKIND(FALSE,      "FALSE"), \
    TOKENKIND(TOK___LITERALS_END, ""), \
\
    /* Operators */ \ 
    TOKENKIND(TOK___OPERATORS_BEGIN, ""), \
    TOKENKIND(PLUS,        "+"), \
    TOKENKIND(MINUS,       "-"), \
    TOKENKIND(MULT,        "*"), \
    TOKENKIND(QUOTIENT,    "/"), \
    TOKENKIND(MOD,         "%"), \
    TOKENKIND(MOD_MOD,     "%%"), \
    TOKENKIND(AND,         "&"), \
    TOKENKIND(OR,          "|"), \
    TOKENKIND(EXCLAMATION, "!"), \
    TOKENKIND(XOR,         "^"), \
    TOKENKIND(AND_NOT,     "&^"), \
    TOKENKIND(AND_AND,     "&&"), \
    TOKENKIND(OR_OR,       "||"), \
    TOKENKIND(INCREMENT,   "++"), \
    TOKENKIND(DECREMENT,   "--"), \
    TOKENKIND(AT_SIGN,     "@"), \
    TOKENKIND(HASH_SIGN,   "#"), \
    TOKENKIND(QUESTION,    "?"), \
\   
    /* Comparison Operators */ \
TOKENKIND(TOK___COMP_OPERATORS_BEGIN, ""), \
    TOKENKIND(GREATER_THAN,             ">"), \
    TOKENKIND(LESS_THAN,                "<"), \
    TOKENKIND(GREATER_THAN_OR_EQUAL_TO, ">="), \
    TOKENKIND(LESS_THAN_OR_EQUAL_TO,    "<="), \
    TOKENKIND(EQUALS_EQUALS,            "=="), \
    TOKENKIND(NOT_EQUALS,               "!="), \
TOKENKIND(TOK___COMP_OPERATORS_END, ""), \
\
    /* Assignment Operators */ \
TOKENKIND(TOK___ASSIGNMENT_OPERATORS_BEGIN, ""), \
    TOKENKIND(EQUALS,           "="), \
    TOKENKIND(PLUS_EQUALS,      "+="), \
    TOKENKIND(MINUS_EQUALS,     "-="), \
    TOKENKIND(STAR_EQUALS,      "*="), \
    TOKENKIND(DIVISION_EQUALS,  "/="), \
    TOKENKIND(MOD_EQUALS,       "%="), \
    TOKENKIND(AND_EQUALS,       "&="), \
    TOKENKIND(OR_EQUALS,        "|="), \
    TOKENKIND(XOR_EQUALS,       "^="), \
    TOKENKIND(LBITSHIFT_EQUALS, "<<="), \
    TOKENKIND(RBITSHIFT_EQUALS, ">>="), \
    TOKENKIND(TILDA,            "~"), \
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
    /* Colons */ \
TOKENKIND(TOK___COLONS_OPERATORS_BEGIN, ""), \
    TOKENKIND(COLON,     ":"),   \
    TOKENKIND(SEMICOLON, ";"),   \
    TOKENKIND(COMMA,     ","),   \
    TOKENKIND(DOT,       "."),   \
    TOKENKIND(DDOT,      ".."),  \
    TOKENKIND(ELLIPSIS,  "..."), \
    TOKENKIND(BACKSLASH, "\\"),  \
TOKENKIND(TOK___COLONS_OPERATORS_END, ""), \
\   
    /* Bitshifts */ \
TOKENKIND(TOK___BITSHIFT_OPERATORS_BEGIN, ""), \
    TOKENKIND(LBITSHIFT, "<<"), \
    TOKENKIND(RBITSHIFT, ">>"), \
TOKENKIND(TOK___BITSHIFT_OPERATORS_END, ""), \
TOKENKIND(TOK___OPERATORS_END, ""), \
\
    /* Keywords */ \
TOKENKIND(TOK___KEYWORDS_BEGIN, ""), \
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
    TOKENKIND(DEFAULT,   "default"),  \
    TOKENKIND(ENUM,      "enum"),     \
    TOKENKIND(ELSE,      "else"),     \
    TOKENKIND(ELSEIF,    "elseif"),   \
    TOKENKIND(EXPORT,    "export"),   \
    TOKENKIND(FINALLY,   "finally"),  \
    TOKENKIND(FOR,       "for"),      \
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
    TOKENKIND(MODULE,    "module"),   \
    TOKENKIND(MUTABLE,   "mutable"),  \
    /* TOKENKIND(NEW, "new"), */ /* if we go for a memory-safe approach like Rust */ \
    TOKENKIND(NO_INLINE, "no_inline"),\
    TOKENKIND(NOT,       "not"),      \
    TOKENKIND(NOT_IN,    "not_in"),   \
    TOKENKIND(RAISE,     "raise"),    \
    TOKENKIND(RANGE,     "range"),    \
    TOKENKIND(RETURN,    "return"),   \
    TOKENKIND(STRUCT,    "struct"),   \
    TOKENKIND(TRY,       "try"),      \
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
    #define TOKENKIND(tok, str) tok
        ALLTOKENS
    #undef TOKENKIND
} AllTokensEnum; 


typedef struct TokenNames {
    AllTokensEnum type; // Token Type
    UInt32 line;        // Token Line Number (1-Based)
    UInt32 column;      // Token Column Number (0-Based) 
    UInt32 offset;      // Offset of the first character of the Token
    UInt32 bytes;       // Token length (in bytes)
    UInt32 tok_length;  // Token length (UTF-8)
    UInt32 tok_fileid;  // Token file ID
    const char* value;  // Token value (_not_ null-terminated)
} Token; 

#define NO_TOKEN            (Token){0,0,0,0,0,0,0,0,null}
#define ILLEGAL_TOKEN       (Token){ILLEGAL,0,0,0,0,0,0,0,null}
#define TOKEN_BYTES(_tok)   _tok.bytes
#define TOKEN_VALUE(_tok)   _tok.value


static char* const TokenStrings[] = {
    #define TOKENKIND(e, s)     cast(UInt8*)s, cstl_sizeof(s)-1
        ALLTOKENS
    #undef TOKENKIND
};


Token* token_init(int type, char* value);
Token* token_clone(Token* token);
void token_free(Token* token);

const char* token_toString(AllTokensEnum token);

inline bool token_isSpecial(AllTokensEnum token); 
inline bool token_isLiteral(AllTokensEnum token); 
inline bool token_isKeyword(AllTokensEnum token); 
inline bool token_isOperator(AllTokensEnum token); 
inline bool token_isComparisonOperator(AllTokensEnum token); 
inline bool token_isAssignmentOperator(AllTokensEnum token); 
inline bool token_isDelimiter(AllTokensEnum token); 
inline bool token_isArrow(AllTokensEnum token); 
inline bool token_isBitshift(AllTokensEnum token); 
inline bool token_isColon(AllTokensEnum token); 

inline bool token_isIdentifier(AllTokensEnum token); 
inline bool token_isEOF(AllTokensEnum token); 
inline bool token_isNULL(AllTokensEnum token);
inline bool token_isIllegal(AllTokensEnum token); 
inline bool token_isMacro(AllTokensEnum token); 
inline bool token_isImport(AllTokensEnum token); 
inline bool token_isInclude(AllTokensEnum token); 
inline bool token_isSemiColon(AllTokensEnum token); 

inline bool token_isDeclStatement(AllTokensEnum token); 
inline bool token_isJumpStatement(AllTokensEnum token); 
inline bool token_isLoopStatement(AllTokensEnum token); 
inline bool token_isFlowStatement(AllTokensEnum token); 
inline bool token_isMatchStatement(AllTokensEnum token); 
inline bool token_isExpressionStatement(AllTokensEnum token); 


#endif // HAZEL_TOKEN 