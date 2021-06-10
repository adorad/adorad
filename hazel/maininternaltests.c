#include <hazel/hazel.h>

Token* sendtoken(TokenKind kind) {
    Token* token = (Token*)calloc(1, sizeof(Token));
    if(token == null) {
        fprintf(stderr, "Could not allocate memory. Memory full.");
    }
    // printf("INSIDE MAKETOKEN\n");
    token->kind = kind;
    token->offset = 0;
    token->colno = 0;
    token->lineno = 0;
    token->fname = 0;
    token->value = token_toString(kind);
    return token;
}

int main(int argc, const char* const argv[]) {
    // C - Example: 
    // To compile a Hazel source file:
    // >> hazel compile hello.hzl
    // if(argc<3) {
    //     printf("Too few arguments"); 
    //     return 1;
    // }

    // puts("    Hazel Language 0.0.1 (May 22 2021 02:39:23)");
    // puts("    GCC version: 9.3.0 on Windows");
    // puts("    All engines are a go!");


    // while(true) {
    //     printf("hazel> ");
    //     char buffer[20];
    //     fgets(buffer, 20, stdin);
    //     printf("%s", buffer);
    //     // printf("\n");
    // }

	char* buffer = readFile("test/LexerDemo.hzl");
    // char* buffer = "0123456789abcdefghijklmnopqrstuvwxyz";
	Lexer* lexer = lexer_init(buffer); 
    printf("Lexer buffer: \n%s\n", lexer->buffer);
    printf("--------------\n");
    lexer_lex(lexer);
    printf("Number of tokens = %d\n", lexer->num_tokens);
    for(int i=0; i<lexer->num_tokens; i++) {
        printf("TOKEN<%s>:%d:%d\n", lexer->tokenList[i].value, lexer->tokenList[i].lineno, lexer->tokenList[i].colno);
    }

    return 0; 
}
