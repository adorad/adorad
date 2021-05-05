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

#ifndef HAZEL_TOKEN
#define HAZEL_TOKEN 

#include <string>
#include <Hazel/Core/HCore.h> 

// token.h defines constants representing the lexical tokens of the Hazel programming language and basic operations on 
// tokens (printing, predicates).

// Set of lexical tokens in the Hazel Programming Language
// 
// NOTE: 
// Any changes made to this function _MUST_ reflect in the toString() (in <Tokens.c>)
// as well as in Syntax.toml (Hazel/Compiler/Syntax/Syntax.toml)
#define ALLTOKENS \
    /* Special (internal usage only) */ \
    TOKENKIND(TOK_ILLEGAL = 0,  ""),   \
    TOKENKIND(TOK_ID = 0,  "TOK_ID"),   \
    TOKENKIND(TOK_EOF, "TOK_EOF"),  \
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

class Token {
public:
    // Default constructor (create a NO_TOKEN instance)
    Token() {
        reset_();
    }

    // Copy constructor 
    Token(const Token& other) {
        this->type = other.type; 
        this->offset = other.offset; 
        this->tok_bytes = other.tok_bytes; 
        this->line_no = other.line_no;
        this->col_no = other.col_no; 
        this->tok_length = other.tok_length; 
        this->fname = other.fname;
        this->value = other.value;
    }

    // Get an illegal token
    Token* illegal_tok() {
        Token* token; 
        token->type = TOK_ILLEGAL; 
        return token;
    }

    // Clone a Token
    Token* clone(Token& other) {
        Token* token; 
        token->type = other.type; 
        token->offset = other.offset; 
        token->tok_bytes = other.tok_bytes; 
        token->line_no = other.line_no;
        token->col_no = other.col_no; 
        token->tok_length = other.tok_length; 
        token->fname = other.fname;
        token->value = other.value;
        return token;
    }

    // Set a token type 
    void set_token_type(TokenType tok_type) {
        this->type = tok_type;
    }

    bool isJumpStatement() {
        // Break (BREAK)
        // Continue (CONTINUE)
        // Return (RETURN)
        return (this->type == BREAK || this->type == CONTINUE || this->type == RETURN); 
    } 

    bool isLoopStatement() {
        // While (WHILE)
        // For (FOR)
        return (this->type == WHILE || this->type == FOR); 
    } 

    bool isFlowStatement() {
        // If 
        // Match 
        return (this->type == IF || this->type == MATCH); 
    } 

    bool isMatchStatement() {
        // Declarations used in match-case 
        return (this->type == MATCH || this->type == CASE || this->type == DEFAULT); 
    } 

    bool isExpressionStatement() {
        // Postfix Operations: isPrimaryExpressionStatement or module (for files)
        // Unary Ops: PLUS, MINUS, EXCLAMATION, NOT
        // RAISE 
        return (isPrimaryExpressionStatement() || this->type == MODULE || this->type == PLUS || 
                this->type == MINUS || this->type == EXCLAMATION || this->type == NOT || this->type == RAISE); 
    } 

    bool isPrimaryExpressionStatement() {
        // Literals (numbers, Strings)
        // Booleans (TRUE, FALSE)
        // IDENTIFIER
        // 'null' 
        // FUNC
        // ILLEGAL
        // '(' expression ')'
        return (this->type == INTEGER || this->type == BIN_INT || this->type == HEX_INT || this->type == IMAG || 
                this->type == FLOAT || this->type == RUNE || this->type == STRING || this->type == IDENTIFIER || 
                this->type == TOK_NULL || this->type == FUNC || this->type == TOK_ILLEGAL || this->type == LPAREN || 
                this->type == RPAREN); 
    }

    bool isDeclStatement() {
        // Variable Declaration (with types + "Any") 
        // Function Declaration (FUNC)
        // Class/Struct Declaration (CLASS and STRUCT)
        // Enum Declaration (ENUM)
        // Module Declaration (MODULE)
        // Empty Declaration (SEMICOLON)
        return (this->type == ANY || this->type == FUNC || this->type == CLASS || this->type == STRUCT || 
                this->type == ENUM || this->type == MODULE || this->type == SEMICOLON); 
    } 


    bool isSpecial() {
        return (this->type == TOK_ID || this->type == TOK_EOF || this->type == TOK_ILLEGAL || this->type == COMMENT); 
    }

    bool isLiteral() {
        return this->type > TOK___LITERALS_BEGIN && this->type < TOK___LITERALS_END; 
    }

    bool isKeyword() {
        return this->type > TOK___KEYWORDS_BEGIN && this->type < TOK___KEYWORDS_END; 
    }

    bool isOperator() {
        return this->type > TOK___OPERATORS_BEGIN && this->type < TOK___OPERATORS_END; 
    }

