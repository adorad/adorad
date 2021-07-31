cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(
    AdoradMain 
    LANGUAGES C
)

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Configure the Macro file 
set(ADORAD_BUILD_SHARED_LIB ${BUILD_SHARED_LIBS}) # used in ../cmake/cmake_macros.h.in
configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/cmake_macros.h.in
    ${CMAKE_SOURCE_DIR}/adorad/core/cmake_macros.h
)

#
# Main Build File for the main Adorad Library
# 
# Note that this library must have _minimal_ dependencies 
# _DO NOT_ add `main.c` here (we're building a library, remember?)
file(GLOB LIBADORAD_SOURCES
    compiler/*.c
)
file(GLOB_RECURSE ADORAD_HEADERS 
    *.h
)

# 
# Build the Shared/Static Library
#

# ======================
# For libAdoradStatic
# ======================
if(ADORAD_BUILD_STATIC_LIB)
    message("--------- [INFO] Building libAdoradStatic")

    add_library(libAdoradStatic STATIC ${LIBADORAD_SOURCES} ${ADORAD_HEADERS})
    # Enable hidden visibility if compiler supports it.
    if(${COMPILER_SUPPORTS_HIDDEN_VISIBILITY})
        target_compile_options(libAdoradStatic PRIVATE "-fvisibility=hidden")
    endif()

    target_include_directories(
        libAdoradStatic PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
        # This is here out of convenience. You might break a test or two if this is removed.
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    )

    target_compile_definitions(libAdoradStatic PUBLIC
        _ADORAD_=1
    )

    # Build the executable
    # main.c (or whatever demo file you want to link against)
    add_executable(AdoradStatic ${CMAKE_CURRENT_SOURCE_DIR}/main.c)
    target_link_libraries(AdoradStatic libAdoradStatic)

    # Installation
    install(TARGETS libAdoradStatic DESTINATION lib)
endif() # ADORAD_BUILD_STATIC_LIB


# ======================
# For libAdoradShared
# ======================
# Currently, we have no symbols to export, so building a shared library with Visual Studio will fail.
# Example: __declspec(dllexport) or __declspec(dllimport) --> export/import symbols 
# When we do, go ahead and remove the `if(NOT MSVC)` checks
if(ADORAD_BUILD_SHARED_LIB)
    if(NOT MSVC)
        message("--------- [INFO] Building libAdoradShared")
        add_library(libAdoradShared SHARED ${LIBADORAD_SOURCES} ${ADORAD_HEADERS})
        target_compile_options(libAdoradShared PRIVATE "-fvisibility=hidden")
        # If building a Shared library, set dllimport/dllexport properly.
        target_compile_options(libAdoradShared PRIVATE "-DCSTL_BUILD_MAIN_LIB")

        target_include_directories(
            libAdoradShared PUBLIC 
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
            $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
            # This is here out of convenience. You might break a test or two if this is removed.
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
            $<BUILD_INTERFACE:${CMAKE_PROJECT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:include>
        )

        target_compile_definitions(libAdoradShared PUBLIC
            _ADORAD_=1
        )

        # Build the executable
        # main.c (or whatever demo file you want to link against) =
        add_executable(AdoradShared ${CMAKE_CURRENT_SOURCE_DIR}/main.c)
        target_link_libraries(AdoradShared libAdoradShared)

        install(TARGETS libAdoradShared DESTINATION lib)
    endif() # MSVC
endif() # ADORAD_BUILD_SHARED_LIB


# ======================
# Miscellaneous Moves
# ======================

# *.h --> include/*.h
install(
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# adorad/core/CMake_Macros.h --> include/adorad/core/CMake_Macros.h
install(
    FILES ${CMAKE_BINARY_DIR}/adorad/core/cmake_macros.h
    DESTINATION include/adorad/core/macros
)

# For CSTL_BUILD_SHARED_LIB, see Core/Export.h
if(MSVC AND CSTL_BUILD_SHARED_LIB)
    install(FILES $<TARGET_PDB_FILE:Adorad> DESTINATION lib OPTIONAL)
endif()