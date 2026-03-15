# Automatically find all .cpp test files
file( GLOB UNIT_TEST_SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable( sudoku_unit_tests ${UNIT_TEST_SOURCES})
add_dependencies( sudoku_unit_tests ${SUDOKU_CLI_NAME})
target_link_libraries( sudoku_unit_tests PRIVATE sudoku_solver_lib Catch2::Catch2WithMain)

# Catch2 uses __COUNTER__ internally, which newer Clang versions classify as a
# C2y extension under -std=c++26, turning -Werror into a build failure.
# Only apply the suppression flag if the compiler actually supports it,
# so older Clang versions on CI do not fail with -Wunknown-warning-option.
include( CheckCXXCompilerFlag)
check_cxx_compiler_flag( -Wno-c2y-extensions COMPILER_SUPPORTS_WNO_C2Y)
if( COMPILER_SUPPORTS_WNO_C2Y)
    target_compile_options( sudoku_unit_tests PRIVATE -Wno-c2y-extensions)
endif()

include( Catch)
catch_discover_tests( sudoku_unit_tests)