    bool isComparisonOperator() {
        return this->type > TOK___COMP_OPERATORS_BEGIN && this->type < TOK___COMP_OPERATORS_END; 
    }

    bool isAssignmentOperator() {
        return this->type > TOK___ASSIGNMENT_OPERATORS_BEGIN && this->type < TOK___ASSIGNMENT_OPERATORS_END; 
    }

    bool isDelimiter() {
        return this->type > TOK___DELIMITERS_OPERATORS_BEGIN && this->type < TOK___DELIMITERS_OPERATORS_END;
    }

    bool isArrow() {
        return this->type > TOK___ARROW_OPERATORS_BEGIN && this->type < TOK___ARROW_OPERATORS_END;
    }

    bool isBitwise() {
        return this->type > TOK___BITWISE_OPERATORS_BEGIN && this->type < TOK___BITWISE_OPERATORS_END;
    }

    bool isSeparator() {
        return this->type > TOK___SEPARATORS_BEGIN && this->type < TOK___SEPARATORS_END;
    }

    bool isIdentifier() {
        return this->type == IDENTIFIER; 
    }

    bool isEOF() {
        return this->type == TOK_EOF; 
    }

    bool isNULL() {
        return this->type == TOK_NULL; 
    }

    bool isIllegal() {
        return this->type == TOK_ILLEGAL; 
    }

    bool isMacro() {
        return this->type == MACRO; 
    }

    bool isImport() {
        return this->type == IMPORT; 
    }

    bool isInclude() {
        return this->type == INCLUDE; 
    }

    bool isSemiColon() {
        return this->type == SEMICOLON; 
    }

    // Convert a Token to its respective String representation
    std::string toString() {
        switch(this->type) {
            // Special (internal usage only)
            case TOK_EOF: return "TOK_EOF";
            case TOK_NULL: return "TOK_NULL";
            case TOK_ILLEGAL: return "ILLEGAL";
            case COMMENT: return "COMMENT";

            // Literals
            case IDENTIFIER: return "IDENTIFIER";
            case INTEGER: return "INTEGER";
            case BIN_INT: return "BIN_INT";
            case HEX_INT: return "HEX_INT";        
            case INT8_LIT: return "INT8_LIT";       
            case INT16_LIT: return "INT16_LIT";      
            case INT32_LIT: return "INT32_LIT";      
            case INT64_LIT: return "INT64_LIT";      
            case UINT_LIT: return "UINT_LIT";       
            case UINT8_LIT: return "UINT8_LIT";      
            case UINT16_LIT: return "UINT16_LIT";     
            case UINT32_LIT: return "UINT32_LIT";     
            case UINT64_LIT: return "UINT64_LIT";     
            case FLOAT: return "FLOAT";          
            case FLOAT32_LIT: return "FLOAT32_LIT";    
            case FLOAT64_LIT: return "FLOAT64_LIT";    
            case FLOAT128_LIT: return "FLOAT128_LIT";  
            case IMAG: return "IMAG";         
            case RUNE: return "RUNE";         
            case STRING: return "STRING";       
            case RAW_STRING: return "RAW_STRING";   
            case TRIPLE_STRING: return "TRIPLE_STRING"; 
            case TRUE: return "TRUE";
            case FALSE: return "FALSE";

            // Operators 
            case PLUS: return "+";
            case MINUS: return "-";
            case MULT: return "*";
            case SLASH: return "/";
            case MOD: return "%";
            case MOD_MOD: return "%%";
            case PLUS_PLUS: return "++";
            case MINUS_MINUS: return "--";
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
            case SEMICOLON: return ";";  
            case COMMA: return ":";  
            case DOT: return ".";  
            case DDOT: return ".."; 
            case ELLIPSIS: return "...";
            case BACKSLASH: return "\\"; 

            // Keywords
            case ANY: return "any";      
            case AS: return "as";      
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
            case EXTERN: return "extern";  
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
            // We should _never_reach here
            default: return "ILLEGAL";
        }
    }

    // Reset the Token
    void reset_() {
        this->type = TOK_ILLEGAL; 
        this->offset = 0; 
        this->tok_bytes = 0; 
        this->line_no = 0;
        this->col_no = 0; 
        this->tok_length = 0; 
        this->fname = "";
        this->value = "";
    }

protected:
    TokenType type;     // Token Type
    UInt32 offset;      // Offset of the first character of the Token
    UInt32 tok_bytes;   // Token length (in bytes)
    UInt32 line_no;     // Token Line Number (1-Based)
    UInt32 col_no;      // Token Column Number (0-Based) 
    UInt32 tok_length;  // Token length (UTF-8)
    std::string fname;  // the file name
    std::string value;  // Token value
    friend class Lexer;
}; // class Token

#endif // HAZEL_TOKEN