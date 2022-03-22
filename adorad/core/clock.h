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

#ifndef CORETEN_CLOCK_H
#define CORETEN_CLOCK_H

#include <time.h>

double clock_now();
double clock_duration(clock_t start, clock_t end);

#ifdef CORETEN_IMPL
    #include <adorad/core/misc.h>
    
    // Returns the current time (in clock_t)
    double clock_now() {
        return cast(double)clock();
    }

    // Get duration between `start` && `end` in seconds.
    double clock_duration(clock_t start, clock_t end) {
        return cast(double)(end - start)/CLOCKS_PER_SEC;
    }
#endif // CORETEN_IMPL

#endif // CORETEN_CLOCK_H