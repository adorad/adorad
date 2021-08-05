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

#ifndef CSTL_MEMORY_H
#define CSTL_MEMORY_H

#ifndef KB_TO_BYTES
    #define KB_TO_BYTES(x)               (x) * (Int64)(1024)
    #define MB_TO_BYTES(x)    KB_TO_BYTES(x) * (Int64)(1024)
    #define GB_TO_BYTES(x)    MB_TO_BYTES(x) * (Int64)(1024)
    #define TB_TO_BYTES(x)    GB_TO_BYTES(x) * (Int64)(1024)
#endif 

#define CSTL_ONES            cast(Ull)-1/UInt8_MAX
#define CSTL_HIGHS           CSTL_ONES * (UInt8_MAX/2+1)
#define CSTL_HAS_ZERO(x)     (x)-CSTL_ONES & ~(x) & CSTL_HIGHS

#endif // CSTL_MEMORY_H