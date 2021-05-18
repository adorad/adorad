cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# Hazel's Internal Tests
# We use the Arrow, a custom unit testing framework built by Jason Dsouza <@jasmcaus>

# Arrow requires at least C11/C++11
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

message("--------- [INFO] Fetching Arrow from GitHub")

include(FetchContent)
FetchContent_Declare(
    Arrow
    URL https://github.com/jasmcaus/Arrow/archive/dev.zip
)

FetchContent_MakeAvailable(Arrow)

enable_testing()

file(GLOB 
    HAZEL_TESTS_SOURCES
    "Compiler/test_*.c"
)
add_executable(
    HazelInternalTests
    ${HAZEL_TESTS_SOURCES}
)

message("--------- [INFO] Linking Tests")
# Link HazelInternalTests with libHazelStatic and Arrow
target_link_libraries(
    HazelInternalTests
    PRIVATE
    libHazelStatic
    Arrow::Arrow
)