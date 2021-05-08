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

#ifndef _CSTL_CORE_CSTL_H
#define _CSTL_CORE_CSTL_H

/** 
    Jason Dsouza's (jasmcaus) C Helper Library 
    Available in the public domain
    Use at your own risk - no warranty implied

    This is a single header file that replaces (and makes modern) a bunch of useful stuff from 
    the C/C++ Standard Library

LICENSE
	This software is dual-licensed to the public domain and under the following license: you are 
    granted a perpetual, irrevocable license to copy, modify, 	publish, and distribute this file 
    as you see fit.

WARNING
	- This library is **slightly** experimental and features may not work as expected.
	- This also means that many functions are not documented.
CREDITS
	Written Jason Dsouza
*/

/*
    NOTE: Do not include this file multiple times (in separate source files)
    Since this is a header-only library (with no implementation source file), each time you include this file, 
    methods are individually built into their separate object files.
    Thus, you will end up with the "Multiple definition of `function`" error. 

    If you need to use an individual file, either include this file (HCore.h) _once_ or pick and choose what you need - 
    being explicit never hurts. 

    For the development of the Hazel Language, it is required (otherwise you'll run into compilation problems such as the 
    one above) you choose the latter option. 
    
    Reminder: Be Explicit. 
*/

#ifndef CSTL_USING_CUSTOM_GENERATED_MACROS
    #include <Hazel/Core/CMake_Macros.h>
#endif // CSTL_USING_CUSTOM_GENERATED_MACROS

#include <Hazel/Core/Clock.h>
#include <Hazel/Core/Compilers.h>
#include <Hazel/Core/CPUs.h>
#include <Hazel/Core/Debug.h>
#include <Hazel/Core/Exception.h>
#include <Hazel/Core/Headers.h>
#include <Hazel/Core/IO.h>
#include <Hazel/Core/Memory.h>
#include <Hazel/Core/Misc.h>
#include <Hazel/Core/OS.h>
#include <Hazel/Core/String.h>
#include <Hazel/Core/Types.h>

#ifdef CSTL_USE_ARROW_UTEST
    #include <Hazel/Core/Arrow.h>
#endif // CSTL_USE_ARROW_UTEST

#endif // _CSTL_CORE_CSTL_H