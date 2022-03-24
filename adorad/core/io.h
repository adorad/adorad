/*
          _____   ____  _____            _____
    /\   |  __ \ / __ \|  __ \     /\   |  __ \
   /  \  | |  | | |  | | |__) |   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /   / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \  / ____ \| |__| | https://github.com/adorad/adorad/
/_/    \_\_____/ \____/|_|  \_\/_/    \_\_____/

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021-22 Jason Dsouza <@jasmcaus>
*/

#ifndef CORETEN_IO_H
#define CORETEN_IO_H

typedef struct File {
    char* full_path;
    char* basename;
    int fileid;

    char* contents;
} File;

char* read_file(const char* fname);
bool file_exists(const char* path);

#ifdef CORETEN_IMPL
    #include <sys/stat.h>

    char* read_file(const char* fname) {
        FILE* file = fopen(fname, "rb"); 
        
        if(NONE(file)) {
            cstl_colored_printf(CORETEN_COLOR_ERROR, "Could not open file: <%s>\n", fname);
            cstl_colored_printf(CORETEN_COLOR_ERROR, "%s\n", !file_exists(fname) ?  
                                "FileNotFoundError: File does not exist." : "");
            cstl_colored_printf(CORETEN_COLOR_ERROR, "Does file exist? %s\n", !file_exists(fname) ? "false" : "true");
            exit(1);
        }

        // Get the length of the input buffer
        fseek(file, 0, SEEK_END); 
        long buff_length = ftell(file); 
        fseek(file, 0, SEEK_SET);

        char* buffer = cast(char*)malloc(sizeof(char) * (buff_length + 1) );
        if(NONE(buffer)) {
            fprintf(stderr, "Could not allocate memory for buffer for file at %s\n", fname);
            exit(1);
        }

        fread(buffer, 1, buff_length, file); 
        buffer[buff_length] = nullchar;
        fclose(file); 

        return buffer;
    }

    bool file_exists(const char* path) {
    #ifdef WIN32
        if (GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES) return true;
    #else
        // if(access(path, 0) == 0) return true;
        struct stat st;
        return !stat(path, &st);
    #endif // WIN32
        return false;
    }

#endif // CORETEN_IMPL

#endif // CORETEN_IO_H