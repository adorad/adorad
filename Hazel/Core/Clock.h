/*
_ _    _           ______   _______        
| |  | |    /\    /___  /   |  ____|| |    
| |__| |   /  \      / /    | |__   | |       Hazel - The Fast, Expressive & Elegant Programming Language
|  __  |  / /\ \    / /     |  __|  | |       Languages: C, C++, and Assembly
| |  | | / ____ \  / /___   | |____ | |____   https://github.com/HazelLang/Hazel/
|_|_ |_|/_/    \_\/_______\ |______|_\______|

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/

#ifndef CSTL_CLOCK_H
#define CSTL_CLOCK_H

#include <Hazel/Core/Misc.h>
#include <Hazel/Core/Types.h>

#ifdef __cplusplus
namespace Hazel {
extern "C" {
#endif

// Time ==========================================
CSTL_DEF UInt64  gb_rdtsc        (void);
CSTL_DEF Float64  gb_time_now    (void); // This is only for relative time e.g. game loops
CSTL_DEF UInt64  gb_utc_time_now (void); // Number of microseconds since 1601-01-01 UTC
CSTL_DEF void gb_sleep_ms        (UInt32 ms);

#ifdef __cplusplus
} // extern "C"
} // namespace Hazel
#endif

#endif // CSTL_CLOCK_H