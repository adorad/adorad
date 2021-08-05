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

    // printf("\033[1;32m\nTokens Vector: \033[0m\n");
    // for(UInt64 i=0; i < vec_size(lexer->tokenList); i++) {
    //     Token* tok = vec_at(lexer->tokenList, i);
    //     printf("TOKEN(%s, \"%s\")\n", token_to_string(tok->kind), tok->value);
    // } 
    // printf("\nTotal time = %lfs\n", total);

    printf("Number of tokens = %d\n", vec_size(lexer->tokenList));
    printf("Total allocated memory (in bytes) = %d\n", lexer->tokenList->internal.objsize * vec_size(lexer->tokenList));
    
    lexer_free(lexer);
    return 0; 
}
