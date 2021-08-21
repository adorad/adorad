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

/* 
    Defines the common scaffolding for exported symbols.
    
    Export is by itself a quite tricky situation to deal with, somake sure you start with the background here:
        - Linux: https://gcc.gnu.org/wiki/Visibility
        - Windows: https://docs.microsoft.com/en-us/cpp/cpp/dllexport-dllimport?view=vs-2017
    Do NOT include this file directly. Instead, use CSTL/macros/Macros.h
 */

/*
    You do not need to edit this part of file unless you are changing the core CSTL export abstractions.

    This part defines the CSTL core export and import macros. This is controlled by whether we are building 
    shared libraries or not, which is determined during build time and codified in CSTL/core/cmake_macros.h.
    When the library is built as a shared lib, EXPORT and IMPORT will contain visibility attributes. 
    If it is being built as a static lib, then EXPORT and IMPORT basically have no effect.

    As a rule of thumb, you should almost NEVER mix static and shared builds for libraries that depend 
    on CSTL. 
    Also, if CSTL is built as a static library, we recommend everything dependent on CSTL to be built 
    statically. If CSTL is built as a shared library, everything dependent on it should be built as shared. 

    In the Adorad project, all native libraries shall use the macro CORETEN_BUILD_SHARED_LIB to check whether 
    Adorad is building shared or static libraries.
*/

#if defined(_WIN32)
	#define CORETEN_DLL_EXPORT     CORETEN_EXTERN __declspec(dllexport)
	#define CORETEN_DLL_IMPORT     CORETEN_EXTERN __declspec(dllimport)
#else
	#define CORETEN_DLL_EXPORT     CORETEN_EXTERN __attribute__((visibility("default")))
	#define CORETEN_DLL_IMPORT     CORETEN_EXTERN
#endif

#ifdef _WIN32
    #define CORETEN_HIDDEN
    #if defined(CORETEN_BUILD_SHARED_LIB)
        #define CORETEN_EXPORT      __declspec(dllexport)
        #define CORETEN_IMPORT      __declspec(dllimport)
    #else
        #define CORETEN_EXPORT
        #define CORETEN_IMPORT
    #endif

#else
    #ifdef __GNUC__
        #define CORETEN_EXPORT      __attribute__((__visibility__("default")))
        #define CORETEN_HIDDEN      __attribute__((__visibility__("hidden")))
    #else
        #define CORETEN_EXPORT
        #define CORETEN_HIDDEN
    #endif // __GNUC__

    #define CORETEN_IMPORT  CORETEN_EXPORT
#endif // _WIN32

#ifdef NO_EXPORT
    #undef CORETEN_EXPORT
    #define CORETEN_EXPORT
#endif

/*
    Definition of an adaptive XX_API macro, that depends on whether you are building the library itself or not, 
    routes to XX_EXPORT and XX_IMPORT. Basically, you will need to do this for each shared library that you are
    building, and the instruction is as follows: assuming that you are building a library called libawesome.so. 
    
    You should:
        1. for your cmake target (usually done by "add_library(awesome, ...)"), define a macro called 
           AWESOME_BUILD_MAIN_LIB using target_compile_options.
        2. define the AWESOME_API macro similar to the one below.
    
    And in the source file of your awesome library, use AWESOME_API to annotate public symbols.

    Here, for the CSTL library, we will define the macro CORETEN_API for both import and export.
*/

#ifdef CORETEN_BUILD_MAIN_LIB
    #define CORETEN_API    CORETEN_EXPORT
#else
    #define CORETEN_API    CORETEN_IMPORT
#endif