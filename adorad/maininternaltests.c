#include <adorad/adorad.h>

int main(int argc, const char* const argv[]) {
    // Buff* buf = os_get_cwd();
    // printf("CWD: %s\n", buf->data);

	char* buffer = readFile("/home/jas/Documents/dev/adorad/test/LexerDemo.ad");
    // char* buffer = "0123456789abcdefghijklmnopqrstuvwxyz";
	Lexer* lexer = lexer_init(buffer, "test/LexerDemo.ad"); 
    // printf("-- LEXER_BUFFER: \n%s\n", lexer->buffer->data);
    printf("--------------\n");

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
