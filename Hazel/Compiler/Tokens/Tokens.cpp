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


bool isJumpStatement(Token& token) {
    // Break (BREAK)
    // Continue (CONTINUE)
    // Return (RETURN)
    return (token.type == BREAK || token.type == CONTINUE || token.type == RETURN); 
} 

bool isLoopStatement(Token& token) {
    // While (WHILE)
    // For (FOR)
    return (token.type == WHILE || token.type == FOR); 
} 

bool isFlowStatement(Token& token) {
    // If 
    // Match 
    return (token.type == IF || token.type == MATCH); 
} 

bool isMatchStatement(Token& token) {
    // Declarations used in match-case 
    return (token.type == MATCH || token.type == CASE || token.type == DEFAULT); 
} 

bool isExpressionStatement(Token& token) {
    // Postfix Operations: isPrimaryExpressionStatement or module (for files)
    // Unary Ops: PLUS, MINUS, EXCLAMATION, NOT
    // RAISE 
    return (isPrimaryExpressionStatement(Token& token) || token.type == MODULE || token.type == PLUS || 
            token.type == MINUS || token.type == EXCLAMATION || token.type == NOT || token.type == RAISE); 
} 

bool isPrimaryExpressionStatement(Token& token) {
    // Literals (numbers, Strings)
    // Booleans (TRUE, FALSE)
    // IDENTIFIER
    // 'null' 
    // FUNC
    // ILLEGAL
    // '(' expression ')'
    return (token.type == INTEGER || token.type == BIN_INT || token.type == HEX_INT || token.type == IMAG || 
            token.type == FLOAT || token.type == RUNE || token.type == STRING || token.type == IDENTIFIER || 
            token.type == TOK_NULL || token.type == FUNC || token.type == TOK_ILLEGAL || token.type == LPAREN || 
            token.type == RPAREN); 
}

bool isDeclStatement(Token& token) {
    // Variable Declaration (with types + "Any") 
    // Function Declaration (FUNC)
    // Class/Struct Declaration (CLASS and STRUCT)
    // Enum Declaration (ENUM)
    // Module Declaration (MODULE)
    // Empty Declaration (SEMICOLON)
    return (token.type == ANY || token.type == FUNC || token.type == CLASS || token.type == STRUCT || 
            token.type == ENUM || token.type == MODULE || token.type == SEMICOLON); 
} 


bool isSpecial(Token& token) {
    return (token.type == TOK_ID || token.type == TOK_EOF || token.type == TOK_ILLEGAL || token.type == COMMENT); 
}

bool isLiteral(Token& token) {
    return token.type > TOK___LITERALS_BEGIN && tokeen->type < TOK___LITERALS_END; 
}

bool isKeyword(Token& token) {
    return token.type > TOK___KEYWORDS_BEGIN && token.type < TOK___KEYWORDS_END; 
}

bool isOperator(Token& token) {
    return token.type > TOK___OPERATORS_BEGIN && token.type < TOK___OPERATORS_END; 
}

bool isComparisonOperator(Token& token) {
    return token.type > TOK___COMP_OPERATORS_BEGIN && token.type < TOK___COMP_OPERATORS_END; 
}

bool isAssignmentOperator(Token& token) {
    return token.type > TOK___ASSIGNMENT_OPERATORS_BEGIN && token.type < TOK___ASSIGNMENT_OPERATORS_END; 
}

bool isDelimiter(Token& token) {
    return token.type > TOK___DELIMITERS_OPERATORS_BEGIN && token.type < TOK___DELIMITERS_OPERATORS_END;
}

bool isArrow(Token& token) {
    return token.type > TOK___ARROW_OPERATORS_BEGIN && token.type < TOK___ARROW_OPERATORS_END;
}

bool isBitwise(Token& token) {
    return token.type > TOK___BITWISE_OPERATORS_BEGIN && token.type < TOK___BITWISE_OPERATORS_END;
}

bool isSeparator(Token& token) {
    return token.type > TOK___SEPARATORS_BEGIN && token.type < TOK___SEPARATORS_END;
}

bool isIdentifier(Token& token) {
    return token.type == IDENTIFIER; 
}

bool isEOF(Token& token) {
    return token.type == TOK_EOF; 
}

bool isNULL(Token& token) {
    return token.type == TOK_NULL; 
}

bool isIllegal(Token& token) {
    return token.type == TOK_ILLEGAL; 
}

bool isMacro(Token& token) {
    return token.type == MACRO; 
}

bool isImport(Token& token) {
    return token.type == IMPORT; 
}

bool isInclude(Token& token) {
    return token.type == INCLUDE; 
}

bool isSemiColon(Token& token) {
    return token.type == SEMICOLON; 
}