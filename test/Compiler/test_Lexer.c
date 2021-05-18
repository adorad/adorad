#include <Hazel/Hazel.h>
#include <Arrow/Arrow.h>

TEST(Lexer, Init) {
    char* buffer = "0123456789abcdefghijklmnopqrstuvwxyz";
    Lexer* lexer = lexer_init(buffer);

    CHECK(lexer->buffer__ != "");
    CHECK(lexer->buffer_capacity__ == 36);
    CHECK(lexer->offset__ == 0);
    CHECK(lexer->location__.lineno__ == 0);
    CHECK(lexer->location__.colno__ == 0);
    CHECK(lexer->location__.fname__ == "");

    free(lexer);
    free(buffer);
}

TEST(Lexer, next()) {
    char* buffer = "0123456789abcdefghijklmnopqrstuvwxyz";
    Lexer* lexer = lexer_init(buffer);
    CHECK(lexer_next(lexer) == '0');
    CHECK(strcmp(lexer->buffer__, buffer));
    CHECK(lexer->offset__ == 1);
    CHECK(lexer->location__.colno__ == 1);
    CHECK(lexer->location__.lineno__ == 0);
}