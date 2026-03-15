file(GLOB ACCEPTANCE_TEST_SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable( sudoku_acceptance_tests ${ACCEPTANCE_TEST_SOURCES})
add_dependencies( sudoku_acceptance_tests ${SUDOKU_CLI_NAME})
target_link_libraries( sudoku_acceptance_tests PRIVATE sudoku_solver_lib Catch2::Catch2WithMain)

# Catch2 uses __COUNTER__ internally, which Clang classifies as a C2y extension
# under -std=c++26. Suppress the warning for the test executable only so that
# -Werror does not break the build. Our own source files remain fully clean.
target_compile_options( sudoku_acceptance_tests PRIVATE -Wno-c2y-extensions)

# define the path for use in test code
target_compile_definitions( sudoku_acceptance_tests PRIVATE
        SUDOKU_CLI_WORKING_DIR="${SUDOKU_CLI_WORKING_DIR}"
        SUDOKU_CLI_COMMAND="${SUDOKU_CLI_COMMAND}"
)

include( Catch)
catch_discover_tests( sudoku_acceptance_tests)