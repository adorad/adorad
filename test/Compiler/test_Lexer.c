#include <HazelInternalTests/HazelInternalTests.h>
#include <Muon/Muon.h>
MUON_MAIN()

TEST(Lexer, Init) {
    char* buffer = "0123456789abcdefghijklmnopqrstuvwxyz";
    Lexer* lexer = lexer_init(buffer);

    CHECK_STRNEQ(lexer->buffer, "");
    CHECK_EQ(lexer->buffer_capacity, strlen(buffer));
    CHECK_EQ(lexer->offset, 0);
    CHECK_EQ(lexer->location.lineno, 1);
    CHECK_EQ(lexer->location.colno, 1);
    CHECK_STREQ(lexer->location.fname, "");

    free(lexer);
}

// Without newline in buffer
TEST(Lexer, next) {
    char* buffer = "abcdefghijklmnopqrstuvwxyz0123456789";
    Lexer* lexer = lexer_init(buffer);
    
    for(UInt32 i=0; i < strlen(buffer); i++) {
        CHECK_STREQ(lexer->buffer, buffer);
        CHECK_EQ(lexer_next(lexer), lexer->buffer[lexer->offset-1]);
        CHECK_EQ(lexer->offset, i+1);
        CHECK_EQ(lexer->location.colno, i+2);
        CHECK_EQ(lexer->location.lineno, 1);
    }
}