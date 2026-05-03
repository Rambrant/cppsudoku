file( GLOB ACCEPTANCE_CLI_SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable( sudoku_acceptance_tests ${ACCEPTANCE_CLI_SOURCES})
add_dependencies( sudoku_acceptance_tests ${SUDOKU_CLI_NAME})

target_link_libraries( sudoku_acceptance_tests
        PRIVATE
        sudoku_solver_lib
        Catch2::Catch2WithMain
)

include( CheckCXXCompilerFlag)
check_cxx_compiler_flag( -Wno-c2y-extensions COMPILER_SUPPORTS_WNO_C2Y)
if( COMPILER_SUPPORTS_WNO_C2Y)
    target_compile_options( sudoku_acceptance_tests PRIVATE -Wno-c2y-extensions)
endif()

target_compile_definitions( sudoku_acceptance_tests PRIVATE
        SUDOKU_CLI_WORKING_DIR="${SUDOKU_CLI_WORKING_DIR}"
        SUDOKU_CLI_COMMAND="${SUDOKU_CLI_COMMAND}"
)

include( Catch)
catch_discover_tests( sudoku_acceptance_tests)