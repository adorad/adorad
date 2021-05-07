// Simple tests 
void test_Lexer_init() {
    const char* buffer = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    Hazel::Lexer lexer(buffer);

    CSTL_ASSERT(lexer.buffer() == "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    CSTL_ASSERT(lexer.buffer_length() == 35);
}