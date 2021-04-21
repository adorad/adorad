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
    TOKENKIND(PLUS,      "+"), \
    TOKENKIND(MINUS,     "-"), \
    TOKENKIND(MULT,      "*"), \
    TOKENKIND(QUOTIENT,  "/"), \
    TOKENKIND(MOD,       "%"), \
    TOKENKIND(MOD_MOD,   "%%"), \
    TOKENKIND(AND,       "&"), \
    TOKENKIND(OR,        "|"), \
    TOKENKIND(NOT,       "!"), \
    TOKENKIND(XOR,       "^"), \
    TOKENKIND(AND_NOT,   "&^"), \
    TOKENKIND(AND_AND,   "&&"), \
    TOKENKIND(OR_OR,     "||"), \
    TOKENKIND(INCREMENT, "++"), \
    TOKENKIND(DECREMENT, "--"), \
    TOKENKIND(AT_SIGN,   "@"), \
    TOKENKIND(HASH_SIGN, "#"), \
    TOKENKIND(QUESTION,  "?"), \
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

// Use a custom type here: 
// Like a Dict{TokenNames, String} mapping
// Useful only when a bootstrapped compiler is available
// Implementing a Dictionary in C is easy, but it has the potential to deride focus from Hazel which is not worth it.
// Something similar to Go's implementation: 
// https://github.com/golang/go/blob/964639cc338db650ccadeafb7424bc8ebb2c0f6c/src/go/token/token.go
// Dict{TokenNames, String} {

// nbytes in AllTokens = 24
// struct AllTokens {
//     Token tok; // nbytes = 16
//     const char* key;
// };

// #define num_all_tokens (int)sizeof(all_tokens)/sizeof(AllTokens)
// static const struct AllTokens all_tokens[] = {
//     // Special Tokens
//     { TOK_EOF    , "EOF" },
//     { COMMENT    , "COMMENT" },
//     { IDENTIFIER , "IDENTIFIER" },
//     { AT_SIGN    , "@" },
//     { COMMA      , "," },
//     { SEMICOLON  , ";" },

//     // Keywords
//     { BEGIN     , "begin" },
//     { BREAK     , "break" },
//     { CASE      , "case" },
//     { CATCH     , "catch" },
//     { CONST     , "const" },
//     { CONTINUE  , "continue" },
//     { DO        , "do" },
//     { DEF       , "def" },
//     { DEFAULT   , "default" },
//     { ELSE      , "else" },
//     { ELSEIF    , "elseif" },
//     { EXPORT    , "export" },
//     { FINALLY   , "finally" },
//     { FOR       , "for" },

//     // GLOBAL    , "global"
//     { IF        , "if" },
//     { IMPORT    , "import" },
//     { INCLUDE   , "include" },
//     { MACRO     , "macro" },
//     { MATCH     , "match" },
//     { MODULE    , "module" },
//     { MUTABLE   , "mutable" },
//     // NEW       , "new"
//     { RANGE     , "range" },
//     { RETURN    , "return" },
//     { RUNE      , "rune" },
//     { STRUCT    , "struct" },
//     { TRY       , "try" },
//     { TYPE      , "type" },
//     { USE       , "use" },
//     { WHERE     , "where" },
//     { WHILE     , "while" },

//     // Literals
//     { INTEGER , "INTEGER" },
//     { FLOAT   , "FLOAT" },
//     { STRING  , "STRING" },
//     { IMAG    , "IMAG" },
//     { CHAR    , "CHAR" },
//     { TRUE    , "TRUE" },
//     { FALSE   , "FALSE" },

//     // Operators 
//     { ADD            , "+" },
//     { SUBTRACT       , "-" },
//     { MUTLIPLICATION , "*" },
//     { QUOTIENT       , "/" },
//     { REM            , "%" },

//     { AND       , "&" },
//     { OR        , "|" },
//     { NOT       , "!" },
//     { XOR       , "^" },
//     { AND_NOT   , "&^" },
//     { LAND      , "&&" },
//     { LOR       , "||" },
//     { INCREMENT , "++" },
//     { DECREMENT , "--" },

