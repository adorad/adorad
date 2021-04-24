#include <stdio.h> 
#include <string.h>
#include <Hazel/Hazel.h>

int main(int argc, char* argv[]) {
    // Example: 
    // To compile a Hazel source file:
    // >> hazel compile hello.hzl
    if(argc<3) {
        printf("Too few arguments"); 
        return 1;
    }

    if(strcmp(argv[1], "compile") == 0) {
        char* source = readFile(argv[2]);
        
        // Delete when done 
        free(source); 
    }
    // char* source = readFile("test/LexerDemo.hzl");
    // printf(source);
    // free(source);

    return 0; 
}
