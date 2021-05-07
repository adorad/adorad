cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(HazelMain LANGUAGES C)

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Configure the Macro file 
set(C10_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS}) # used in ../CMake/cmake_macros.h.in
configure_file(
    ${CMAKE_SOURCE_DIR}/CMake/cmake_macros.h.in
    ${CMAKE_SOURCE_DIR}/Hazel/Core/CMake_Macros.h
)

# Main Build File for the main Hazel Library
# 
# Note that this library must have _minimal_ dependencies 
# _DO NOT_ add `main.c` here (we're building a library, remember?)
file(GLOB LIBHAZEL_SOURCES
    Compiler/IO/*.c
    Compiler/Lexer/*.c
    Compiler/Tokens/*.c
    Compiler/Types/*.c
)

file(GLOB_RECURSE HAZEL_ALL_TEST_FILES ../test/*.c)
file(GLOB_RECURSE HAZEL_HEADERS *.h)

# Build the Shared/Static Library
#
# Currently, we have no symbols to export, so building a shared library with Visual Studio will fail.
# Example: __declspec(dllexport) or __declspec(dllimport) --> export/import symbols 
# When we do, go ahead and remove the `if(NOT MSVC)` checks

add_library(libHazelStatic STATIC ${LIBHAZEL_SOURCES} ${HAZEL_HEADERS})
if(NOT MSVC)
    add_library(libHazelShared SHARED ${LIBHAZEL_SOURCES} ${HAZEL_HEADERS})
endif() # MSVC

# Enable hidden visibility if compiler supports it.
if(${COMPILER_SUPPORTS_HIDDEN_VISIBILITY})
    target_compile_options(libHazelStatic PRIVATE "-fvisibility=hidden")
    if(NOT MSVC)
        target_compile_options(libHazelShared PRIVATE "-fvisibility=hidden")
    endif() # MSVC
endif()

# ======================
# For libHazelStatic
# =====================
target_include_directories(
    libHazelStatic PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)
# Build the executable
# main.c (or whatever demo file you want to link against) =
add_executable(HazelStatic ${CMAKE_CURRENT_SOURCE_DIR}/main.c)
target_link_libraries(HazelStatic libHazelStatic)

# Installation
install(TARGETS libHazelStatic DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        DESTINATION include
        FILES_MATCHING PATTERN "*.h"
)

# ================

if(NOT MSVC)
    # ======================
    # For libHazelShared
    # =====================
    target_include_directories(
        libHazelShared PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>
    )
    # Build the executable
    # main.c (or whatever demo file you want to link against) =
    add_executable(HazelShared ${CMAKE_CURRENT_SOURCE_DIR}/main.c)
    target_link_libraries(HazelShared libHazelShared)
endif() # MSVC

# Installation
install(TARGETS libHazelStatic DESTINATION lib)
if(NOT MSVC)
    install(TARGETS libHazelShared DESTINATION lib)
endif() # MSVC

# *.h --> include/*.h
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        DESTINATION include
        FILES_MATCHING PATTERN "*.h"
)

# Hazel/Core/CMake_Macros.h --> include/Hazel/Core/Macros/CMake_Macros.h
install(FILES ${CMAKE_BINARY_DIR}/Hazel/Core/CMake_Macros.h
        DESTINATION include/Hazel/Core/Macros
)

if(MSVC AND HAZEL_BUILD_SHARED_LIBS)
    install(FILES $<TARGET_PDB_FILE:Hazel> DESTINATION lib OPTIONAL)
endif()