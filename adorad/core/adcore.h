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

#ifndef CORETEN_CORE_CORETEN_H
#define CORETEN_CORE_CORETEN_H

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
	Written by Jason Dsouza <@jasmcaus>
*/

/*
    NOTE: Do not include this file multiple times (in separate source files)
    Since this is a header-only library (with no implementation source file), each time you include this file, 
    methods are individually built into their separate object files.
    Thus, you will end up with the "Multiple definition of `function`" error. 

    If you need to use an individual file, either include this file (adcore.h) _once_ or pick and choose what you need - 
    being explicit never hurts. 

    For the development of the Adorad Language, it is required (otherwise you'll run into compilation problems such as the 
    one above) you choose the latter option. 
    
    Reminder: Be Explicit. 
*/

#ifndef CORETEN_USING_CUSTOM_GENERATED_MACROS
    #include <adorad/core/cmake_macros.h>
#endif // CORETEN_USING_CUSTOM_GENERATED_MACROS

#include <adorad/core/os_defs.h>
#include <adorad/core/cpu.h>
#include <adorad/core/compilers.h>
#include <adorad/core/types.h>
#include <adorad/core/headers.h>
#include <adorad/core/clock.h>
#include <adorad/core/debug.h>
#include <adorad/core/misc.h>
#include <adorad/core/io.h>
#include <adorad/core/memory.h>
#include <adorad/core/math.h>
#include <adorad/core/os.h>
#include <adorad/core/buffer.h>
#include <adorad/core/buffview.h>
#include <adorad/core/char.h>
#include <adorad/core/utf8.h>
#include <adorad/core/vector.h>
#include <adorad/core/warnings.h>

#ifdef CORETEN_INCLUDE_HASH_H
    #include <adorad/core/hash.h>
#endif // CORETEN_INCLUDE_HASH_H

#ifdef CORETEN_INCLUDE_WINDOWS_H
    // #include <adorad/core/windows.h>
#endif // CORETEN_INCLUDE_WINDOWS_H

#endif // CORETEN_CORE_CORETEN_H