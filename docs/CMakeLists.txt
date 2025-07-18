cmake_minimum_required( VERSION 3.15)

find_package( Doxygen REQUIRED dot)

set( DOXYGEN_INPUT_DIR "${PROJECT_SOURCE_DIR}/src  ${PROJECT_SOURCE_DIR}/includes")
set( DOXYGEN_OUTPUT_DIR ${PROJECT_BINARY_DIR}/docs)
set( DOXYGEN_CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)

set( DOXYGEN_GENERATED_CONFIG ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

configure_file( ${DOXYGEN_CONFIG_FILE} ${DOXYGEN_GENERATED_CONFIG} @ONLY)

add_custom_target( build_documentation
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_GENERATED_CONFIG}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
)