//     // Comparison
//     { GREATER                  , ">"  },
//     { LESS                     , "<" },
//     { GREATER_THAN             , ">=" },
//     { GREATER_THAN_OR_EQUAL_TO , "≥" },
//     { LESS_THAN                , "<=" },
//     { LESS_THAN_OR_EQUAL_TO    , "≤" },
//     { DOUBLEEQUALS             , "=="  },
//     { NOT_EQUALS               , "!="  },
//     { NOT_EQUAL_TO             , "≠" },
//     { IN                       , "in" },
//     { ISA                      , "isa" },

//     // Delimiters
//     { LBRACK  , "[" },
//     { RBRACK  , "]" },
//     { LBRACE  , "{" },
//     { RBRACE  , "}" },
//     { LPAREN  , "(" },
//     { RPAREN  , ")" },

//     // Assignments
//     { EQUALS           , "="  },
//     { PLUS_EQUALS      , "+=" },
//     { MINUS_EQUALS     , "-=" },
//     { START_EQUALS     , "*=" },
//     { OR_EQUALS        , "|=" },
//     { DIVISION_EQUALS  , "÷=" },
//     { FWD_SLASH_EQUALS , "/=" },
//     { REM_EQUALS       , "%=" },
//     { LBITSHIFT_EQUALS , "<<=" },
//     { RBITSHIFT_EQUALS , ">>=" },
//     { AND_EQUALS       , "&=" },
//     { APPROX           , "~" },

//     // Colons
//     { COLON    , ":" },
//     { DOT      , "." },
//     { DDOT     , ".." },
//     { ELLIPSIS , "..." },

//     // Bitshifts
//     { LBITSHIFT , "<<" },
//     { RBITSHIFT , ">>" },

//     // Conditional },
//     { CONDITIONAL , "?" },

//     // Arrows },
//     { PAIR_ARROWS                , "=>" },
//     { ANON_FUNC                  , "->" },
//     { LEFT_ARROW                 , "<-" },
//     { LEFTWARDS_ARROW            , "←" },
//     { RIGHTWARDS_ARROW           , "→" },
//     { HALFWIDTH_LEFTWARDS_ARROW  , "￩" },
//     { HALFWIDTH_RIGHTWARDS_ARROW , "￫" },
//     { LONG_LEFTWARDS_ARROW       , "⟵" },
//     { LONG_RIGHTWARDS_ARROW      , "⟶ " },

//      // Declaration (we might/might not use this) },
//     { DECLARATION , "::" }
    
// }; 

// #define num_all_special_tokens (int)sizeof(all_special_tokens)/sizeof(AllTokens)
// static const struct AllTokens all_special_tokens[] = {
//     // Special Tokens
//     { TOK_EOF    , "EOF" },
//     { COMMENT    , "COMMENT" },
//     { IDENTIFIER , "IDENTIFIER" },
//     { AT_SIGN    , "@" },
//     { COMMA      , "," },
//     { SEMICOLON  , ";" }
// }; 

// #define num_all_keywords (int)sizeof(all_keywords)/sizeof(AllTokens)
// static const struct AllTokens all_keywords[] = {
//     // Keywords
//     { BEGIN     , "begin" },
//     { BREAK     , "break" },
//     { CASE      , "case" },
//     { CATCH     , "catch" },
//     { CONST     , "const" },
//     { CONTINUE  , "continue" },
//     { DO        , "do" },
//     { DEF       , "def" },
//     { DEFAULT   , "default" },
//     { ELSE      , "else" },
//     { ELSEIF    , "elseif" },
//     { EXPORT    , "export" },
//     { FINALLY   , "finally" },
//     { FOR       , "for" },
//     // GLOBAL    , "global"
//     { IF        , "if" },
//     { IMPORT    , "import" },
//     { INCLUDE   , "include" },
//     { MACRO     , "macro" },
//     { MATCH     , "match" },
//     { MODULE    , "module" },
//     { MUTABLE   , "mutable" },
//     // NEW       , "new"
//     { RANGE     , "range" },
//     { RETURN    , "return" },
//     { RUNE      , "rune" },
//     { STRUCT    , "struct" },
//     { TRY       , "try" },
//     { TYPE      , "type" },
//     { USE       , "use" },
//     { WHERE     , "where" },
//     { WHILE     , "while" }
// }; 

// #define num_all_literals (int)sizeof(all_literals)/sizeof(AllTokens)
// static const struct AllTokens all_literals[] = {
//     // Literals
//     { INTEGER , "INTEGER" },
//     { FLOAT   , "FLOAT" },
//     { STRING  , "STRING" },
//     { IMAG    , "IMAG" },
//     { CHAR    , "CHAR" },
//     { TRUE    , "TRUE" },
//     { FALSE   , "FALSE" }
// }; 

