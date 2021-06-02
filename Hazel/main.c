#include <Hazel/Hazel.h>

int main(int argc, const char* const argv[]) {
    // C - Example: 
    // To compile a Hazel source file:
    // >> hazel compile hello.hzl
    // if(argc<3) {
    //     printf("Too few arguments"); 
    //     return 1;
    // }

    puts("    Hazel Language 0.0.1 (May 22 2021 02:39:23)");
    puts("    GCC version: 9.3.0 on Windows");
    puts("    All engines are a go!");

    while(true) {
        printf("hazel> ");
        char buffer[20];
        fgets(buffer, 20, stdin);
        printf("%s", buffer);
        // printf("\n");
    }

    // if(strcmp(argv[1], "compile") == 0) {
	// char* buffer = readFile("test/LexerDemo.hzl");
    // char* buffer = "abcdefghijklmnopqrstuvwxyz";
	// Lexer* lexer = lexer_init(buffer); 
    // printf("Lexer buffer: \n%s\n", lexer->buffer);
    // printf("--------------\n");
    
    // // char ch = lexer_next(lexer);
    // printf("FIRST: %c\n", ch);
    // // ch = lexer_next(lexer);
    // printf("Character received: %c\n", ch);
    // // lexer_print_stats(lexer);

    // Delete when done 
    // free(lexer); 

    return 0; 
}
