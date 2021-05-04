cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(HazelMain CXX C)

# ------ Disable CMAKE_INSTALL_MESSAGE  ------
set(CMAKE_INSTALL_MESSAGE NEVER)

# ------ Check and set CMAKE_CXX_STANDARD ------
string(FIND "${CMAKE_CXX_FLAGS}" "-std=c++" ENV_CXX_STANDARD)
if(ENV_CXX_STANDARD GREATER -1)
    message (
        WARNING "C++ Standard Version definition detected in an environment variable."
        "Hazel requires -std=c++17. Please remove -std=c++ settings in your environment."
    )
endif()

# ------ Setting the CXX Standard ------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)


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


option(HAZEL_NO_TEST "Do not build Hazel test binaries" OFF)
option(BUILD_BINARY "Build the C++ binaries" OFF)
option(BUILD_DOCS "Build Hazel documentation" OFF)
option(USE_CUDA "Use CUDA" OFF) # TODO (turn ON)
option(USE_FAST_NVCC "Use parallel NVCC build" OFF)
option(WERROR "Build with -Werror supported by the compiler" OFF)

# ------ CMake build directories ------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# ------Compiler and linker options ------
# In case of Makefiles, if the user does not setup CMAKE_BUILD_TYPE, assume it's Release: 
if(CMAKE_GENERATOR MATCHES "Makefiles|Ninja" AND "${CMAKE_BUILD_TYPE}" STREQUAL "")
    message(STATUS "Build type not set - defaulting to Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build from: Debug Release." FORCE)
endif()

# ------ A List of Compiler Flags ------
# A (more or less comprehensive) list is here: https://caiorss.github.io/C-Cpp-Notes/compiler-flags-options.html
if(NOT MSVC)
    # Optimizations
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        string(APPEND CMAKE_CXX_FLAGS " -O2")
    endif()
    string(APPEND CMAKE_CXX_FLAGS " -Wall")
    string(APPEND CMAKE_CXX_FLAGS " -Wextra")
    string(APPEND CMAKE_CXX_FLAGS " -Werror=return-type")
    string(APPEND CMAKE_CXX_FLAGS " -Wno-unknown-pragmas")
    string(APPEND CMAKE_CXX_FLAGS " -Wno-sign-compare")
    string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-parameter")
    string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-variable")
    string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-function")
    string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-result")
    string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-local-typedefs")
    string(APPEND CMAKE_CXX_FLAGS " -Wno-strict-overflow")
    string(APPEND CMAKE_CXX_FLAGS " -Wno-strict-aliasing")
    string(APPEND CMAKE_CXX_FLAGS " -Wno-error=deprecated-declarations")

    if(WERROR)
        check_cxx_compiler_flag("-Werror" COMPILER_SUPPORT_WERROR)
        if(NOT COMPILER_SUPPORT_WERROR)
            set(WERROR FALSE)
        else()
            string(APPEND CMAKE_CXX_FLAGS " -Werror")   
        endif()
    endif(WERROR)

    if(NOT APPLE)
        string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-but-set-variable")
        string(APPEND CMAKE_CXX_FLAGS " -Wno-maybe-uninitialized")
    endif()
endif()

# ------ Main Build ------
add_subdirectory(Hazel)
