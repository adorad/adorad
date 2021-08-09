cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

# Adorad's Internal Tests
# We use the Tau, a custom unit testing framework built by Jason Dsouza <@jasmcaus>

# Tau requires at least C11/C++11
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

# Fetch Tau, our custom C/C++ testing library
message("--------- [INFO] Fetching Tau from GitHub")
include(FetchContent)
FetchContent_Declare(
    Tau
    URL https://github.com/jasmcaus/tau/archive/dev.zip
)
FetchContent_MakeAvailable(Tau)

file(GLOB 
    ADORAD_INTERNAL_TESTS_SOURCES
    "compiler/test_*.c"
)

# We need to create a separate library that links to our tests
# The reason is because most of the methods that are part of Adorad's Compiler are internally-linked 
# (i.e they are either `static`, `inline` or both) and thus we'd be unable to test these core methods.
# For this reason, we've written a custom Python script (`before_tests_ci.py` can be found in {ROOT}/tools/tests) 
# that essentially copies over the entire Adorad folder into a new folder `AdoradInternalTests` stripping away any 
# keywords that may result in internal linkage.
# Theoretically, this should work and we should be able to test the core logic behind Adorad.
# Note: This only happens during the CI phase and _should not_ run whenever an external library needs to 
# link to Adorad
file(GLOB ADORAD_INTERNALTEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../AdoradInternalTests/compiler/*.c
)
file(GLOB_RECURSE ADORAD_INTERNALTEST_HEADERS 
    ${CMAKE_CURRENT_SOURCE_DIR}/../AdoradInternalTests/*.h
)

# Main Build File for the main CSTL Library
file(GLOB LIBCSTLINTERNALTESTS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../AdoradInternalTests/coreten/*.c
)
file(GLOB_RECURSE CSTLINTERNALTESTS_HEADERS 
    ${CMAKE_CURRENT_SOURCE_DIR}/../AdoradInternalTests/coreten/*.c
)
set(
    CSTLINTERNALTESTS_BUILD_INCLUDE_DIRS
    # "${ADORAD_ROOT_DIR}/AdoradInternalTests/core" # results in weird <windows.h> errors (conflicts with adorad/coreten/windows.h)
    "${ADORAD_ROOT_DIR}"
)

################
##  Building CSTL
################

message("--------- [INFO] Building the Static Library for CSTLInternalTests ")
add_library(
    libCoretenTests STATIC
    ${LIBCSTLINTERNALTESTS_SOURCES}
    ${CSTLINTERNALTESTS_HEADERS}
)
# Add an alias
add_library(libCoretenTests::libCoretenTests ALIAS libCoretenTests)

target_include_directories(
    libCoretenTests PUBLIC
    "$<BUILD_INTERFACE:${CSTLINTERNALTESTS_BUILD_INCLUDE_DIRS}>"
)

################
##  Building AdoradInternalTests
################
message("--------- [INFO] Building the Static Library for AdoradInternalTests ")
add_library(libAdoradInternalTests STATIC ${ADORAD_INTERNALTEST_SOURCES} ${ADORAD_INTERNALTEST_HEADERS})
# Enable hidden visibility if compiler supports it.
if(${COMPILER_SUPPORTS_HIDDEN_VISIBILITY})
    target_compile_options(libAdoradInternalTests PRIVATE "-fvisibility=hidden")
endif()

target_include_directories(
    libAdoradInternalTests PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../AdoradInternalTests>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
    # This is here out of convenience. You might break a test or two if this is removed.
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Build the executable
# main.c (or whatever demo file you want to link against)
add_executable(
    AdoradInternalTestsWithMain 
    # maininternaltests.c is renamed to `main.c` in tools/tests/before_tests_ci.py
    ${CMAKE_CURRENT_SOURCE_DIR}/../AdoradInternalTests/main.c
)
target_link_libraries(
    AdoradInternalTestsWithMain PUBLIC
    libAdoradInternalTests
    libCoretenTests::libCoretenTests
)

# Installation
install(TARGETS libAdoradInternalTests DESTINATION lib)


message("--------- [INFO] Building the test executables ")
#############################################################################
# One executable for each unit test file
#############################################################################
foreach(source ${ADORAD_INTERNAL_TESTS_SOURCES})
    get_filename_component(testcase ${source} NAME_WE)
    add_executable(
        ${testcase} # without file extension
        ${source}   # with file extension
    )

    # Link AdoradInternalTests with libAdoradInternalTests and Tau
    target_link_libraries(
        ${testcase}
        PRIVATE
        libAdoradInternalTests
        libCoretenTests::libCoretenTests
        Tau::Tau
    )

    # Be CTest-friendly
    add_test(
        NAME ${testcase}
        COMMAND ${testcase}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endforeach() 