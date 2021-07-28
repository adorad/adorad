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

#ifndef CSTL_CLOCK_H
#define CSTL_CLOCK_H

#include <time.h>

// Returns the current time (in clock_t)
static double now() {
    return clock();
}

// Get duration between `start` and `end` in seconds.
static double duration(clock_t start, clock_t end) {
    return (double)(end - start)/CLOCKS_PER_SEC;
}

#endif // CSTL_CLOCK_H