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
#ifndef ADORAD_LOCATION_H
#define ADORAD_LOCATION_H

#include <adorad/core/types.h>
#include <adorad/core/buffer.h>

typedef struct Location Location;

struct Location {
    UInt32 line;
    UInt32 col;
    Buff* fname;
};

Location* loc_new(const char* fname);
void loc_reset(Location* loc);
void loc_free(Location* loc);

#endif // ADORAD_LOCATION_H