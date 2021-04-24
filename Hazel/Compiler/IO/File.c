#include <stdio.h>
#include <Hazel/Core/HCore.h>>
#include <Hazel/Compiler/IO/File.h>


// Read the contents of <fname> 
char* readFile(const char* fname) {
    FILE* file = fopen(fname, "rb"); 
    
    if(!file) { 
        printf("Could not open file: <%s>\n", fname);
        exit(2);
    }

    // Get the length of the input buffer
    fseek(file, 0, SEEK_END); 
    long buf_length = ftell(file); 
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(sizeof(char) * (buf_length + 1) );
    if(!buffer) {
        printf("Could not allocate memory for buffer for file at %s\n", fname);
		exit(1);
    }

    fread(buffer, 1, buf_length, file); 
    buffer[buf_length] = nullchar; // null char 
    fclose(file); 

    return buffer;
}