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

const char* token_toString(Token* token) {

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
