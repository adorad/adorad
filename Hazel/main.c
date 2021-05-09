#include <Hazel/Hazel.h>
// #include <string>
// #include <iostream>

TEST(c, ASSERT_TRUE) { ASSERT_TRUE(1); }

TEST(c, ASSERT_FALSE) { ASSERT_FALSE(0); }

TEST(c, ASSERT_EQ) { ASSERT_EQ(1, 1); }

TEST(c, ASSERT_NE) { ASSERT_NE(1, 2); }

TEST(c, ASSERT_LT) { ASSERT_LT(1, 2); } 

TEST(c, ASSERT_LE) {
  ASSERT_LE(1, 1);
  ASSERT_LE(1, 2);
}

TEST(c, ASSERT_GT) { ASSERT_GT(2, 1); }

TEST(c, ASSERT_GE) {
  ASSERT_GE(1, 1);
  ASSERT_GE(2, 1);
}

TEST(c, ASSERT_STREQ) { ASSERT_STREQ("foo", "foo"); }

TEST(c, ASSERT_STRNE) { ASSERT_STRNE("foo", "bar"); }

TEST(c, ASSERT_STRNEQ) { ASSERT_STRNEQ("foo", "foobar", strlen("foo")); }

TEST(c, ASSERT_STRNNE) { ASSERT_STRNNE("foo", "barfoo", strlen("foo")); }

TEST(c, CHECK_TRUE) { CHECK_TRUE(1); }

TEST(c, CHECK_FALSE) { CHECK_FALSE(0); }

TEST(c, CHECK_EQ) { CHECK_EQ(1, 1); }

TEST(c, CHECK_NE) { CHECK_NE(1, 2); }

TEST(c, CHECK_LT) { CHECK_LT(1, 2); }

TEST(c, CHECK_LE) {
  CHECK_LE(1, 1);
  CHECK_LE(1, 2);
}

TEST(c, CHECK_GT) { CHECK_GT(2, 1); }

TEST(c, CHECK_GE) {
  CHECK_GE(1, 1);
  CHECK_GE(2, 1);
}

TEST(c, CHECK_STREQ) { CHECK_STREQ("foo", "foo"); }

TEST(c, CHECK_STRNE) { CHECK_STRNE("foo", "bar"); }

TEST(c, CHECK_STRNEQ) { CHECK_STRNEQ("foo", "foobar", strlen("foo")); }

TEST(c, CHECK_STRNNE) { CHECK_STRNNE("foo", "barfoo", strlen("foo")); }

TEST(foo, bar) {
    ASSERT_TRUE(1);
    ASSERT_TRUE(0);
    CHECK_TRUE(1 == 1);
    CHECK_TRUE(2 != 3);
    ASSERT_TRUE(1 == 1);
    ASSERT_TRUE(2 != 3);
}

TEST(foo, b) {
    ASSERT_TRUE(1);
    ASSERT_TRUE(1);
    ASSERT_TRUE(1 == 1);
    ASSERT_TRUE(2 != 3);
}
 

// int main(int argc, const char* const argv[]) {
//     // std::string buffer = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//     // Hazel::Lexer lexer(buffer);

//     // std::cout << lexer.buffer() << "\n";
//     // std::cout << lexer.buffer_capacity() << "\n";
//     // std::cout << lexer.next() << "\n";
//     // std::cout << lexer.peek_curr() << "\n";

//     // C - Example: 
//     // To compile a Hazel source file:
//     // >> hazel compile hello.hzl
//     // if(argc<3) {
//     //     printf("Too few arguments"); 
//     //     return 1;
//     // }

//     // if(strcmp(argv[1], "compile") == 0) {
//     // Lexer* lexer = calloc(1, sizeof(Lexer)); 
// 	// char* buffer = readFile("test/LexerDemo.hzl");
// 	// lexer_init(buffer, "jfjf"); 
    
//     // // Delete when done 
//     // lexer_free(lexer); 
//     // free(buffer);
//     // char* source = readFile("test/LexerDemo.hzl");
//     // printf(source);
//     // free(source);

//     return 0; 
// }
