#include <stdlib.h> 

#include <Hazel/Compiler/Tokens/Tokens.h>

Token* token_init(int type, char* value) {
    Token* token = calloc(1, sizeof(Token));
    token->type = type; 
    token->value = calloc(strlen(value)+1, sizeof(char)); 
    strcpy(token->value, value);

    return token;
}    

Token* token_clone(Token* token) {
    return token_init(token->type, token->value); 
}

void token_destroy(Token* token) {
    free(token->value); // Important because we allocate separately for `token->value`
    free(token);
}

// char* token_to_string(Token* token) {
//     char* str = calloc(strlen(token->value)+1, sizeof(char));
//     strcpy(str, token->value);

//     return str;
// }

// NOTE: 
// Any changes made to this function _MUST_ reflect in the ALLTOKENS macro in <tokens.h> as well 
// as well as in Syntax.toml (Hazel/Compiler/Syntax/Syntax.toml)
const char* token_toString(TokenType token) {
    switch(token) {
        // Special (internal usage only)
        case TOK_EOF: return "TOK_EOF";
        case TOK_NULL: return "TOK_NULL";
        case ILLEGAL: return "ILLEGAL";
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
        case QUOTIENT: return "/";
        case MOD: return "%";
        case MOD_MOD: return "%%";
        case INCREMENT: return "++";
        case DECREMENT: return "--";
        case AT_SIGN: return "@";
        case HASH_SIGN: return "#";
        case QUESTION: return "?";

        // Comparison Operators
        case GREATER_THAN: return ">";
        case LESS_THAN: return "<";
        case GREATER_THAN_OR_EQUAL_TO: return ">=";
        case LESS_THAN_OR_EQUAL_TO: return "<=";
        case EQUALS_EQUALS: return "==";
        case NOT_EQUALS: return "!=";

        // Assignment Operators
        case EQUALS: return "=";
        case PLUS_EQUALS: return "+=";
        case MINUS_EQUALS: return "-=";
        case STAR_EQUALS: return "*=";
        case DIVISION_EQUALS: return "/=";
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
    }

    // We should _NEVER_ reach here 
    return "UNRECOGNIZED TOKEN"; 
}


static inline bool token_isJumpStatement(TokenType token) {
    // Break (BREAK)
    // Continue (CONTINUE)
    // Return (RETURN)
    return (token == BREAK || token == CONTINUE || token == RETURN); 
} 

static inline bool token_isLoopStatement(TokenType token) {
    // While (WHILE)
    // For (FOR)
    return (token == WHILE || token == FOR); 
} 

static inline bool token_isFlowStatement(TokenType token) {
    // If 
    // Match 
    return (token == IF || token == MATCH); 
} 

static inline bool token_isMatchStatement(TokenType token) {
    // Declarations used in match-case 
    return (token == MATCH || token == CASE || token == DEFAULT); 
} 

static inline bool token_isExpressionStatement(TokenType token) {
    // Postfix Operations: isPrimaryExpressionStatement or module (for files)
    // Unary Ops: PLUS, MINUS, EXCLAMATION, NOT
    // RAISE 
    return (isPrimaryExpressionStatement(token) == ANY || token == MODULE || token == PLUS || token == MINUS || 
            token == EXCLAMATION || token == NOT || token == RAISE); 
    
} 

static inline bool token_isPrimaryExpressionStatement(TokenType token) {
    // Literals (numbers, Strings)
    // Booleans (TRUE, FALSE)
    // IDENTIFIER
    // 'null' 
    // FUNC
    // ILLEGAL
    // '(' expression ')'
    return (token == INTEGER || token == BIN_INT || token == HEX_INT || token == IMAG || 
            token == FLOAT || token == RUNE || token == STRING || token == IDENTIFIER || 
            token == TOK_NULL || token == FUNC || token == ILLEGAL || token == LPAREN || 
            token == RPAREN); 
}

static inline bool token_isDeclStatement(TokenType token) {
    // Variable Declaration (with types + "Any") 
    // Function Declaration (FUNC)
    // Class/Struct Declaration (CLASS and STRUCT)
    // Enum Declaration (ENUM)
    // Module Declaration (MODULE)
    // Empty Declaration (SEMICOLON)
    return (token == ANY || token == FUNC || token == CLASS || token == STRUCT || 
            token == ENUM || token == MODULE || token == SEMICOLON); 
} 


static inline bool token_isSpecial(TokenType token) {
    return (token == TOK_ID || token == TOK_EOF || token == ILLEGAL || token == COMMENT); 
}

static inline bool token_isLiteral(TokenType token) {
    return token > TOK___LITERALS_BEGIN && token < TOK___LITERALS_END; 
}

static inline bool token_isKeyword(TokenType token) {
    return token > TOK___KEYWORDS_BEGIN && token < TOK___KEYWORDS_END; 
}

static inline bool token_isOperator(TokenType token) {
    return token > TOK___OPERATORS_BEGIN && token < TOK___OPERATORS_END; 
}

static inline bool token_isComparisonOperator(TokenType token) {
    return token > TOK___COMP_OPERATORS_BEGIN && token < TOK___COMP_OPERATORS_END; 
}

static inline bool token_isAssignmentOperator(TokenType token) {
    return token > TOK___ASSIGNMENT_OPERATORS_BEGIN && token < TOK___ASSIGNMENT_OPERATORS_END; 
}

static inline bool token_isDelimiter(TokenType token) {
    return token > TOK___DELIMITERS_OPERATORS_BEGIN && token < TOK___DELIMITERS_OPERATORS_END;
}

static inline bool token_isArrow(TokenType token) {
    return token > TOK___ARROW_OPERATORS_BEGIN && token < TOK___ARROW_OPERATORS_END;
}

static inline bool token_isBitwise(TokenType token) {
    return token > TOK___BITWISE_OPERATORS_BEGIN && token < TOK___BITWISE_OPERATORS_END;
}

static inline bool token_isSeparator(TokenType token) {
    return token > TOK___SEPARATORS_BEGIN && token < TOK___SEPARATORS_END;
}

static inline bool token_isIdentifier(TokenType token) {
    return token == IDENTIFIER; 
}

static inline bool token_isEOF(TokenType token) {
    return token == TOK_EOF; 
}

static inline bool token_isNULL(TokenType token) {
    return token == TOK_NULL; 
}

static inline bool token_isIllegal(TokenType token) {
    return token == ILLEGAL; 
}

static inline bool token_isMacro(TokenType token) {
    return token == MACRO; 
}

static inline bool token_isImport(TokenType token) {
    return token == IMPORT; 
}

static inline bool token_isInclude(TokenType token) {
    return token == INCLUDE; 
}

static inline bool token_isSemiColon(TokenType token) {
    return token == SEMICOLON; 
}