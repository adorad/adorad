/*
          _____   ____  _____            _____  
    /\   |  __ \ / __ \|  __ \     /\   |  __ \ 
   /  \  | |  | | |  | | |__) |   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /   / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \  / ____ \| |__| | https://github.com/adorad/adorad/
/_/    \_\_____/ \____/|_|  \_\/_/    \_\_____/ 

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <@jasmcaus>
*/

#include <stdlib.h>
#include <adorad/compiler/location.h>

Location* loc_new(const char* fname) {
    Location* loc = (Location*)calloc(1, sizeof(Location));
    loc->line = 1;
    loc->col = 1;
    loc->fname = buff_new(fname);
    
    return loc;
}

void loc_reset(Location* loc) {
    loc->line = 1;
    loc->col = 1;
    loc->fname->data = "";
}

void loc_free(Location* loc) {
    if(loc) {
        buff_free(loc->fname);
        free(loc);
    }
}