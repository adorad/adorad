#include <stdio.h> 

#include <Hazel/Hazel.h>

int main() {
    char* buffer = readFile("Hazel/Hazel.h");
    printf("WE ARE DONE HERE!");
    printf(buffer);
}
