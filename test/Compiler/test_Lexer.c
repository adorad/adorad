#include <Hazel/Hazel.h>
#include <Muon/Muon.h>
MUON_MAIN()

// These tests will only pass if the Lexer functions it tests aren't declared as static, inline or both

TEST(Lexer, Init) {
    char* buffer = "0123456789abcdefghijklmnopqrstuvwxyz";
    Lexer* lexer = lexer_init(buffer);

    CHECK(strcmp(lexer->buffer, "") != 0, "Buffer must not be empty");
    CHECK(lexer->buffer_capacity == 36, "Buffer capacity mismatch");
    CHECK(lexer->offset == 0, "Offset must be 0");
    CHECK(lexer->location.lineno == 1, "Lineno must be 1");
    CHECK(lexer->location.colno == 1, "Colno must be 1");
    CHECK(strcmp(lexer->location.fname, "") == 0, "Filename must be empty");

    free(lexer);
}

TEST(Lexer, next) {
    char* buffer = "0123456789abcdefghijklmnopqrstuvwxyz";
    Lexer* lexer = lexer_init(buffer);
    
    CHECK(strcmp(lexer->buffer, buffer) == 0, "Buffer lengths are not the same");
    // CHECK(lexer_next(lexer) == '0', "Needs to be `0`");
    // CHECK(lexer->offset == 1, "Offset needs to be 1");
    // CHECK(lexer->location.colno == 2, "Colno needs to be 2");
    // CHECK(lexer->location.lineno == 1, "Lineno needs to be 1");
}