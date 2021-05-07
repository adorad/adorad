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

#include <Hazel/Compiler/Tokens/Tokens.h>

// Convert a Token to its respective String representation
std::string Token::toString() {
    switch(this->__type) {
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
        // We should _never_ reach here
        default: return "ILLEGAL";
    }
}

bool Token::isJumpStatement() {
	// Break (BREAK)
	// Continue (CONTINUE)
	// Return (RETURN)
	return (this->__type == BREAK || this->__type == CONTINUE || this->__type == RETURN); 
} 

bool Token::isLoopStatement() {
	// While (WHILE)
	// For (FOR)
	return (this->__type == WHILE || this->__type == FOR); 
} 

bool Token::isFlowStatement() {
	// If 
	// Match 
	return (this->__type == IF || this->__type == MATCH); 
} 

bool Token::isMatchStatement() {
	// Declarations used in match-case 
	return (this->__type == MATCH || this->__type == CASE || this->__type == DEFAULT); 
} 

bool Token::isExpressionStatement() {
	// Postfix Operations: isPrimaryExpressionStatement or module (for files)
	// Unary Ops: PLUS, MINUS, EXCLAMATION, NOT
	// RAISE 
	return (isPrimaryExpressionStatement() || this->__type == MODULE || this->__type == PLUS || this->__type == MINUS || 
            this->__type == EXCLAMATION || this->__type == NOT || this->__type == RAISE); 
} 

bool Token::isPrimaryExpressionStatement() {
	// Literals (numbers, Strings)
	// Booleans (TRUE, FALSE)
	// IDENTIFIER
	// 'null' 
	// FUNC
	// ILLEGAL
	// '(' expression ')'
	return (this->__type == INTEGER || this->__type == BIN_INT || this->__type == HEX_INT || this->__type == IMAG || 
			this->__type == FLOAT || this->__type == RUNE || this->__type == STRING || this->__type == IDENTIFIER || 
			this->__type == TOK_NULL || this->__type == FUNC || this->__type == TOK_ILLEGAL || this->__type == LPAREN || 
			this->__type == RPAREN); 
}

bool Token::isDeclStatement() {
	// Variable Declaration (with types + "Any") 
	// Function Declaration (FUNC)
	// Class/Struct Declaration (CLASS and STRUCT)
	// Enum Declaration (ENUM)
	// Module Declaration (MODULE)
	// Empty Declaration (SEMICOLON)
	return (this->__type == ANY || this->__type == FUNC || this->__type == CLASS || this->__type == STRUCT || 
			this->__type == ENUM || this->__type == MODULE || this->__type == SEMICOLON); 
} 


bool Token::isSpecial() {
	return (this->__type == TOK_ID || this->__type == TOK_EOF || this->__type == TOK_ILLEGAL || this->__type == COMMENT); 
}

bool Token::isLiteral() {
	return this->__type > TOK___LITERALS_BEGIN && this->__type < TOK___LITERALS_END; 
}

bool Token::isKeyword() {
	return this->__type > TOK___KEYWORDS_BEGIN && this->__type < TOK___KEYWORDS_END; 
}

bool Token::isOperator() {
	return this->__type > TOK___OPERATORS_BEGIN && this->__type < TOK___OPERATORS_END; 
}

bool Token::isComparisonOperator() {
	return this->__type > TOK___COMP_OPERATORS_BEGIN && this->__type < TOK___COMP_OPERATORS_END; 
}

bool Token::isAssignmentOperator() {
	return this->__type > TOK___ASSIGNMENT_OPERATORS_BEGIN && this->__type < TOK___ASSIGNMENT_OPERATORS_END; 
}

bool Token::isDelimiter() {
	return this->__type > TOK___DELIMITERS_OPERATORS_BEGIN && this->__type < TOK___DELIMITERS_OPERATORS_END;
}

bool Token::isArrow() {
	return this->__type > TOK___ARROW_OPERATORS_BEGIN && this->__type < TOK___ARROW_OPERATORS_END;
}

bool Token::isBitwise() {
	return this->__type > TOK___BITWISE_OPERATORS_BEGIN && this->__type < TOK___BITWISE_OPERATORS_END;
}

bool Token::isSeparator() {
	return this->__type > TOK___SEPARATORS_BEGIN && this->__type < TOK___SEPARATORS_END;
}

bool Token::isIdentifier() {
	return this->__type == IDENTIFIER; 
}

bool Token::isEOF() {
	return this->__type == TOK_EOF; 
}

bool Token::isNULL() {
	return this->__type == TOK_NULL; 
}

bool Token::isIllegal() {
	return this->__type == TOK_ILLEGAL; 
}

bool Token::isMacro() {
	return this->__type == MACRO; 
}

bool Token::isImport() {
	return this->__type == IMPORT; 
}

bool Token::isInclude() {
	return this->__type == INCLUDE; 
}

bool Token::isSemiColon() {
	return this->__type == SEMICOLON; 
}