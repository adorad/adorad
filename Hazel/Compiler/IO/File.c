#include <stdio.h>
#include <Hazel/Core/HCore.h>>
#include <Hazel/Compiler/IO/File.h>

// Read the contents of <fname> 
char* readFile(const char* fname) {
    char* buffer = 0; 
    long length = 0; 
    FILE* file = fopen(fname, 'rb'); 
    
    if(file) {
        // Get the length of the input buffer
        fseek(file, 0, SEEK_END); 
        length = ftell(file); 
        fseek(file, 0, SEEK_SET);

        buffer = (char*)malloc(sizeof(char) * (length + 1) );

        if(buffer) {
            fread(buffer, cstl_sizeof(char), length, file); 
        }

        fclose(file); 
        return buffer;
    }

    // Else 
    printf("ERROR READING FILE %s", file);
    exit(2);

}