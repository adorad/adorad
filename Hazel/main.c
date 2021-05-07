#include <Hazel/Hazel.h>

int main() {
    return 0;
}

// int main() {
//     const char* buffer = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//     Hazel::Lexer lexer(buffer);

//     std::cout << lexer.buffer() << "\n";
//     std::cout << lexer.buffer_capacity() << "\n";
//     std::cout << lexer.next() << "\n";
//     std::cout << lexer.peek_curr() << "\n";

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
