# Automatically find all .cpp test files
file( GLOB UNIT_TEST_SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable( sudoku_unit_tests ${UNIT_TEST_SOURCES})
add_dependencies( sudoku_unit_tests ${SUDOKU_CLI_NAME})
target_link_libraries( sudoku_unit_tests PRIVATE sudoku_solver_lib Catch2::Catch2WithMain)

include( Catch)
catch_discover_tests( sudoku_unit_tests)