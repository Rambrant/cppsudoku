cmake_minimum_required( VERSION 3.10)
project( SudokuSolver VERSION 1.0 LANGUAGES CXX)

option( ENABLE_DOCS "Enable building documentation" ON)
enable_testing()

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

set( CMAKE_TOOLCHAIN_FILE "$ENV{HOME}/Developer/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set( CMAKE_OSX_ARCHITECTURES arm64 CACHE STRING "" FORCE)

set( CMAKE_CXX_STANDARD 17)
set( CMAKE_CXX_STANDARD_REQUIRED ON)
set( CMAKE_BUILD_TYPE Debug)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set( SUDOKU_CLI_NAME "sudoku_solver")
set( SUDOKU_CLI_WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set( SUDOKU_CLI_COMMAND "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${SUDOKU_CLI_NAME}")

# External libraries
include( FetchContent)

FetchContent_Declare( Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v3.5.2)
FetchContent_MakeAvailable( Catch2)

add_subdirectory( src)
add_subdirectory( tests/unit)
add_subdirectory( tests/acceptance)

if( ENABLE_DOCS)
    add_subdirectory( docs)
endif()

# Custom target that ensures CLI app + tests are built, then runs tests
add_custom_target( build_solver_and_run_tests
        DEPENDS ${SUDOKU_CLI_NAME} sudoku_unit_tests sudoku_acceptance_tests
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        COMMENT "✅ Building and running solver + tests..."
)
add_custom_target( run_all_tests
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        DEPENDS sudoku_unit_tests sudoku_acceptance_tests
        COMMENT "🏃 Running all tests..."
)