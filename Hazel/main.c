#include <Hazel/Hazel.h>

int main(int argc, const char* const argv[]) {
    // C - Example: 
    // To compile a Hazel source file:
    // >> hazel compile hello.hzl
    // if(argc<3) {
    //     printf("Too few arguments"); 
    //     return 1;
    // }

    // if(strcmp(argv[1], "compile") == 0) {
    Lexer* lexer = calloc(1, sizeof(Lexer)); 
	char* buffer = readFile("test/LexerDemo.hzl");
	lexer = lexer_init(buffer); 
    printf("Lexer buffer: \n%s\n", lexer->buffer__);
    printf("--------------\n");
    
    // TokenKind t = lexer_lex_comment(lexer);

    // Delete when done 
    free(lexer); 
    free(buffer);

    // printf("Aren't we supposed to work? \n");
    return 0; 
}
