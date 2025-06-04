file(GLOB ACCEPTANCE_TEST_SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable( sudoku_acceptance_tests ${ACCEPTANCE_TEST_SOURCES})
target_link_libraries( sudoku_acceptance_tests PRIVATE sudoku_solver_lib Catch2::Catch2WithMain)

include( Catch)
catch_discover_tests( sudoku_acceptance_tests)