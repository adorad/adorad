#include <stdio.h>
#include <Hazel/Core/HCore.h>>
#include <Hazel/Compiler/IO/File.h>


// Read the contents of <fname> 
char* readFile(const char* fname) {
    char* buffer = 0; 
    long length = 0; 
    FILE* file = fopen(fname, 'rb'); 
    
    if(!file) { 
        fprintf(stderr, "ERROR READING FILE %s", file);
        exit(2);
    }

    // Get the length of the input buffer
    fseek(file, 0, SEEK_END); 
    length = ftell(file); 
    fseek(file, 0, SEEK_SET);

    buffer = (char*)malloc(sizeof(char) * (length + 1) );

    if(buffer) {
        fread(buffer, 1, length, file); 
    }

    fclose(file); 
    buffer[length] = nullchar; // null char 

    return buffer;
}