#include <AdoradInternalTests/AdoradInternalTests.h>
#include <tau/tau.h>
TAU_MAIN()
 
TEST(Lexer, Init) {
    char* buffer = "0123456789abcdefghijklmnopqrstuvwxyz";
    Lexer* lexer = lexer_init(buffer, null);

    ENFORCE_STRNE(lexer->buffer->data, "");
    ENFORCE_EQ(lexer->buffer->len, strlen(buffer));
    ENFORCE_EQ(vec_cap(lexer->toklist), TOKENLIST_ALLOC_CAPACITY);
    ENFORCE_EQ(vec_size(lexer->toklist), 0);
    ENFORCE_EQ(lexer->offset, 0);
    ENFORCE_EQ(lexer->loc->line, 1);
    ENFORCE_EQ(lexer->loc->col, 1);
    ENFORCE_STREQ(lexer->loc->fname->data, "");

    free(lexer);
}

// // Without newline in buffer
// TEST(Lexer, advance_without_newline) {
//     char* buffer = "abcdefghijklmnopqrstuvwxyz0123456789";
//     Lexer* lexer = lexer_init(buffer, null);
    
//     for(UInt32 i=0; i < strlen(buffer); i++) {
//         ENFORCE_STREQ(lexer->buffer->data, buffer);
//         ENFORCE_EQ(lexer_advance(lexer), lexer->buffer->data[lexer->offset-1]);
//         ENFORCE_EQ(vec_cap(lexer->toklist), TOKENLIST_ALLOC_CAPACITY);
//         ENFORCE_EQ(vec_size(lexer->toklist), 0);
//         ENFORCE_EQ(lexer->offset, i+1);
//         ENFORCE_EQ(lexer->loc->col, i+2);
//         ENFORCE_EQ(lexer->loc->line, 1);
//     }
// }

// // With newline in buffer
// TEST(Lexer, advance_with_newline) {
//     char* buffer = "a\nb\ncdefghijklmnopqrstuvwxyz0123456789";
//     Lexer* lexer = lexer_init(buffer, null);
    
//     ENFORCE_STREQ(lexer->buffer->data, buffer);
//     ENFORCE_EQ(lexer_advance(lexer), 'a');
//     ENFORCE_EQ(lexer->offset, 1);
//     ENFORCE_EQ(vec_cap(lexer->toklist), TOKENLIST_ALLOC_CAPACITY);
//     ENFORCE_EQ(vec_size(lexer->toklist), 0);
//     ENFORCE_EQ(lexer->loc->col, 2);
//     ENFORCE_EQ(lexer->loc->line, 1);

//     // Hit a newline
//     ENFORCE_STREQ(lexer->buffer->data, buffer);
//     ENFORCE_EQ(lexer_advance(lexer), '\n');
//     ENFORCE_EQ(lexer->offset, 2);
//     ENFORCE_EQ(vec_cap(lexer->toklist), TOKENLIST_ALLOC_CAPACITY);
//     ENFORCE_EQ(vec_size(lexer->toklist), 0);
//     ENFORCE_EQ(lexer->loc->col, 3);
//     ENFORCE_EQ(lexer->loc->line, 1);
// }

// TEST(Lexer, advancen) {
//     char* buffer = "abcdefghijklmnopqrstuvwxyz0123456789";
//     Lexer* lexer = lexer_init(buffer, null);
    
//     // Go ahead 4 chars
//     char e = lexer_advancen(lexer, 4); // should be 'e'
//     ENFORCE_EQ(e, 'e');
//     ENFORCE_EQ(vec_cap(lexer->toklist), TOKENLIST_ALLOC_CAPACITY);
//     ENFORCE_EQ(vec_size(lexer->toklist), 0);
//     ENFORCE_EQ(lexer->offset, 4);
//     ENFORCE_EQ(lexer->loc->col, 5);
//     ENFORCE_EQ(lexer->loc->line, 1);

//     // Go ahead 1 char
//     char f = lexer_advancen(lexer, 1); // 'f'
//     ENFORCE_EQ(f, 'f');
//     ENFORCE_EQ(vec_cap(lexer->toklist), TOKENLIST_ALLOC_CAPACITY);
//     ENFORCE_EQ(vec_size(lexer->toklist), 0);
//     ENFORCE_EQ(lexer->offset, 5);
//     ENFORCE_EQ(lexer->loc->col, 6);
//     ENFORCE_EQ(lexer->loc->line, 1);

