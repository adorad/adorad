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
	Lexer* lexer = lexer_init(buffer, "test/LexerDemo.hzl"); 
    // printf("-- LEXER_BUFFER: \n%s\n", lexer->buffer);
    printf("--------------\n");

    clock_t st, end;
    printf("Lexing beginning...\n");
    st = now();
    int count = 0;
    lexer_lex(lexer);
    end = now();
    printf("Lexing finished...\n");
    double total = duration(st, end);

    printf("Number of tokens = %d\n", lexer->tokenList->size(lexer->tokenList));
    printf("Total allocated memory (in bytes) = %d\n", lexer->tokenList->internal.objsize * lexer->tokenList->size(lexer->tokenList));
    printf("\033[1;32m\nTokens Vector: \033[0m\n");
    for(UInt64 i=0; i < lexer->tokenList->size(lexer->tokenList); i++) {
        Token* tok = lexer->tokenList->at(lexer->tokenList, i);
        printf("TOKEN(%s, \"%s\")\n", token_to_string(tok->kind), tok->value);
    }
    printf("Total time = %lfs\n", total);

    return 0; 
}
