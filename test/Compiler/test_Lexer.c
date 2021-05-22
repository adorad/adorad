#include <Hazel/Hazel.h>
#include <Muon/Muon.h>
MUON_MAIN()

// These tests will only pass if the Lexer functions it tests aren't declared as static, inline or both

TEST(Lexer, Init) {
    char* buffer = "0123456789abcdefghijklmnopqrstuvwxyz";
    Lexer* lexer = lexer_init(buffer);

    CHECK_NE(strcmp(lexer->buffer, ""), 0);
    CHECK_EQ(lexer->buffer_capacity, 36);
    CHECK_EQ(lexer->offset, 0);
    CHECK_EQ(lexer->location.lineno, 1);
    CHECK_EQ(lexer->location.colno, 1);
    CHECK_EQ(strcmp(lexer->location.fname, ""), 0);

    free(lexer);
}

// Without newline in buffer
TEST(Lexer, next1) {
    char* buffer = "abcdefghijklmnopqrstuvwxyz0123456789";
    Lexer* lexer = lexer_init(buffer);
    
    CHECK_EQ(strcmp(lexer->buffer, buffer), 0);
    CHECK_EQ(lexer_next(lexer), 'a');
    CHECK_EQ(lexer->offset, 134);
    CHECK_EQ(lexer->location.colno, 2);
    CHECK_EQ(lexer->location.lineno, 1);
}