//     // Go ahead 3 chars
//     char i = lexer_advancen(lexer, 3); // 'i'
//     ENFORCE_EQ(i, 'i');
//     ENFORCE_EQ(vec_cap(lexer->toklist), TOKENLIST_ALLOC_CAPACITY);
//     ENFORCE_EQ(vec_size(lexer->toklist), 0);
//     ENFORCE_EQ(lexer->offset, 8);
//     ENFORCE_EQ(lexer->loc->col, 9);
//     ENFORCE_EQ(lexer->loc->line, 1);

//     // Go ahead 7 chars
//     char p = lexer_advancen(lexer, 7); // 'p'
//     ENFORCE_EQ(p, 'p');
//     ENFORCE_EQ(vec_cap(lexer->toklist), TOKENLIST_ALLOC_CAPACITY);
//     ENFORCE_EQ(vec_size(lexer->toklist), 0);
//     ENFORCE_EQ(lexer->offset, 15);
//     ENFORCE_EQ(lexer->loc->col, 16);
//     ENFORCE_EQ(lexer->loc->line, 1);
    
//     // Go ahead 10 chars
//     char z = lexer_advancen(lexer, 10); // 'z'
//     ENFORCE_EQ(z, 'z');
//     ENFORCE_EQ(vec_cap(lexer->toklist), TOKENLIST_ALLOC_CAPACITY);
//     ENFORCE_EQ(vec_size(lexer->toklist), 0);
//     ENFORCE_EQ(lexer->offset, 25);
//     ENFORCE_EQ(lexer->loc->col, 26);
//     ENFORCE_EQ(lexer->loc->line, 1);

//     // Go ahead 10 chars
//     char nine = lexer_advancen(lexer, 10); // '9'
//     ENFORCE_EQ(nine, '9');
//     ENFORCE_EQ(vec_cap(lexer->toklist), TOKENLIST_ALLOC_CAPACITY);
//     ENFORCE_EQ(vec_size(lexer->toklist), 0);
//     ENFORCE_EQ(lexer->offset, 35);
//     ENFORCE_EQ(lexer->loc->col, 36);
//     ENFORCE_EQ(lexer->loc->line, 1);

//     // Go ahead 1 char (end of buff cap)
//     char eof1 = lexer_advancen(lexer, 1);
//     ENFORCE_EQ(eof1, nullchar);
//     // Options should remain the same
//     ENFORCE_EQ(vec_cap(lexer->toklist), TOKENLIST_ALLOC_CAPACITY);
//     ENFORCE_EQ(vec_size(lexer->toklist), 0);
//     ENFORCE_EQ(lexer->offset, 35);
//     ENFORCE_EQ(lexer->loc->col, 36);
//     ENFORCE_EQ(lexer->loc->line, 1);

//     // Go ahead 4 more chars (end of cap)
//     char eof2 = lexer_advancen(lexer, 4);
//     ENFORCE_EQ(eof2, nullchar);
//     // Options should remain the same
//     ENFORCE_EQ(vec_cap(lexer->toklist), TOKENLIST_ALLOC_CAPACITY);
//     ENFORCE_EQ(vec_size(lexer->toklist), 0);
//     ENFORCE_EQ(lexer->offset, 35);
//     ENFORCE_EQ(lexer->loc->col, 36);
//     ENFORCE_EQ(lexer->loc->line, 1);
// }

// #define lt  lexer->toklist

// TEST(lexer, lex_keywords) {
//     char* buffer = "atomic UInt32 var = 0x123;";
//     Lexer* lexer = lexer_init(buffer, null);
//     Token* tok;
    
//     lexer_lex(lexer);

//     // Test
//     tok = lt->at(lt, 0);
//     ENFORCE(tok->kind == ATOMIC);
//     ENFORCE_EQ(tok->offset, 0);
//     ENFORCE_EQ(tok->line, 1);
//     ENFORCE_EQ(tok->col, 1);
//     ENFORCE_STREQ(tok->value, "atomic");
//     ENFORCE_STREQ(tok->fname, "");
    
//     tok = lt->at(lt, 1);
//     ENFORCE(tok->kind == IDENTIFIER);
//     ENFORCE_EQ(tok->offset, 7);
//     ENFORCE_EQ(tok->line, 1);
//     ENFORCE_EQ(tok->col, 8);
//     ENFORCE_STREQ(tok->value, "UInt32");
//     ENFORCE_STREQ(tok->fname, "");

