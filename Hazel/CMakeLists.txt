cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(HazelMain CXX)

# message("Sub1 Dir: CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
# message("Sub1 Dir: CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")

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
    Core/*.c 
    Compiler/IO/*.c
    Compiler/Lexer/*.c
    Compiler/Parser/*.c
    Compiler/Tokens/*.c
    Compiler/Types/*.c
)

file(GLOB_RECURSE HAZEL_ALL_TEST_FILES ../test/*.c)
file(GLOB_RECURSE HAZEL_HEADERS *.h)

# Build the Shared/Static Library
# A Shared Library is built if the BUILD_SHARED_LIBS command is on (check the root CMakeLists.txt)
add_library(libHazel ${HAZEL_SOURCES} ${HAZEL_HEADERS})

# Enable hidden visibility if compiler supports it.
if(${COMPILER_SUPPORTS_HIDDEN_VISIBILITY})
    target_compile_options(libHazel PRIVATE "-fvisibility=hidden")
endif()

target_include_directories(
    libHazel PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Build the executable
add_executable(Hazel ${HAZEL_SOURCES}  ${HAZEL_HEADERS})
target_link_libraries(Hazel libHazel)

# Installation
install(TARGETS libHazel DESTINATION lib)
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