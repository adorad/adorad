#include <Hazel/Hazel.h>
// #include <string>
// #include <iostream>

CSTL_MAIN()

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

TEST(c, EXPECT_TRUE) { EXPECT_TRUE(1); }

TEST(c, EXPECT_FALSE) { EXPECT_FALSE(0); }

TEST(c, EXPECT_EQ) { EXPECT_EQ(1, 1); }

TEST(c, EXPECT_NE) { EXPECT_NE(1, 2); }

TEST(c, EXPECT_LT) { EXPECT_LT(1, 2); }

TEST(c, EXPECT_LE) {
  EXPECT_LE(1, 1);
  EXPECT_LE(1, 2);
}

TEST(c, EXPECT_GT) { EXPECT_GT(2, 1); }

TEST(c, EXPECT_GE) {
  EXPECT_GE(1, 1);
  EXPECT_GE(2, 1);
}

TEST(c, EXPECT_STREQ) { EXPECT_STREQ("foo", "foo"); }

TEST(c, EXPECT_STRNE) { EXPECT_STRNE("foo", "bar"); }

TEST(c, EXPECT_STRNEQ) { EXPECT_STRNEQ("foo", "foobar", strlen("foo")); }

TEST(c, EXPECT_STRNNE) { EXPECT_STRNNE("foo", "barfoo", strlen("foo")); }

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
