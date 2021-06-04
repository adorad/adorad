cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(
    HazelMain 
    LANGUAGES C
)

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Configure the Macro file 
set(HAZEL_BUILD_SHARED_LIB ${BUILD_SHARED_LIBS}) # used in ../CMake/cmake_macros.h.in
configure_file(
    ${CMAKE_SOURCE_DIR}/CMake/cmake_macros.h.in
    ${CMAKE_SOURCE_DIR}/hazel/core/CMake_Macros.h
)

#
# Main Build File for the main Hazel Library
# 
# Note that this library must have _minimal_ dependencies 
# _DO NOT_ add `main.c` here (we're building a library, remember?)
file(GLOB LIBHAZEL_SOURCES
    compiler/io/*.c
    compiler/lexer/*.c
    compiler/tokens/*.c
    compiler/types/*.c
)
file(GLOB_RECURSE HAZEL_HEADERS *.h)

# 
# Build the Shared/Static Library
#

# ======================
# For libHazelStatic
# ======================
if(HAZEL_BUILD_STATIC_LIB)
    message("--------- [INFO] Building libHazelStatic")

    add_library(libHazelStatic STATIC ${LIBHAZEL_SOURCES} ${HAZEL_HEADERS})
    # Enable hidden visibility if compiler supports it.
    if(${COMPILER_SUPPORTS_HIDDEN_VISIBILITY})
        target_compile_options(libHazelStatic PRIVATE "-fvisibility=hidden")
    endif()

    target_include_directories(
        libHazelStatic PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
        # This is here out of convenience. You might break a test or two if this is removed.
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    )
    # Build the executable
    # main.c (or whatever demo file you want to link against)
    add_executable(HazelStatic ${CMAKE_CURRENT_SOURCE_DIR}/main.c)
    target_link_libraries(HazelStatic libHazelStatic)

    # Installation
    install(TARGETS libHazelStatic DESTINATION lib)
endif() # HAZEL_BUILD_STATIC_LIB


# ======================
# For libHazelShared
# ======================
# Currently, we have no symbols to export, so building a shared library with Visual Studio will fail.
# Example: __declspec(dllexport) or __declspec(dllimport) --> export/import symbols 
# When we do, go ahead and remove the `if(NOT MSVC)` checks
if(HAZEL_BUILD_SHARED_LIB)
    if(NOT MSVC)
        message("--------- [INFO] Building libHazelShared")
        add_library(libHazelShared SHARED ${LIBHAZEL_SOURCES} ${HAZEL_HEADERS})
        target_compile_options(libHazelShared PRIVATE "-fvisibility=hidden")
        # If building a Shared library, set dllimport/dllexport properly.
        target_compile_options(libHazelShared PRIVATE "-DCSTL_BUILD_MAIN_LIB")

        target_include_directories(
            libHazelShared PUBLIC 
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
            $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
            # This is here out of convenience. You might break a test or two if this is removed.
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
            $<INSTALL_INTERFACE:include>
        )

        # Build the executable
        # main.c (or whatever demo file you want to link against) =
        add_executable(HazelShared ${CMAKE_CURRENT_SOURCE_DIR}/main.c)
        target_link_libraries(HazelShared libHazelShared)

        install(TARGETS libHazelShared DESTINATION lib)
    endif() # MSVC
endif() # HAZEL_BUILD_SHARED_LIB


# ======================
# Miscellaneous Moves
# ======================

# *.h --> include/*.h
install(
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# hazel/core/CMake_Macros.h --> include/hazel/core/Macros/CMake_Macros.h
install(
    FILES ${CMAKE_BINARY_DIR}/hazel/core/CMake_Macros.h
    DESTINATION include/hazel/core/Macros
)

# For CSTL_BUILD_SHARED_LIB, see Core/Export.h
if(MSVC AND CSTL_BUILD_SHARED_LIB)
    install(FILES $<TARGET_PDB_FILE:Hazel> DESTINATION lib OPTIONAL)
endif()