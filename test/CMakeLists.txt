cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

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

file(GLOB 
    HAZEL_INTERNAL_TESTS_SOURCES
    "Compiler/test_*.c"
)

#############################################################################
# One executable for each unit test file
#############################################################################
foreach(source ${HAZEL_INTERNAL_TESTS_SOURCES})
    get_filename_component(testcase ${source} NAME_WE)
    # message("FILE_BASENAME: ${testcase}")
    add_executable(
        ${testcase} # without file extension
        ${source} # With file extension
    )

    # Link HazelInternalTests with libHazelStatic and Muon
    target_link_libraries(
        ${testcase}
        PRIVATE
        libHazelStatic
        Muon::Muon
    )

    # Be CTest-friendly
    add_test(
        NAME ${testcase}
        COMMAND ${testcase}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endforeach()