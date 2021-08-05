#include <adorad/adorad.h>
#include <adorad/core/os.h>

int main(int argc, const char* const argv[]) {
    // The CWD for this executable is in ".../build/bin"
    printf("Proof that executable works!!!\n");
	char* buffer = readFile("../../test/LexerDemo.ad");
	Lexer* lexer = lexer_init(buffer, "test/LexerDemo.ad"); 

    clock_t st, end;
    printf("Lexing beginning...\n");
    st = now();
    int count = 0;
    lexer_lex(lexer);
    end = now();
    printf("Lexing finished...\n");
    double total = duration(st, end);

    printf("\033[1;32m\nTokens Vector: \033[0m\n");
    for(Token* tok = vec_begin(lexer->toklist); tok <= vec_end(lexer->toklist); tok++) {
        printf("TOKEN(%s, \"%s\")\n", token_to_buff(tok->kind)->data, tok->value->data);
    }
    printf("\nTotal time = %lfs\n", total);

    printf("Number of tokens = %d\n", vec_size(lexer->toklist));
    printf("Total allocated memory (in bytes) = %d\n", lexer->toklist->internal.objsize * vec_size(lexer->toklist));
    
    lexer_free(lexer);
    return 0; 
}