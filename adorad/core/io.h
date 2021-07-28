/*
          _____         _____           _____  
    /\   |  __ \ / __ \|  __ \    /\   |  __ \
   /  \  | |  | | |  | | |__)|   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /  / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \ / ____ \| |__| | https://github.com/adorad/adorad/
/_/_ __\_\_____/_\____/|_|__\_/_/_ __\_\_____/

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <@jasmcaus>
*/

#ifndef _CSTL_IO_H
#define _CSTL_IO_H

#include <stdio.h>
#include <stdlib.h>

char* readFile(const char* fname) {
    FILE* file = fopen(fname, "rb"); 
    
    if(!file) {
        printf("Could not open file: <%s>\n", fname);
        exit(1);
    }

    // Get the length of the input buffer
    fseek(file, 0, SEEK_END); 
    long buff_length = ftell(file); 
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(sizeof(char) * (buff_length + 1) );
    if(!buffer) {
        printf("Could not allocate memory for buffer for file at %s\n", fname);
		exit(1);
    }

    fread(buffer, 1, buff_length, file); 
    buffer[buff_length] = nullchar;
    fclose(file); 

    return buffer;
}

#endif // _CSTL_IO_H