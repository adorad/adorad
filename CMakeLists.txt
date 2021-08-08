cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(
    Adorad 
    DESCRIPTION "The Adorad Programming Language"
    VERSION 1.0.0
    LANGUAGES C
)

# CMake's FATAL_ERROR is ignored on CMake versions >2.6 
# Hence we explicitely check for correct enforcement of CMake configurations.
if (${CMAKE_VERSION} VERSION_LESS "3.20")
    message(
        ERROR "You are currently on CMake Version ${CMAKE_VERSION}, but Adorad supports CMake 3.20 above."
        "Please update your CMake installation."
    )
endif()

    
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
        "Adorad requires at least-std=c11. Please remove -std=c settings in your environment."
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

# Determine if Adorad is built as a subproject (using add_subdirectory) or if it is the main project being built
set(IS_MAIN_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(IS_MAIN_PROJECT ON)
endif()

option(ADORAD_BUILDTESTS "Build Adorad test binaries" OFF)
option(ADORAD_BUILD_STATIC_LIB "Build Adorad Static Library " OFF)
option(ADORAD_BUILD_SHARED_LIB "Build Adorad Shared Library " OFF)
option(BUILD_DOCS "Build Adorad documentation" OFF)

if(ADORAD_BUILDTESTS)
    # We need at least a Static Library to build and link with Adorad's Internal Tests
    if(NOT ADORAD_BUILD_STATIC_LIB)
        set(ADORAD_BUILD_STATIC_LIB ON)
    endif()
endif()

# ------ Options ------
set(ADORAD_ROOT_DIR                  ${CMAKE_CURRENT_SOURCE_DIR})
set(ADORAD_BIN_DIR                   ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)
set(ADORAD_LIB_DIR                   ${CMAKE_CURRENT_SOURCE_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY   ${ADORAD_BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY   ${ADORAD_LIB_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY   ${ADORAD_LIB_DIR})


# ------Compiler and linker options ------
# In case of Makefiles, if the user does not setup CMAKE_BUILD_TYPE, assume it's Release: 
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Build type not set - defaulting to Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug, Release,
    RelWithDebInfo, or MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)


# ------ A List of Compiler Flags ------
# A (more or less comprehensive) list is here: https://caiorss.github.io/C-Cpp-Notes/compiler-flags-options.html
message("--------- [INFO] CMAKE_C_COMPILER_ID               == ${CMAKE_C_COMPILER_ID}")
message("--------- [INFO] CMAKE_C_COMPILER_FRONTEND_VARIANT == ${CMAKE_C_COMPILER_FRONTEND_VARIANT}")
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

    if(NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
        # Clang doesn't have either of these flags
        if(NOT CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
            string(APPEND CMAKE_C_FLAGS " -fPIC")
            string(APPEND CMAKE_C_FLAGS " -Wno-discarded-qualifiers")
            string(APPEND CMAKE_C_FLAGS " -Wno-multistatement-macros")
        else()
    else() # For Clang/AppleClang
        string(APPEND CMAKE_C_FLAGS " -Wno-ignored-qualifiers")
        string(APPEND CMAKE_C_FLAGS " -Wno-newline-eof")
        string(APPEND CMAKE_C_FLAGS " -Wno-reserved-id-macro")
        string(APPEND CMAKE_C_FLAGS " -Wno-deprecated-declarations")
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

    # Ignore some MSVC warnings
    string(APPEND CMAKE_C_FLAGS " /wd4018")
    string(APPEND CMAKE_C_FLAGS " /wd4255")
    string(APPEND CMAKE_C_FLAGS " /wd4267")
    string(APPEND CMAKE_C_FLAGS " /wd4242")
    string(APPEND CMAKE_C_FLAGS " /wd4547")
    string(APPEND CMAKE_C_FLAGS " /wd4061")
    string(APPEND CMAKE_C_FLAGS " /wd4668")
    string(APPEND CMAKE_C_FLAGS " /wd4090")
    string(APPEND CMAKE_C_FLAGS " /wd4100")
    string(APPEND CMAKE_C_FLAGS " /wd4127")
    string(APPEND CMAKE_C_FLAGS " /wd4310")
    string(APPEND CMAKE_C_FLAGS " /wd4514")
    string(APPEND CMAKE_C_FLAGS " /wd4710")
    string(APPEND CMAKE_C_FLAGS " /wd4711")
    string(APPEND CMAKE_C_FLAGS " /wd4777")
    string(APPEND CMAKE_C_FLAGS " /wd5045")
    string(APPEND CMAKE_C_FLAGS " /wd5105")
endif()

# A shared library is always produced if a library type is not passed to the add_library command.
# If this is off, a Static Library will be built by default
set(BUILD_SHARED_LIBS OFF)

# ------ Main Build ------
add_subdirectory(adorad)

if(ADORAD_BUILDTESTS)
    message("--------- [INFO] Building Adorad Tests")
    enable_testing()
    include(CTest)
    add_subdirectory(test)
endif()
