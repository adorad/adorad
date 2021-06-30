#include <HazelInternalTests/HazelInternalTests.h>
#include <tau/tau.h>
TAU_MAIN()
 
TEST(Lexer, Init) {
    char* buffer = "0123456789abcdefghijklmnopqrstuvwxyz";
    Lexer* lexer = lexer_init(buffer, null);

    CHECK_STRNEQ(lexer->buffer, "");
    CHECK_EQ(lexer->buffer_capacity, strlen(buffer));
    CHECK_EQ(lexer->tokenList->capacity(lexer->tokenList), TOKENLIST_ALLOC_CAPACITY);
    CHECK_EQ(lexer->tokenList->size(lexer->tokenList), 0);
    CHECK_EQ(lexer->offset, 0);
    CHECK_EQ(lexer->lineno, 1);
    CHECK_EQ(lexer->colno, 1);
    CHECK_STREQ(lexer->fname, "");

    free(lexer);
}

// Without newline in buffer
TEST(Lexer, advance_without_newline) {
    char* buffer = "abcdefghijklmnopqrstuvwxyz0123456789";
    Lexer* lexer = lexer_init(buffer, null);
    
    for(UInt32 i=0; i < strlen(buffer); i++) {
        CHECK_STREQ(lexer->buffer, buffer);
        CHECK_EQ(lexer_advance(lexer), lexer->buffer[lexer->offset-1]);
        CHECK_EQ(lexer->tokenList->capacity(lexer->tokenList), TOKENLIST_ALLOC_CAPACITY);
        CHECK_EQ(lexer->tokenList->size(lexer->tokenList), 0);
        CHECK_EQ(lexer->offset, i+1);
        CHECK_EQ(lexer->colno, i+2);
        CHECK_EQ(lexer->lineno, 1);
    }
}

// With newline in buffer
TEST(Lexer, advance_with_newline) {
    char* buffer = "a\nb\ncdefghijklmnopqrstuvwxyz0123456789";
    Lexer* lexer = lexer_init(buffer, null);
    
    CHECK_STREQ(lexer->buffer, buffer);
    CHECK_EQ(lexer_advance(lexer), 'a');
    CHECK_EQ(lexer->offset, 1);
    CHECK_EQ(lexer->tokenList->capacity(lexer->tokenList), TOKENLIST_ALLOC_CAPACITY);
    CHECK_EQ(lexer->tokenList->size(lexer->tokenList), 0);
    CHECK_EQ(lexer->colno, 2);
    CHECK_EQ(lexer->lineno, 1);

    // Hit a newline
    CHECK_STREQ(lexer->buffer, buffer);
    CHECK_EQ(lexer_advance(lexer), '\n');
    CHECK_EQ(lexer->offset, 2);
    CHECK_EQ(lexer->tokenList->capacity(lexer->tokenList), TOKENLIST_ALLOC_CAPACITY);
    CHECK_EQ(lexer->tokenList->size(lexer->tokenList), 0);
    CHECK_EQ(lexer->colno, 3);
    CHECK_EQ(lexer->lineno, 1);
}

