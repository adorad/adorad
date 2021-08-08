# Main Build File for the main CSTL Library

file(GLOB LIBCSTL_SOURCES
    *.c
)

file(GLOB_RECURSE CSTL_HEADERS 
    *.h
)

set(
    CSTL_BUILD_INCLUDE_DIRS
    "${ADORAD_ROOT_DIR}/adorad/core"
    "${ADORAD_ROOT_DIR}/adorad"
    "${ADORAD_ROOT_DIR}"
)

add_library(
    CSTL STATIC
    ${LIBCSTL_SOURCES}
    ${CSTL_HEADERS}
)
# Add an alias
add_library(CSTL::CSTL ALIAS CSTL)

target_include_directories(
    CSTL PUBLIC
    "$<BUILD_INTERFACE:${CSTL_BUILD_INCLUDE_DIRS}>"
)