#include <adorad/adorad.h>

int main(int argc, const char* const argv[]) {
    // The CWD for this executable is in ".../build/bin"
	char* buffer = read_file("../../test/LexerDemo.ad");
	Lexer* lexer = lexer_init(buffer, "test/LexerDemo.ad"); 

    clock_t st, end;
    printf("Lexing beginning...\n");
    st = now();
    int count = 0;
    lexer_lex(lexer);
    end = now();
    printf("Lexing finished in ");
    double total = duration(st, end);
    printf("%lfs\n", total);

    Parser* parser = parser_init(lexer);
    AstNode* node = return_result(parser);
    if(node == null)
        printf("Null node");
    lexer_free(lexer);
    return 0; 
}