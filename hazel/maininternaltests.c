#include <hazel/hazel.h>

// static void lex_identifier(Lexer* lexer) {
//     char ch = lexer->buffer[lexer->offset];
//     printf("Char 1 = %c\nCurr char: \n", ch);
//     int i = 0;
//     while(ch == '_' || isalnum(ch)) {
//         printf("%c ", ch);
//         ch = lexer_advance(lexer);
//         ++i;
//     }
//     printf("\nChar 2 = %d\n", (int)ch);
//     printf("i = %d\n", i);
// }

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
    // lexer_lex_identifier(lexer);
    lexer_lex(lexer);
    // printf("Number of tokens = %d\n", lexer->num_tokens);
    // for(int i=0; i<lexer->num_tokens; i++) {
    //     printf("TOKENKIND(%s) && TOKENVALUE(%s)\n", token_toString(lexer->tokenList[i].kind), lexer->tokenList[i].value);
    // }

    return 0; 
}
