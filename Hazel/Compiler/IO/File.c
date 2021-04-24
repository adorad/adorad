#include <stdio.h>
#include <Hazel/Core/HCore.h>>
#include <Hazel/Compiler/IO/File.h>

// Read the contents of <fname> 
char* readFile(const char* fname) {
    printf("INSIDE READFILE\n");
    char* buffer = 0; 
    long length = 0; 
    FILE* file = fopen(fname, 'rb'); 
    
    printf("FILE = %s\n", file);

    if(file) {
        printf("INSIDE IF\n");
        // Get the length of the input buffer
        fseek(file, 0, SEEK_END); 
        length = ftell(file); 
        fseek(file, 0, SEEK_SET);

        buffer = calloc(length, length+1); 

        if(buffer) {
            fread(buffer, cstl_sizeof(char), length, file); 
        }

        fclose(file); 
        return buffer;
    }
    printf("INSIDE READFILE\n");
    // Else 
    printf("ERROR READING FILE %s", file);
    exit(2);

}