// #define num_all_operators (int)sizeof(all_operators)/sizeof(AllTokens)
// static const struct AllTokens all_operators[] = {
//     // Operators 
//     { ADD            , "+" },
//     { SUBTRACT       , "-" },
//     { MUTLIPLICATION , "*" },
//     { QUOTIENT       , "/" },
//     { REM            , "%" },

//     { AND       , "&" },
//     { OR        , "|" },
//     { NOT       , "!" },
//     { XOR       , "^" },
//     { AND_NOT   , "&^" },
//     { LAND      , "&&" },
//     { LOR       , "||" },
//     { INCREMENT , "++" },
//     { DECREMENT , "--" },

//     // Comparison
//     { GREATER                  , ">"  },
//     { LESS                     , "<" },
//     { GREATER_THAN             , ">=" },
//     { GREATER_THAN_OR_EQUAL_TO , "≥" },
//     { LESS_THAN                , "<=" },
//     { LESS_THAN_OR_EQUAL_TO    , "≤" },
//     { DOUBLEEQUALS             , "=="  },
//     { NOT_EQUALS               , "!="  },
//     { NOT_EQUAL_TO             , "≠" },
//     { IN                       , "in" },
//     { ISA                      , "isa" }
// }; 

// #define num_all_delimiters (int)sizeof(all_delimiters)/sizeof(AllTokens)
// static const struct AllTokens all_delimiters[] = {
//     // Delimiters
//     { LBRACK  , "[" },
//     { RBRACK  , "]" },
//     { LBRACE  , "{" },
//     { RBRACE  , "}" },
//     { LPAREN  , "(" },
//     { RPAREN  , ")" }
// }; 

// #define num_all_assignment_ops (int)sizeof(all_assignment_ops)/sizeof(AllTokens)
// static const struct AllTokens all_assignment_ops[] = {
//     // Assignments
//     { EQUALS           , "="  },
//     { PLUS_EQUALS      , "+=" },
//     { MINUS_EQUALS     , "-=" },
//     { START_EQUALS     , "*=" },
//     { OR_EQUALS        , "|=" },
//     { DIVISION_EQUALS  , "÷=" },
//     { FWD_SLASH_EQUALS , "/=" },
//     { REM_EQUALS       , "%=" },
//     { LBITSHIFT_EQUALS , "<<=" },
//     { RBITSHIFT_EQUALS , ">>=" },
//     { AND_EQUALS       , "&=" },
//     { APPROX           , "~" }
// }; 

// #define num_all_colons (int)sizeof(all_colons)/sizeof(AllTokens) 
// static const struct AllTokens all_colons[] = {
//     // Colons
//     { COLON    , ":" },
//     { DOT      , "." },
//     { DDOT     , ".." },
//     { ELLIPSIS , "..." },
// }; 

// #define num_all_arrows (int)sizeof(all_arrows)/sizeof(AllTokens)
// static const struct AllTokens all_arrows[] = {
//     // Arrows },
//     { PAIR_ARROWS                , "=>" },
//     { ANON_FUNC                  , "->" },
//     { LEFT_ARROW                 , "<-" },
//     // { LEFTWARDS_ARROW            , "←" },
//     // { RIGHTWARDS_ARROW           , "→" },
//     // { HALFWIDTH_LEFTWARDS_ARROW  , "￩" },
//     // { HALFWIDTH_RIGHTWARDS_ARROW , "￫" },
//     // { LONG_LEFTWARDS_ARROW       , "⟵" },
//     // { LONG_RIGHTWARDS_ARROW      , "⟶ " },
// }; 


// Returns the string corresponding to the Token tok. 
// For operators, delimiters and keywords, the string is the actual token character sequence (e.g. for the token ADD,
// the string is "+"). For all other tokens, the string corresponds to the token constant name (e.g. for the token
// IN, the string is "IN")
// const char* get_tok_string(Token tok) {
//     char c = nullchar;

//     if(tok>=0 && tok > Token(enumsize)) {
//         c = tokens[tok];
//     }
//     if c == nullchar {
//         c = "token(" + tokens.strconv(int(tok)) + ")";
//     }
//     return c;
// }


#endif // HAZEL_TOKEN 