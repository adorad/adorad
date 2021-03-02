#ifndef HAZEL_TOKEN
#define HAZEL_TOKEN 


typdef struct TOKEN_STRUCT {
    enum {
        TOKEN_ID, 
        TOKEN_EQUALS,
        TOKEN_STRING,
        TOKEN_SEMICOLON,
        TOKEN_LPARENTHESIS,
        TOKEN_RPARENTHESIS,
    } type; 

    char* value;

} token_T 

token_T* token_init(int type, char* value);


#endif // HAZEL_TOKEN 
