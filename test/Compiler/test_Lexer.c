#include <Hazel/Hazel.h>
#include <Muon/Muon.h>
MUON_MAIN()

// These tests will only pass if the Lexer functions it tests aren't declared as static, inline or both

TEST(Lexer, Init) {
    char* buffer = "0123456789abcdefghijklmnopqrstuvwxyz";
    Lexer* lexer = lexer_init(buffer);

    CHECK(strcmp(lexer->buffer__, "") != 0, "Buffer must not be empty");
    CHECK(lexer->buffer_capacity__ == 36, "Buffer capacity mismatch");
    CHECK(lexer->offset__ == 0, "Offset must be 0");
    CHECK(lexer->location__.lineno__ == 1, "Lineno must be 1");
    CHECK(lexer->location__.colno__ == 1, "Colno must be 1");
    CHECK(strcmp(lexer->location__.fname__, "") == 0, "Filename must be empty");

    free(lexer);
}

TEST(Lexer, next) {
    char* buffer = "0123456789abcdefghijklmnopqrstuvwxyz";
    Lexer* lexer = lexer_init(buffer);
    
    CHECK(strcmp(lexer->buffer__, buffer) == 0, "Buffer lengths are not the same");
    CHECK(lexer_next(lexer) == '0', "Needs to be `0`");
    CHECK(lexer->offset__ == 1, "Offset needs to be 1");
    CHECK(lexer->location__.colno__ == 2, "Colno needs to be 2");
    CHECK(lexer->location__.lineno__ == 1, "Lineno needs to be 1");
}