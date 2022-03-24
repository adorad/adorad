#include <adorad/adorad.h>

int main(int argc, const char* const argv[]) {
    // The CWD for this executable is in ".../build/bin"
    BuffView cwd = os_get_cwd();
    printf("CWD = %s\n", cwd.data);
    printf("Test1 = %s\n", ospd(cwd).data);
    printf("Test2 = %s\n", ospd(ospd(cwd)).data);
    printf("Test3 = %s\n", ospj(ospd(ospd(cwd)), BV("test/LexerDemo.ad")).data);
    BuffView filepath = ospj(ospd(ospd(cwd)), BV("test/LexerDemo.ad"));
    printf("Reading file from %s\n", filepath.data);
	char* buffer = read_file(filepath.data);

	Lexer* lexer = lexer_init(buffer, "test/LexerDemo.ad"); 

    clock_t st, end;
    printf("Lexing beginning...\n");
    st = clock_now();
    lexer_lex(lexer);
    end = clock_now();
    printf("Lexing finished in ");
    double total = clock_duration(st, end);
    printf("%lfs\n", total);

    // Loop over tokens
    Token* token = null;
    printf("Tokens: \n---------------------\n");
    for(UInt64 i=0; i<lexer->toklist->core.len; i++) {
        token = cast(Token*)vec_at(lexer->toklist, i);
        printf("%s: %s\n", token_to_buff(token->kind)->data, token->value->data);
    }
    printf("---------------------\n");


    Parser* parser = parser_init(lexer);
    AstNode* node = return_result(parser);
    if(NONE(node))
        printf("Null node\n");

    parser_free(parser); // Frees the lexer as well

    return 0; 
}