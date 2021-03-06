#include <stdio.h> 

#include <hazel/runtime/lexer/lexer.h> 
#include <hazel/internal/defines.h> 

int main() {
    Lexer* lexer = lexer_init(
        "var name = \"John Doe\" \n" 
        "print(name)\n"
    ); 
    Token* token = null;
    token = lexer_get_next_token(lexer); 

    while( token != null) {
        printf("Token(%d, %d)", token->type, token->value);
    }
}
