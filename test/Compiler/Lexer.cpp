// Simple tests 
void test_Lexer_init() {
    std::string buffer = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    Hazel::Lexer lexer(buffer);

    CSTL_ASSERT(lexer.get_buffer() == "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    CSTL_ASSERT(lexer.get_buffer_length() == 35);
}