/*
_ _    _           ______   _______        
| |  | |    /\    /___  /   |  ____|| |    
| |__| |   /  \      / /    | |__   | |       Hazel - The Fast, Expressive & Elegant Programming Language
|  __  |  / /\ \    / /     |  __|  | |       Languages: C, C++, and Assembly
| |  | | / ____ \  / /___   | |____ | |____   https://github.com/HazelLang/hazel/
|_|_ |_|/_/    \_\/_______\ |______|_\______|

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/

#ifndef _CSTL_IO_H
#define _CSTL_IO_H

#include <stdio.h>
#include <stdlib.h>

// C-version
char* readFile(const char* fname) {
    FILE* file = fopen(fname, "rb"); 
    
    if(!file)
        printf("Could not open file: <%s>\n", fname);

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
    buffer[buf_length] = nullchar;
    fclose(file); 

    return buffer;
}

#endif // _CSTL_IO_H