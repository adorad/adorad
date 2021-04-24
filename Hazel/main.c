#include <stdio.h> 
#include <string.h>
#include <Hazel/Hazel.h>

int main(int argc, char* argv[]) {
    // // Example: 
    // // To compile a Hazel source file:
    // // >> hazel -c hello.hzl
    // if(argc<3) {
    //     printf("Too few arguments"); 
    //     return 1;
    // }

    // if(strcmp(argv[1], "-c") == 0) {
    //     char* source = readFile(argv[2]);
    // }
    char* source = readFile("Hazel/Hazel.h");
    printf(source);
    free(source);

    return 0; 
}
