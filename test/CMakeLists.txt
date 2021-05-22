cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# Hazel's Internal Tests
# We use the Muon, a custom unit testing framework built by Jason Dsouza <@jasmcaus>

# Muon requires at least C11/C++11
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

message("--------- [INFO] Fetching Muon from GitHub")

include(FetchContent)
FetchContent_Declare(
    Muon
    URL https://github.com/jasmcaus/Muon/archive/dev.zip
)

FetchContent_MakeAvailable(Muon)

enable_testing()
include(CTest)

file(GLOB 
    HAZEL_INTERNAL_TESTS_SOURCES
    "Compiler/test_*.c"
)

# We need to create a separate library that links to our tests
# The reason is because most of the methods that are part of Hazel's Compiler are internally-linked 
# (i.e they are either `static`, `inline` or both) and thus we'd be unable to test these core methods.
# For this reason, we've written a custom Python script (`before_tests_ci.py` can be found in {ROOT}/tools/tests) 
# that essentially copies over the entire Hazel folder into a new folder `HazelInternalTests` stripping away any 
# keywords that may result in internal linkage.
# Theoretically, this should work and we should be able to test the core logic behind Hazel.
# Note: This only happens during the CI phase and _should not_ run whenever an external library needs to 
# link to Hazel
file(GLOB HAZEL_INTERNALTEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../HazelInternalTests/Compiler/IO/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../HazelInternalTests/Compiler/Lexer/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../HazelInternalTests/Compiler/Tokens/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../HazelInternalTests/Compiler/Types/*.c
)
file(GLOB_RECURSE HAZEL_INTERNALTEST_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../HazelInternalTests/*.h)

message("--------- [INFO] Building the Static Library for HazelInternalTests ")
add_library(libHazelInternalTests STATIC ${HAZEL_INTERNALTEST_SOURCES} ${HAZEL_INTERNALTEST_HEADERS})
# Enable hidden visibility if compiler supports it.
if(${COMPILER_SUPPORTS_HIDDEN_VISIBILITY})
    target_compile_options(libHazelInternalTests PRIVATE "-fvisibility=hidden")
endif()

target_include_directories(
    libHazelInternalTests PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
    # This is here out of convenience. You might break a test or two if this is removed.
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Build the executable
# main.c (or whatever demo file you want to link against)
add_executable(HazelInternalStatic ${CMAKE_CURRENT_SOURCE_DIR}/../HazelInternalTests/main.c)
target_link_libraries(HazelInternalStatic libHazelInternalTests)

# Installation
install(TARGETS libHazelInternalTests DESTINATION lib)


#############################################################################
# One executable for each unit test file
#############################################################################
foreach(source ${HAZEL_INTERNAL_TESTS_SOURCES})
    get_filename_component(testcase ${source} NAME_WE)
    add_executable(
        ${testcase} # without file extension
        ${source}   # With file extension
    )

    # Link HazelInternalTests with libHazelInternalTests and Muon
    target_link_libraries(
        ${testcase}
        PRIVATE
        libHazelInternalTests
        Muon::Muon
    )

    # Be CTest-friendly
    add_test(
        NAME ${testcase}
        COMMAND ${testcase}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endforeach() 