cmake_minimum_required( VERSION 3.15)

find_package( Doxygen REQUIRED dot)

set( DOXYGEN_INPUT_DIRS
        "${PROJECT_SOURCE_DIR}/cli"
        "${PROJECT_SOURCE_DIR}/gui/viewmodel"
        "${PROJECT_SOURCE_DIR}/lib/core"
        "${PROJECT_SOURCE_DIR}/lib/readers"
        "${PROJECT_SOURCE_DIR}/lib/solvers"
        "${PROJECT_SOURCE_DIR}/lib/writers"
)

# Doxygen INPUT accepts space-separated paths, but each path must be unambiguous.
# The safest cross-platform separator is a newline (or backslash-newline).
# We join with a space here; paths with spaces would need quoting, but none of
# ours have spaces so this is fine.
list( JOIN DOXYGEN_INPUT_DIRS " \\\n                         " DOXYGEN_INPUT_DIR)

set( DOXYGEN_OUTPUT_DIR ${PROJECT_BINARY_DIR}/docs)
set( DOXYGEN_CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set( DOXYGEN_README ${PROJECT_SOURCE_DIR}/README.md)

set( DOXYGEN_GENERATED_CONFIG ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

configure_file( ${DOXYGEN_CONFIG_FILE} ${DOXYGEN_GENERATED_CONFIG} @ONLY)

add_custom_target( build_documentation
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_GENERATED_CONFIG}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
)