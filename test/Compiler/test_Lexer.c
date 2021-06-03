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
TEST(Lexer, advance_without_newline) {
    char* buffer = "abcdefghijklmnopqrstuvwxyz0123456789";
    Lexer* lexer = lexer_init(buffer);
    
    for(UInt32 i=0; i < strlen(buffer); i++) {
        CHECK_STREQ(lexer->buffer, buffer);
        CHECK_EQ(lexer_advance(lexer), lexer->buffer[lexer->offset-1]);
        CHECK_EQ(lexer->offset, i+1);
        CHECK_EQ(lexer->location.colno, i+2);
        CHECK_EQ(lexer->location.lineno, 1);
    }
}

// With newline in buffer
TEST(Lexer, advance_with_newline) {
    char* buffer = "a\nb\ncdefghijklmnopqrstuvwxyz0123456789";
    Lexer* lexer = lexer_init(buffer);
    
    CHECK_STREQ(lexer->buffer, buffer);
    CHECK_EQ(lexer_advance(lexer), 'a');
    CHECK_EQ(lexer->offset, 1);
    CHECK_EQ(lexer->location.colno, 2);
    CHECK_EQ(lexer->location.lineno, 1);

    // Hit a newline
    CHECK_STREQ(lexer->buffer, buffer);
    CHECK_EQ(lexer_advance(lexer), '\n');
    CHECK_EQ(lexer->offset, 2);
    CHECK_EQ(lexer->location.colno, 3);
    CHECK_EQ(lexer->location.lineno, 1);
}

TEST(Lexer, advance_n) {
    char* buffer = "abcdefghijklmnopqrstuvwxyz0123456789";
    Lexer* lexer = lexer_init(buffer);
    
    // Go ahead 4 chars
    char e = lexer_advance_n(lexer, 4); // should be 'e'
    CHECK_EQ(e, 'e');
    CHECK_EQ(lexer->offset, 4);
    CHECK_EQ(lexer->location.colno, 5);
    CHECK_EQ(lexer->location.lineno, 1);

    // Go ahead 1 char
    char f = lexer_advance_n(lexer, 1); // 'f'
    CHECK_EQ(f, 'f');
    CHECK_EQ(lexer->offset, 5);
    CHECK_EQ(lexer->location.colno, 6);
    CHECK_EQ(lexer->location.lineno, 1);

    // Go ahead 3 chars
    char i = lexer_advance_n(lexer, 3); // 'i'
    CHECK_EQ(i, 'i');
    CHECK_EQ(lexer->offset, 8);
    CHECK_EQ(lexer->location.colno, 9);
    CHECK_EQ(lexer->location.lineno, 1);

    // Go ahead 7 chars
    char p = lexer_advance_n(lexer, 7); // 'p'
    CHECK_EQ(p, 'p');
    CHECK_EQ(lexer->offset, 15);
    CHECK_EQ(lexer->location.colno, 16);
    CHECK_EQ(lexer->location.lineno, 1);
    
    // Go ahead 10 chars
    char z = lexer_advance_n(lexer, 10); // 'z'
    CHECK_EQ(z, 'z');
    CHECK_EQ(lexer->offset, 25);
    CHECK_EQ(lexer->location.colno, 26);
    CHECK_EQ(lexer->location.lineno, 1);

    // Go ahead 10 chars
    char nine = lexer_advance_n(lexer, 10); // '9'
    CHECK_EQ(nine, '9');
    CHECK_EQ(lexer->offset, 35);
    CHECK_EQ(lexer->location.colno, 36);
    CHECK_EQ(lexer->location.lineno, 1);

    // Go ahead 1 char (end of buff cap)
    char eof1 = lexer_advance_n(lexer, 1);
    CHECK_EQ(eof1, nullchar);
    // Options should remain the same
    CHECK_EQ(lexer->offset, 35);
    CHECK_EQ(lexer->location.colno, 36);
    CHECK_EQ(lexer->location.lineno, 1);

    // Go ahead 4 more chars (end of cap)
    char eof2 = lexer_advance_n(lexer, 4);
    CHECK_EQ(eof2, nullchar);
    // Options should remain the same
    CHECK_EQ(lexer->offset, 35);
    CHECK_EQ(lexer->location.colno, 36);
    CHECK_EQ(lexer->location.lineno, 1);
}