TEST(Lexer, advancen) {
    char* buffer = "abcdefghijklmnopqrstuvwxyz0123456789";
    Lexer* lexer = lexer_init(buffer, null);
    
    // Go ahead 4 chars
    char e = lexer_advancen(lexer, 4); // should be 'e'
    CHECK_EQ(e, 'e');
    CHECK_EQ(lexer->tokenList->capacity(lexer->tokenList), TOKENLIST_ALLOC_CAPACITY);
    CHECK_EQ(lexer->tokenList->size(lexer->tokenList), 0);
    CHECK_EQ(lexer->offset, 4);
    CHECK_EQ(lexer->colno, 5);
    CHECK_EQ(lexer->lineno, 1);

    // Go ahead 1 char
    char f = lexer_advancen(lexer, 1); // 'f'
    CHECK_EQ(f, 'f');
    CHECK_EQ(lexer->tokenList->capacity(lexer->tokenList), TOKENLIST_ALLOC_CAPACITY);
    CHECK_EQ(lexer->tokenList->size(lexer->tokenList), 0);
    CHECK_EQ(lexer->offset, 5);
    CHECK_EQ(lexer->colno, 6);
    CHECK_EQ(lexer->lineno, 1);

    // Go ahead 3 chars
    char i = lexer_advancen(lexer, 3); // 'i'
    CHECK_EQ(i, 'i');
    CHECK_EQ(lexer->tokenList->capacity(lexer->tokenList), TOKENLIST_ALLOC_CAPACITY);
    CHECK_EQ(lexer->tokenList->size(lexer->tokenList), 0);
    CHECK_EQ(lexer->offset, 8);
    CHECK_EQ(lexer->colno, 9);
    CHECK_EQ(lexer->lineno, 1);

    // Go ahead 7 chars
    char p = lexer_advancen(lexer, 7); // 'p'
    CHECK_EQ(p, 'p');
    CHECK_EQ(lexer->tokenList->capacity(lexer->tokenList), TOKENLIST_ALLOC_CAPACITY);
    CHECK_EQ(lexer->tokenList->size(lexer->tokenList), 0);
    CHECK_EQ(lexer->offset, 15);
    CHECK_EQ(lexer->colno, 16);
    CHECK_EQ(lexer->lineno, 1);
    
    // Go ahead 10 chars
    char z = lexer_advancen(lexer, 10); // 'z'
    CHECK_EQ(z, 'z');
    CHECK_EQ(lexer->tokenList->capacity(lexer->tokenList), TOKENLIST_ALLOC_CAPACITY);
    CHECK_EQ(lexer->tokenList->size(lexer->tokenList), 0);
    CHECK_EQ(lexer->offset, 25);
    CHECK_EQ(lexer->colno, 26);
    CHECK_EQ(lexer->lineno, 1);

    // Go ahead 10 chars
    char nine = lexer_advancen(lexer, 10); // '9'
    CHECK_EQ(nine, '9');
    CHECK_EQ(lexer->tokenList->capacity(lexer->tokenList), TOKENLIST_ALLOC_CAPACITY);
    CHECK_EQ(lexer->tokenList->size(lexer->tokenList), 0);
    CHECK_EQ(lexer->offset, 35);
    CHECK_EQ(lexer->colno, 36);
    CHECK_EQ(lexer->lineno, 1);

    // Go ahead 1 char (end of buff cap)
    char eof1 = lexer_advancen(lexer, 1);
    CHECK_EQ(eof1, nullchar);
    // Options should remain the same
    CHECK_EQ(lexer->tokenList->capacity(lexer->tokenList), TOKENLIST_ALLOC_CAPACITY);
    CHECK_EQ(lexer->tokenList->size(lexer->tokenList), 0);
    CHECK_EQ(lexer->offset, 35);
    CHECK_EQ(lexer->colno, 36);
    CHECK_EQ(lexer->lineno, 1);

    // Go ahead 4 more chars (end of cap)
    char eof2 = lexer_advancen(lexer, 4);
    CHECK_EQ(eof2, nullchar);
    // Options should remain the same
    CHECK_EQ(lexer->tokenList->capacity(lexer->tokenList), TOKENLIST_ALLOC_CAPACITY);
    CHECK_EQ(lexer->tokenList->size(lexer->tokenList), 0);
    CHECK_EQ(lexer->offset, 35);
    CHECK_EQ(lexer->colno, 36);
    CHECK_EQ(lexer->lineno, 1);
}

#define lt  lexer->tokenList

