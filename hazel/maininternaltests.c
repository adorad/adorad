#include <hazel/hazel.h>
#include <time.h>

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
    // printf("-- LEXER_BUFFER: \n%s\n", lexer->buffer);
    printf("--------------\n");

    cstlVector* vec = vec_new(sizeof(Token), 40);
    Token* token = lexer_maketoken(lexer, EQUALS, null);
    printf("First, token = %d\n", token->kind);
    printf("No segfault 1\n");
    vec->push(vec, token);
    printf("Should print\n");
    printf("vec->size = %d\n",vec->size);
    for(int i = 0; i< vec->size; i++) {
        printf(" i = %d\n", i);
        Token* tok = vec_at(vec, i);
        printf("%d", tok->kind);
    }

    // clock_t st, end, total;
    // st = clock();
    // lexer_lex(lexer);
    // end = clock();
    // printf("Number of tokens = %d\n", lexer->num_tokens);
    // for(int i=0; i<lexer->num_tokens; i++) {
    //     printf("TOKEN(%s, %s)\n", token_toString(lexer->tokenList[i].kind), lexer->tokenList[i].value);
    // }
    // printf("Total time = %lf\n", (double)((end-st)/CLOCKS_PER_SEC));

    return 0; 
}
