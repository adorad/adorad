cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(
    Hazel 
    DESCRIPTION "The Hazel Programming Language"
    VERSION 1.0.0
    LANGUAGES C
)

    
# Use compiler ID "AppleClang" instead of "Clang" for XCode.
# Not setting this sometimes makes XCode C compiler gets detected as "Clang",
# even when the C++ one is detected as "AppleClang".
cmake_policy(SET CMP0010 NEW)
cmake_policy(SET CMP0025 NEW)

# ------ Disable CMAKE_INSTALL_MESSAGE  ------
set(CMAKE_INSTALL_MESSAGE NEVER)

# ------ Check and set CMAKE_CXX_STANDARD ------
string(FIND "${CMAKE_C_FLAGS}" "-std=c" ENV_CXX_STANDARD)
if(ENV_CXX_STANDARD GREATER -1)
    message (
        WARNING "C Standard Version definition detected in an environment variable."
        "Hazel requires at least-std=c11. Please remove -std=c settings in your environment."
    )
endif()

# ------ Setting the C Standard ------
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

# Disable in-source builds to prevent corruption of the source tree 
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message(
        FATAL_ERROR "FATAL: In-source builds are not allowed. 
        You should create a separate directory for build files."
    )
endif()

# Break in case of popular CMake configuration mistakes
if(NOT CMAKE_SIZEOF_VOID_P GREATER 0)
    message(FATAL_ERROR "CMake failed to determine the bitness of the target platform. Check your CMake and compiler        
            installation. If you are cross-compiling, ensure that your CMake toolchain file correctly sets the 
            compiler details")
endif()


option(HAZEL_BUILD_TESTS "Build Hazel test binaries" ON)
option(HAZEL_BUILD_STATIC_LIB "Build Hazel Static Library " OFF)
option(HAZEL_BUILD_SHARED_LIB "Build Hazel Shared Library " OFF)
option(BUILD_DOCS "Build Hazel documentation" OFF)

if(HAZEL_BUILD_TESTS)
    # We need at least a Static Library to build and link with Hazel's Internal Tests
    if(NOT HAZEL_BUILD_STATIC_LIB)
        set(HAZEL_BUILD_STATIC_LIB ON)
    endif()
endif()

# ------ CMake build directories ------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bin)


# ------Compiler and linker options ------
# In case of Makefiles, if the user does not setup CMAKE_BUILD_TYPE, assume it's Release: 
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Build type not set - defaulting to Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug, Release,
    RelWithDebInfo, or MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)


# ------ A List of Compiler Flags ------
# A (more or less comprehensive) list is here: https://caiorss.github.io/C-Cpp-Notes/compiler-flags-options.html
if(NOT MSVC)
    if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
        string(APPEND CMAKE_C_FLAGS " -std=gnu11")
    else()
        string(APPEND CMAKE_C_FLAGS " -std=c11")
    endif()
    # Optimizations
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        string(APPEND CMAKE_C_FLAGS " -O2")
    endif()

    if (NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
        string(APPEND CMAKE_C_FLAGS " -fPIC")
    endif()

    string(APPEND CMAKE_C_FLAGS " -Wall")
    string(APPEND CMAKE_C_FLAGS " -Wextra")
    string(APPEND CMAKE_C_FLAGS " -Wno-unknown-pragmas")
    string(APPEND CMAKE_C_FLAGS " -Wno-sign-compare")
    string(APPEND CMAKE_C_FLAGS " -Wno-unused-parameter")
    string(APPEND CMAKE_C_FLAGS " -Wno-unused-variable")
    string(APPEND CMAKE_C_FLAGS " -Wno-unused-function")
    string(APPEND CMAKE_C_FLAGS " -Wno-unused-result")
    string(APPEND CMAKE_C_FLAGS " -Wno-unused-local-typedefs")
    string(APPEND CMAKE_C_FLAGS " -Wno-discarded-qualifiers")
    string(APPEND CMAKE_C_FLAGS " -Wno-multistatement-macros")
    string(APPEND CMAKE_C_FLAGS " -Wno-strict-overflow")
    string(APPEND CMAKE_C_FLAGS " -Wno-strict-aliasing")
    string(APPEND CMAKE_C_FLAGS " -Werror=return-type")
    string(APPEND CMAKE_C_FLAGS " -Wno-error=deprecated-declarations")

    if(WERROR)
        check_cxx_compiler_flag("-Werror" COMPILER_SUPPORT_WERROR)
        if(NOT COMPILER_SUPPORT_WERROR)
            set(WERROR FALSE)
        else()
            string(APPEND CMAKE_C_FLAGS " -Werror")   
        endif()
    endif(WERROR)

    if(NOT APPLE)
        string(APPEND CMAKE_C_FLAGS " -Wno-unused-const-variable")
        string(APPEND CMAKE_C_FLAGS " -Wno-uninitialized")
    else()
        # Disables the ``implicit declaration of function is invalid in C99`` warning 
        # This error is irritating even though we explicitely tell CMake to build according to the C11 standard
        string(APPEND CMAKE_C_FLAGS " -Wno-implicit-function-declaration") 
    endif()

# For MSVC 
else()
    # Optimizations
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        string(APPEND CMAKE_C_FLAGS " /O2")
    endif()
    string(APPEND CMAKE_C_FLAGS " /Wall")
    # string(APPEND CMAKE_C_FLAGS " /WX")  # Treats Linker Warnings as Errors
    string(APPEND CMAKE_C_FLAGS " /wd4514")
    string(APPEND CMAKE_C_FLAGS " /wd5045")

    # Ignore some MSVC warnings
    # 1. C4061: enumerator in switch os enum is not explicitly handled by a case label
    # 2. C4100: argv & argc: unreferenced formal parameter
    # 3. C4710: printf: function not inlined
    # 4: C4711: function selected for automatic inline expansion
    # 5. C5105: macro expansion producing `defined` has undefined behaviour
    # NOTE: For some reason (maybe I'm looking at this the wrong way, `/IGNORE` does not seems to work!!)
    # string(APPEND CMAKE_C_FLAGS " /IGNORE:4061,4100,4710,4711")
    string(APPEND CMAKE_C_FLAGS " /wd4061")
    string(APPEND CMAKE_C_FLAGS " /wd4100")
    string(APPEND CMAKE_C_FLAGS " /wd4710")
    string(APPEND CMAKE_C_FLAGS " /wd4711")
    string(APPEND CMAKE_C_FLAGS " /wd5105")
endif()

# A shared library is always produced if a library type is not passed to the add_library command.
# If this is off, a Static Library will be built by default
set(BUILD_SHARED_LIBS OFF)

# ------ Main Build ------
add_subdirectory(Hazel)

if(HAZEL_BUILD_TESTS)
    add_subdirectory(test)
endif()