TEST(lexer, lex_keywords) {
    char* buffer = "atomic UInt32 var = 0x123;";
    Lexer* lexer = lexer_init(buffer, null);
    Token* tok;
    
    lexer_lex(lexer);

    // Test
    tok = lt->at(lt, 0);
    CHECK(tok->kind == ATOMIC);
    CHECK_EQ(tok->offset, 0);
    CHECK_EQ(tok->lineno, 1);
    CHECK_EQ(tok->colno, 1);
    CHECK_STREQ(tok->value, "atomic");
    CHECK_STREQ(tok->fname, "");
    
    tok = lt->at(lt, 1);
    CHECK(tok->kind == IDENTIFIER);
    CHECK_EQ(tok->offset, 7);
    CHECK_EQ(tok->lineno, 1);
    CHECK_EQ(tok->colno, 8);
    CHECK_STREQ(tok->value, "UInt32");
    CHECK_STREQ(tok->fname, "");

    tok = lt->at(lt, 2);
    CHECK(tok->kind == IDENTIFIER);
    CHECK_EQ(tok->offset, 14);
    CHECK_EQ(tok->lineno, 1);
    CHECK_EQ(tok->colno, 15);
    CHECK_STREQ(tok->value, "var");
    CHECK_STREQ(tok->fname, "");

    tok = lt->at(lt, 3);
    CHECK(tok->kind == EQUALS);
    CHECK_EQ(tok->offset, 17);
    CHECK_EQ(tok->lineno, 1);
    CHECK_EQ(tok->colno, 18);
    CHECK_STREQ(tok->value, "=");
    CHECK_STREQ(tok->fname, "");

    tok = lt->at(lt, 4);
    CHECK(tok->kind == HEX_INT);
    CHECK_EQ(tok->offset, 20);
    CHECK_EQ(tok->lineno, 1);
    CHECK_EQ(tok->colno, 21);
    CHECK_STREQ(tok->value, "0x123");
    CHECK_STREQ(tok->fname, "");

    tok = lt->at(lt, 5);
    CHECK(tok->kind == SEMICOLON);
    CHECK_EQ(tok->offset, 25);
    CHECK_EQ(tok->lineno, 1);
    CHECK_EQ(tok->colno, 26);
    CHECK_STREQ(tok->value, ";");
    CHECK_STREQ(tok->fname, "");

    tok = lt->at(lt, 6);
    CHECK(tok->kind == TOK_EOF);
    CHECK_STREQ(tok->value, "EOF");
}

// TEST(Lexer, lexer_lex_digits) {
//     char* buffer = "car = 0b1010101\nvar = 0xDeadBeef\nbar = 0o72626457263\nhar = 0b111111111111111111\nlar = 0x28300293";
//     int nbin_digits = 15;
//     Lexer* lexer = lexer_init(buffer, null);

//     // LexerTestArr
//     LexerTestArr* lta = LexerTestArr_alloc(nbin_digits);
//     LexerTestArr_append(lta, IDENTIFIER, 0, "car", 1, 1, "");
//     LexerTestArr_append(lta, EQUALS,     4, "=", 1, 5, "");
//     LexerTestArr_append(lta, BIN_INT,    6, "0b1010101", 1, 7, "");
//     LexerTestArr_append(lta, IDENTIFIER, 16, "var", 2, 1, "");
//     LexerTestArr_append(lta, EQUALS,     20, "=", 2, 1, "");
//     LexerTestArr_append(lta, HEX_INT,    22, "0xDeadBeef", 2, 1, "");
//     LexerTestArr_append(lta, IDENTIFIER, 33, "bar", 4, 1, "");
//     LexerTestArr_append(lta, EQUALS,     37, "=", 4, 1, "");
//     LexerTestArr_append(lta, BIN_INT,    39, "0b111111111111111111", 4, 1, "");
//     LexerTestArr_append(lta, IDENTIFIER, 53, "har", 3, 1, "");
//     LexerTestArr_append(lta, EQUALS,     57, "=", 3, 1, "");
//     LexerTestArr_append(lta, OCT_INT,    59, "0o72626457263", 3, 1, "");
//     LexerTestArr_append(lta, IDENTIFIER, 80, "lar", 5, 1, "");
//     LexerTestArr_append(lta, EQUALS,     84, "=", 5, 1, "");
//     LexerTestArr_append(lta, HEX_INT,    86, "0x28300293", 5, 1, "");

//     // Call lexer_lex()
//     lexer_lex(lexer);

//     CHECK_EQ(lexer->tokenList->size(lexer->tokenList), nbin_digits + 1);
//     CHECK_EQ(lta->cap, nbin_digits);

//     for(int i = 0; i<nbin_digits; i++) {
//         CHECK(lexer->tokenList->kind == lta->token->kind);
//         // CHECK_EQ(lexer->tokenList->offset, lta->token->offset);
//         // CHECK_EQ(lexer->tokenList->lineno, lta->token->lineno);
//         // CHECK_EQ(lexer->tokenList->colno, lta->token->colno);
//         CHECK_STREQ(lexer->tokenList->value, lta->token->value);
//         CHECK_STREQ(lexer->tokenList->fname, lta->token->fname);
//     }

//     lexer_free(lexer);
//     LexerTestArr_free(lta);
// }