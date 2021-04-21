#include <stdlib.h> 
#include <string.h> 

#include <hazel/compiler/tokens/tokens.h>

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

void token_free(Token* token) {
    free(token->value); // Important because we allocate separately for `token->value`
    free(token);
}

char* token_to_string(Token* token) {
    char* str = calloc(strlen(token->value)+1, sizeof(char));
    strcpy(str, token->value);

    return str;
}

// NOTE: 
// Any changes made to this function _MUST_ reflect in the ALLTOKENS macro in <tokens.h> as well 
const char* token_toString(AllTokensEnum token) {
    switch(token) {
        // Special (internal usage only)
        case TOK_EOF: return "TOK_EOF";
        case ILLEGAL: return "ILLEGAL";
        case COMMENT: return "COMMENT";

        // Literals
        case IDENTIFIER: return "IDENTIFIER";
        case INTEGER: return "INTEGER";
        case BIN_INT: return "BIN_INT";
        case HEX_INT: return "HEX_INT";
        case IMAG: return "IMAG";
        case FLOAT: return "FLOAT";
        case RUNE: return "RUNE";
        case STRING: return "STRING";
        case TRUE: return "TRUE";
        case FALSE: return "FALSE";

        // Operators 
        case PLUS: return "+";
        case MINUS: return "-";
        case MULT: return "*";
        case QUOTIENT: return "/";
        case MOD: return "%";
        case MOD_MOD: return "%%";
        case AND: return "&";
        case OR: return "|";
        case NOT: return "!";
        case XOR: return "^";
        case AND_NOT: return "&^";
        case AND_AND: return "&&";
        case OR_OR: return "||";
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

        // Colons
        case COLON: return ":";  
        case SEMICOLON: return ";";  
        case COMMA: return ":";  
        case DOT: return ".";  
        case DDOT: return ".."; 
        case ELLIPSIS: return "...";
        case BACKSLASH: return "\\"; 

        // Bitshits
        case LBITSHIFT: return "<<";
        case RBITSHIFT: return ">>";

            // Keywords
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
        case DEFAULT: return "default"; 
        case ENUM: return "enum";    
        case ELSE: return "else";    
        case ELSEIF: return "elseif";  
        case EXPORT: return "export";  
        case FINALLY: return "finally"; 
        case FOR: return "for";     
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
        case MODULE: return "module";  
        case MUTABLE: return "mutable"; 
        case NO_INLINE: return "no_inline"; 
        case NOT_IN: return "not_in";  
        case RANGE: return "range";   
        case RETURN: return "return";  
        case STRUCT: return "struct";  
        case TRY: return "try";     
        case TYPEOF: return "typeof";  
        case WHEN: return "when";    
        case WHERE: return "where";   
        case WHILE: return "while";   
        case UNION: return "union";   
    }

    // We should _NEVER_ reach here 
    return "UNRECOGNIZED TOKEN"; 
}

bool token_isSpecial(Token token) {

} 

bool token_isLiteral(Token token) {

} 

bool token_isKeyword(Token token) {

} 

bool token_isOperator(Token token) {

} 

bool token_isComparisonOperator(Token token) {

} 

bool token_isAssignmentOperator(Token token) {

} 

bool token_isDelimiter(Token token) {

} 

bool token_isArrow(Token token) {

} 

bool token_isBitshift(Token token) {

} 

bool token_isColon(Token token) {

} 

inline bool token_isEOF(Token token) {

} 

inline bool token_isIllegal(Token token) {

} 

inline bool token_isMacro(Token token) {

}

inline bool token_isImport(Token token) {

}

inline bool token_isInclude(Token token) {

}
