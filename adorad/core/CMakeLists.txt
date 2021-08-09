# Main Build File for the main Coreten Library

file(GLOB LIBCORETEN_SOURCES
    *.c
)

file(GLOB_RECURSE CORETEN_HEADERS 
    *.h
)

set(
    CORETEN_BUILD_INCLUDE_DIRS
    # "${ADORAD_ROOT_DIR}/adorad/core" # results in weird <windows.h> errors (conflicts with adorad/core/windows.h)
    "${ADORAD_ROOT_DIR}"
)

add_library(
    Coreten STATIC
    ${LIBCORETEN_SOURCES}
    ${CORETEN_HEADERS}
)
# Add an alias
add_library(Coreten::Coreten ALIAS Coreten)

target_include_directories(
    Coreten PUBLIC
    "$<BUILD_INTERFACE:${CORETEN_BUILD_INCLUDE_DIRS}>"
)