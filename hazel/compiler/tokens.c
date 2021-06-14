/*
_ _    _           ______   _______        
| |  | |    /\    /___  /   |  ____|| |    
| |__| |   /  \      / /    | |__   | |       Hazel - The Fast, Expressive & Elegant Programming Language
|  __  |  / /\ \    / /     |  __|  | |       Languages: C, C++, and Assembly
| |  | | / ____ \  / /___   | |____ | |____   https://github.com/HazelLang/hazel/
|_|_ |_|/_/    \_\/_______\ |______|_\______|

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/

#include <hazel/compiler/tokens.h>

// String representation of a TokenKind
// To access the string representation of a TokenKind object, simply use `tokenHash[tokenkind]`
static const char* tokenHash[] = {
    #define TOKENKIND(kind, str)    str
        ALLTOKENS
    #undef TOKENKIND
};

// Token constructor
Token* token_init(void) {
    Token* token = calloc(1, sizeof(Token));
    token->kind = TOK_ILLEGAL; 
    token->offset = 0; 
    token->tok_length = 0; 
    token->value = "";
    token->lineno = 1;
    token->colno = 1; 
    token->fname = ""; 

    return token;
}

// Reset a Token instance
void token_reset_token(Token* token) {
    token->kind = TOK_ILLEGAL; 
    token->offset = 0; 
    token->tok_length = 0; 
    token->value = "";
    token->lineno = 0; 
    token->colno = 0; 
    token->fname = "";
}

// Returns the token corresponding to a single character
static TokenKind token_onechar(char c) {
    switch(c) {
        // Operators
        case '+': return PLUS;
        case '-': return MINUS;
        case '*': return MULT;
        case '/': return SLASH;
        case '%': return MOD;
        case '@': return AT_SIGN;
        case '#': return HASH_SIGN;
        case '?': return QUESTION;
        // Comparison Operators
        case '>': return GREATER_THAN;
        case '<': return LESS_THAN;
        // Assignment operators
        case '=': return EQUALS;
        case '~': return TILDA;
        // Delimiters
        case '[': return LSQUAREBRACK;
        case ']': return RSQUAREBRACK;
        case '{': return LBRACE; 
        case '}': return RBRACE;
        case '(': return LPAREN;
        case ')': return RPAREN;
        // Bitwise Operators
        case '&': return AND;
        case '|': return OR;
        case '!': return EXCLAMATION;
        // Separators
        case ':': return COLON;
        case ';': return SEMICOLON;
        case ',': return COMMA;
        case '.': return DOT;
        // This is actually one character - the second backslash acts as an escape character in C.
        case '\\': return BACKSLASH;
    }
    // We should never get here
    return TOK_ILLEGAL;
}

// Returns the token corresponding to two characters
static TokenKind token_twochar(char c1, char c2) {
    switch(c1) {
        case '+':
            if(c2 == '+') return PLUS_PLUS;
            if(c2 == '=') return PLUS_EQUALS;
            break;
        case '-':
            if(c2 == '-') return MINUS_MINUS;
            if(c2 == '=') return MINUS_EQUALS;
            if(c2 == '>') return RARROW;
            break;
        case '*':
            if(c2 == '*') return MULT_MULT;
            if(c2 == '=') return MULT_EQUALS;
            break;
        case '/':
            if(c2 == '/') return SLASH_SLASH;
            if(c2 == '=') return SLASH_EQUALS;
            break;
        case '%':
            if(c2 == '%') return MOD_MOD;
            if(c2 == '=') return MOD_EQUALS;
            break;
        case '&':
            if(c2 == '&') return AND_AND;
            if(c2 == '^') return AND_NOT;
            if(c2 == '=') return AND_EQUALS;
            break;
        case '|':
            if(c2 == '|') return OR_OR;
            if(c2 == '=') return OR_EQUALS;
            break;
        case '^':
            if(c2 == '=') return XOR_EQUALS;
            break;
        case '<':
            if(c2 == '=') return LESS_THAN_OR_EQUAL_TO;
            if(c2 == '-') return LARROW;
            if(c2 == '<') return LBITSHIFT;
            break;
        case '>':
            if(c2 == '=') return GREATER_THAN_OR_EQUAL_TO;
            if(c2 == '>') return RBITSHIFT;
            break;
        case '!':
            if(c2 == '=') return EXCLAMATION_EQUALS;
            break;
        case '=':
            if(c2 == '=') return EQUALS_EQUALS;
            break;
        case '~':
            if(c2 == '=') return TILDA_EQUALS;
            break;
        case '.':
            if(c2 == '.') return DDOT;
            break;
    }
    // We should never get here
    return TOK_ILLEGAL;
}

// Returns the token corresponding to three characters
static TokenKind token_threechar(char c1, char c2, char c3) {
    switch(c1) {
        case '<':
            if(c2 == '<' && c3 == '=') return LBITSHIFT_EQUALS;
            break;
        case '>':
            if(c2 == '>' && c3 == '=') return RBITSHIFT_EQUALS;
            break;
        case '.':
            if(c2 == '.' && c3 == '.') return ELLIPSIS;
            break;
    }
    // We should never get here
    return TOK_ILLEGAL;
}

// Convert a Token to its respective String representation
const char* token_toString(TokenKind kind) {
    switch(kind) {
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
        case OCT_INT: return "OCT_INT";
        case INT8_LIT: return "INT8_LIT";
        case INT16_LIT: return "INT16_LIT";
        case INT32_LIT: return "INT32_LIT";
        case INT64_LIT: return "INT64_LIT";
        case UINT_LIT: return "UINT_LIT";
        case UINT8_LIT: return "UINT8_LIT";
        case UINT16_LIT: return "UINT16_LIT";
        case UINT32_LIT: return "UINT32_LIT";
        case UINT64_LIT: return "UINT64_LIT";
        case TOK_FLOAT: return "FLOAT";
        case FLOAT32_LIT: return "FLOAT32_LIT";
        case FLOAT64_LIT: return "FLOAT64_LIT";
        case FLOAT128_LIT: return "FLOAT128_LIT";
        case IMAG: return "IMAG";
        case RUNE: return "RUNE";
        case STRING: return "STRING";
        case RAW_STRING: return "RAW_STRING";
        case TRIPLE_STRING: return "TRIPLE_STRING"; 
        case TOK_TRUE: return "TRUE";
        case TOK_FALSE: return "FALSE";

        // Operators 
        case PLUS: return "+";
        case MINUS: return "-";
        case MULT: return "*";
        case SLASH: return "/";
        case MOD: return "%";
        case MOD_MOD: return "%%";
        case PLUS_PLUS: return "++";
        case MINUS_MINUS: return "--";
        case MULT_MULT: return "**";
        case SLASH_SLASH: return "//";
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
        case TILDA_EQUALS: return "~=";

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
        case COLON_COLON: return "::";
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
        case EXTERN: return "inline";
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
        case PRAGMA: return "pragma";  
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
        case USE: return "use";
        // We should _never_ reach here
        default: return "ILLEGAL";
    }
}

inline bool token_isJumpStatement(Token* token) {
	// Break (BREAK)
	// Continue (CONTINUE)
	// Return (RETURN)
	return (token->kind == BREAK || token->kind == CONTINUE || token->kind == RETURN); 
} 

inline bool token_isLoopStatement(Token* token) {
	// While (WHILE)
	// For (FOR)
	return (token->kind == WHILE || token->kind == FOR); 
} 

inline bool token_isFlowStatement(Token* token) {
	// If 
	// Match 
	return (token->kind == IF || token->kind == MATCH); 
} 

inline bool token_isMatchStatement(Token* token) {
	// Declarations used in match-case 
	return (token->kind == MATCH || token->kind == CASE || token->kind == DEFAULT); 
} 

inline bool token_isExpressionStatement(Token* token) {
	// Postfix Operations: isPrimaryExpressionStatement or module (for files)
	// Unary Ops: PLUS, MINUS, EXCLAMATION, NOT
	// RAISE 
	return (token_isPrimaryExpressionStatement(token) || token->kind == MODULE || token->kind == PLUS || 
            token->kind == MINUS || token->kind == EXCLAMATION || token->kind == NOT || token->kind == RAISE); 
} 

inline bool token_isPrimaryExpressionStatement(Token* token) {
	// Literals (numbers, Strings)
	// inline booletoken_ans (TRUE, FALSE)
	// IDENTIFIER
	// 'null' 
	// FUNC
	// ILLEGAL
	// '(' expression ')'
	return (token->kind == INTEGER || token->kind == BIN_INT || token->kind == HEX_INT || token->kind == IMAG || 
			token->kind == TOK_FLOAT || token->kind == RUNE || token->kind == STRING || token->kind == IDENTIFIER || 
			token->kind == TOK_NULL || token->kind == FUNC || token->kind == TOK_ILLEGAL || token->kind == LPAREN || 
			token->kind == RPAREN); 
} 

inline bool token_isDeclStatement(Token* token) {
	// Variable Declaration (with types + "Any") 
	// Function Declaration (FUNC)
	// Class/Struct Declaration (CLASS and STRUCT)
	// Enum Declaration (ENUM)
	// Module Declaration (MODULE)
	// Empty Declaration (SEMICOLON)
	return (token->kind == ANY || token->kind == FUNC || token->kind == CLASS || token->kind == STRUCT || 
			token->kind == ENUM || token->kind == MODULE || token->kind == SEMICOLON); 
} 

inline bool token_isSpecial(Token* token) {
	return (token->kind == TOK_ID || token->kind == TOK_EOF || token->kind == TOK_ILLEGAL || token->kind == COMMENT); 
}

inline bool token_isLiteral(Token* token) {
	return token->kind > TOK___LITERALS_BEGIN && token->kind < TOK___LITERALS_END; 
}

inline bool token_isKeyword(Token* token) {
	return token->kind > TOK___KEYWORDS_BEGIN && token->kind < TOK___KEYWORDS_END; 
}

inline bool token_isOperator(Token* token) {
	return token->kind > TOK___OPERATORS_BEGIN && token->kind < TOK___OPERATORS_END; 
}

inline bool token_isComparisonOperator(Token* token) {
	return token->kind > TOK___COMP_OPERATORS_BEGIN && token->kind < TOK___COMP_OPERATORS_END; 
}

inline bool token_isAssignmentOperator(Token* token) {
	return token->kind > TOK___ASSIGNMENT_OPERATORS_BEGIN && token->kind < TOK___ASSIGNMENT_OPERATORS_END; 
}

inline bool token_isDelimiter(Token* token) {
	return token->kind > TOK___DELIMITERS_OPERATORS_BEGIN && token->kind < TOK___DELIMITERS_OPERATORS_END;
}

inline bool token_isArrow(Token* token) {
	return token->kind > TOK___ARROW_OPERATORS_BEGIN && token->kind < TOK___ARROW_OPERATORS_END;
}

inline bool token_isBitwise(Token* token) {
	return token->kind > TOK___BITWISE_OPERATORS_BEGIN && token->kind < TOK___BITWISE_OPERATORS_END;
}

inline bool token_isSeparator(Token* token) {
	return token->kind > TOK___SEPARATORS_BEGIN && token->kind < TOK___SEPARATORS_END;
}

inline bool token_isIdentifier(Token* token) {
	return token->kind == IDENTIFIER; 
}

inline bool token_isEOF(Token* token) {
	return token->kind == TOK_EOF; 
}

inline bool token_isNULL(Token* token) {
	return token->kind == TOK_NULL; 
}

inline bool token_isIllegal(Token* token) {
	return token->kind == TOK_ILLEGAL; 
}

inline bool token_isMacro(Token* token) {
	return token->kind == MACRO; 
}

inline bool token_isImport(Token* token) {
	return token->kind == IMPORT; 
}

inline bool token_isInclude(Token* token) {
	return token->kind == INCLUDE; 
}

inline bool token_isSemiColon(Token* token) {
	return token->kind == SEMICOLON; 
}
