cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(Hazel CXX)

set(CMAKE_CXX_STANDARD 17)
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

file(GLOB HAZEL_SOURCES
    Core/*.cpp 
    Compiler/IO/*.cpp
    Compiler/Lexer/*.cpp
    Compiler/Parser/*.cpp
    Compiler/Tokens/*.cpp
    Compiler/Types/*.cpp
)

file(GLOB_RECURSE HAZEL_ALL_TEST_FILES ../test/*.cpp)
file(GLOB_RECURSE HAZEL_HEADERS *.h)

add_library(Hazel $(HAZEL_SOURCES) &(HAZEL_HEADERS))

# Enable hidden visibility if compiler supports it.
if(${COMPILER_SUPPORTS_HIDDEN_VISIBILITY})
    target_compile_options(Hazel PRIVATE "-fvisibility=hidden")
endif()

target_include_directories(
    Hazel PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>)
)

# Installation
install(TARGETS Hazel DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        DESTINATION include
        FILES_MATCHING PATTERN "*.h"
)
install(FILES ${CMAKE_BINARY_DIR}/Hazel/Core/CMake_Macros.h
        DESTINATION include/Hazel/Core/Macros
)

if(MSVC AND HAZEL_BUILD_SHARED_LIBS)
    install(FILES $<TARGET_PDB_FILE:c10> DESTINATION lib OPTIONAL)
endif()