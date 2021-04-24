#include <stdio.h> 
#include <Hazel/Hazel.h>

int main(int argc, char* argv[]) {
    // bool interactive_mode = false; 
    // if(argc < 2) {
    //     interactive_mode = true; 
    //     while(interactive_mode) {
    //         char* str = getStdIn("> "); 
    //         Lexer* lexer = lexer_init(str); 
    //     }
    // }
    
    char* buffer = readFile("Hazel.h"); 
    if(buffer) {
        printf("BUFFER AVAILABLE");
    }
    free(buffer);
}
