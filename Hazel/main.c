#include <Hazel/Hazel.h>
// #include <string>
// #include <iostream>

UTEST_MAIN()

UTEST(c, ASSERT_TRUE) { ASSERT_TRUE(1); }

UTEST(c, ASSERT_FALSE) { ASSERT_FALSE(0); }

UTEST(c, ASSERT_EQ) { ASSERT_EQ(1, 1); }

UTEST(c, ASSERT_NE) { ASSERT_NE(1, 2); }

UTEST(c, ASSERT_LT) { ASSERT_LT(1, 2); } 

UTEST(c, ASSERT_LE) {
  ASSERT_LE(1, 1);
  ASSERT_LE(1, 2);
}

UTEST(c, ASSERT_GT) { ASSERT_GT(2, 1); }

UTEST(c, ASSERT_GE) {
  ASSERT_GE(1, 1);
  ASSERT_GE(2, 1);
}

UTEST(c, ASSERT_STREQ) { ASSERT_STREQ("foo", "foo"); }

UTEST(c, ASSERT_STRNE) { ASSERT_STRNE("foo", "bar"); }

UTEST(c, ASSERT_STRNEQ) { ASSERT_STRNEQ("foo", "foobar", strlen("foo")); }

UTEST(c, ASSERT_STRNNE) { ASSERT_STRNNE("foo", "barfoo", strlen("foo")); }

UTEST(c, EXPECT_TRUE) { EXPECT_TRUE(1); }

UTEST(c, EXPECT_FALSE) { EXPECT_FALSE(0); }

UTEST(c, EXPECT_EQ) { EXPECT_EQ(1, 1); }

UTEST(c, EXPECT_NE) { EXPECT_NE(1, 2); }

UTEST(c, EXPECT_LT) { EXPECT_LT(1, 2); }

UTEST(c, EXPECT_LE) {
  EXPECT_LE(1, 1);
  EXPECT_LE(1, 2);
}

UTEST(c, EXPECT_GT) { EXPECT_GT(2, 1); }

UTEST(c, EXPECT_GE) {
  EXPECT_GE(1, 1);
  EXPECT_GE(2, 1);
}

UTEST(c, EXPECT_STREQ) { EXPECT_STREQ("foo", "foo"); }

UTEST(c, EXPECT_STRNE) { EXPECT_STRNE("foo", "bar"); }

UTEST(c, EXPECT_STRNEQ) { EXPECT_STRNEQ("foo", "foobar", strlen("foo")); }

UTEST(c, EXPECT_STRNNE) { EXPECT_STRNNE("foo", "barfoo", strlen("foo")); }

// UTEST(foo, bar) {
//     ASSERT_TRUE(1);
//     ASSERT_TRUE(0);
//     CHECK_TRUE(1 == 1);
//     CHECK_TRUE(2 != 3);
//     ASSERT_TRUE(1 == 1);
//     ASSERT_TRUE(2 != 3);
// }

// UTEST(foo, b) {
//     ASSERT_TRUE(1);
//     ASSERT_TRUE(1);
//     ASSERT_TRUE(1 == 1);
//     ASSERT_TRUE(2 != 3);
// }
 


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