//     tok = lt->at(lt, 2);
//     ENFORCE(tok->kind == IDENTIFIER);
//     ENFORCE_EQ(tok->offset, 14);
//     ENFORCE_EQ(tok->line, 1);
//     ENFORCE_EQ(tok->col, 15);
//     ENFORCE_STREQ(tok->value, "var");
//     ENFORCE_STREQ(tok->fname, "");

//     tok = lt->at(lt, 3);
//     ENFORCE(tok->kind == EQUALS);
//     ENFORCE_EQ(tok->offset, 18);
//     ENFORCE_EQ(tok->line, 1);
//     ENFORCE_EQ(tok->col, 19);
//     ENFORCE_STREQ(tok->value, "=");
//     ENFORCE_STREQ(tok->fname, "");

//     tok = lt->at(lt, 4);
//     ENFORCE(tok->kind == HEX_INT);
//     ENFORCE_EQ(tok->offset, 20);
//     ENFORCE_EQ(tok->line, 1);
//     ENFORCE_EQ(tok->col, 21);
//     ENFORCE_STREQ(tok->value, "0x123");
//     ENFORCE_STREQ(tok->fname, "");

//     tok = lt->at(lt, 5);
//     ENFORCE(tok->kind == SEMICOLON);
//     ENFORCE_EQ(tok->offset, 25);
//     ENFORCE_EQ(tok->line, 1);
//     ENFORCE_EQ(tok->col, 26);
//     ENFORCE_STREQ(tok->value, ";");
//     ENFORCE_STREQ(tok->fname, "");

//     tok = lt->at(lt, 6);
//     ENFORCE(tok->kind == TOK_EOF);
//     ENFORCE_STREQ(tok->value, "EOF");
// }

// // TEST(Lexer, lexer_lex_digits) {
// //     char* buffer = "car = 0b1010101\nvar = 0xDeadBeef\nbar = 0o72626457263\nhar = 0b111111111111111111\nlar = 0x28300293";
// //     int nbin_digits = 15;
// //     Lexer* lexer = lexer_init(buffer, null);

// //     // LexerTestArr
// //     LexerTestArr* lta = LexerTestArr_alloc(nbin_digits);
// //     LexerTestArr_append(lta, IDENTIFIER, 0, "car", 1, 1, "");
// //     LexerTestArr_append(lta, EQUALS,     4, "=", 1, 5, "");
// //     LexerTestArr_append(lta, BIN_INT,    6, "0b1010101", 1, 7, "");
// //     LexerTestArr_append(lta, IDENTIFIER, 16, "var", 2, 1, "");
// //     LexerTestArr_append(lta, EQUALS,     20, "=", 2, 1, "");
// //     LexerTestArr_append(lta, HEX_INT,    22, "0xDeadBeef", 2, 1, "");
// //     LexerTestArr_append(lta, IDENTIFIER, 33, "bar", 4, 1, "");
// //     LexerTestArr_append(lta, EQUALS,     37, "=", 4, 1, "");
// //     LexerTestArr_append(lta, BIN_INT,    39, "0b111111111111111111", 4, 1, "");
// //     LexerTestArr_append(lta, IDENTIFIER, 53, "har", 3, 1, "");
// //     LexerTestArr_append(lta, EQUALS,     57, "=", 3, 1, "");
// //     LexerTestArr_append(lta, OCT_INT,    59, "0o72626457263", 3, 1, "");
// //     LexerTestArr_append(lta, IDENTIFIER, 80, "lar", 5, 1, "");
// //     LexerTestArr_append(lta, EQUALS,     84, "=", 5, 1, "");
// //     LexerTestArr_append(lta, HEX_INT,    86, "0x28300293", 5, 1, "");

// //     // Call lexer_lex()
// //     lexer_lex(lexer);

// //     ENFORCE_EQ(vec_size(lexer->toklist), nbin_digits + 1);
// //     ENFORCE_EQ(lta->cap, nbin_digits);

// //     for(int i = 0; i<nbin_digits; i++) {
// //         ENFORCE(lexer->toklist->kind == lta->token->kind);
// //         // ENFORCE_EQ(lexer->toklist->offset, lta->token->offset);
// //         // ENFORCE_EQ(lexer->toklist->line, lta->token->line);
// //         // ENFORCE_EQ(lexer->toklist->col, lta->token->col);
// //         ENFORCE_STREQ(lexer->toklist->value, lta->token->value);
// //         ENFORCE_STREQ(lexer->toklist->fname, lta->token->fname);
// //     }

// //     lexer_free(lexer);
// //     LexerTestArr_free(lta);
// // }