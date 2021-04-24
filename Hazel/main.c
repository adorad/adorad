#include <stdio.h> 

#include <Hazel/Hazel.h>

int main(int argc, char* arv[]) {
    bool interactive_mode = false; 

    if(argc < 2) {
        interactive_mode = true; 
        while(interactive_mode) {
            char* str = getStdIn("> "); 
            Lexer* lexer = lexer_init(str); 
        }
    }
    

}
