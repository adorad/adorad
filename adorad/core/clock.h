/*
_ _    _           ______   _______        
| |  | |    /\    /___  /   |  ____|| |    
| |__| |   /  \      / /    | |__   | |       Adorad - The Fast, Expressive & Elegant Programming Language
|  __  |  / /\ \    / /     |  __|  | |       Languages: C, C++, and Assembly
| |  | | / ____ \  / /___   | |____ | |____   https://github.com/AdoradLang/adorad/
|_|_ |_|/_/    \_\/_______\ |______|_\______|

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
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