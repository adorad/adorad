#include <Hazel/Hazel.h>
#include <Muon/Muon.h>
MUON_MAIN()

TEST(Lexer, Init) {
    char* buffer = "0123456789abcdefghijklmnopqrstuvwxyz";
    Lexer* lexer = lexer_init(buffer);

    CHECK(lexer->buffer__ != "", "Buffer must not be empty");
    CHECK(lexer->buffer_capacity__ == 36, "Buffer capacity mismatch");
    CHECK(lexer->offset__ == 0, "Offset must be 0");
    CHECK(lexer->location__.lineno__ == 1, "Lineno must be 0");
    CHECK(lexer->location__.colno__ == 0, "Colno must be 0");
    CHECK(lexer->location__.fname__ == "", "FName must be empty");

    free(lexer);
    free(buffer);
}

TEST(Lexer, next) {
    char* buffer = "0123456789abcdefghijklmnopqrstuvwxyz";
    Lexer* lexer = lexer_init(buffer);
    CHECK(lexer_next(lexer) == '0', "Needs to be `0`");
    CHECK(strcmp(lexer->buffer__, buffer), "Buffer lengths are not the same");
    CHECK(lexer->offset__ == 1, "Offset needs to be 1");
    CHECK(lexer->location__.colno__ == 1, "Colno needs to be 1");
    CHECK(lexer->location__.lineno__ == 1, "Lineno needs to be 1");
}