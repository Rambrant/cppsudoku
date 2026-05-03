file( GLOB_RECURSE UNIT_TEST_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp
)

add_executable( sudoku_unit_tests ${UNIT_TEST_SOURCES})
add_dependencies( sudoku_unit_tests ${SUDOKU_CLI_NAME})

target_link_libraries( sudoku_unit_tests
        PRIVATE
        sudoku_solver_lib
        sudoku_cli_lib
        sudoku_gui_lib
        Catch2::Catch2WithMain
)

include( CheckCXXCompilerFlag)
check_cxx_compiler_flag( -Wno-c2y-extensions COMPILER_SUPPORTS_WNO_C2Y)
if( COMPILER_SUPPORTS_WNO_C2Y)
    target_compile_options( sudoku_unit_tests PRIVATE -Wno-c2y-extensions)
endif()

include( Catch)
catch_discover_tests( sudoku_unit_tests)