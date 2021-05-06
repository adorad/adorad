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

#ifndef CSTL_USING_CUSTOM_GENERATED_MACROS
    #include <Hazel/Core/CMake_Macros.h>
#endif // CSTL_USING_CUSTOM_GENERATED_MACROS

#include <Hazel/Core/Clock.h>
#include <Hazel/Core/Compilers.h>
#include <Hazel/Core/Cpus.h>
#include <Hazel/Core/Debug.h>
#include <Hazel/Core/Exception.h>
#include <Hazel/Core/Headers.h>
#include <Hazel/Core/IO.h>
#include <Hazel/Core/Memory.h>
#include <Hazel/Core/Misc.h>
#include <Hazel/Core/OS.h>
#include <Hazel/Core/String.h>
#include <Hazel/Core/Types.h>

#endif // _CSTL_CORE_CSTL_H