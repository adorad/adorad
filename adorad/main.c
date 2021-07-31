#include <adorad/adorad.h>

static void usage(int status) {
    fprintf(stderr, "Usage: adorad [ -c ] <file>\n");
    exit(status);
}

int main(int argc, char** argv) {
    // C - Example: 
    // To compile a Adorad source file:
    // >> adorad compile hello.ad
    // if(argc<3) {
    //     printf("Too few arguments"); 
    //     return 1;
    // }

    // puts("    Adorad Language 0.0.1 (May 22 2021 02:39:23)");
    // puts("    GCC version: 9.3.0 on Windows");
    // puts("    All engines are a go!");

    // while(true) {
    //     printf("adorad> ");
    //     char buffer[20];
    //     fgets(buffer, 20, stdin);
    //     printf("%s", buffer);
    //     // printf("\n");
    // }
    return 0; 
}