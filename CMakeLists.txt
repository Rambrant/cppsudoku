cmake_minimum_required( VERSION 3.25)

# ---------------------------------------------------------------------------
# Auto-detect Homebrew LLVM Clang before project() locks in the compiler.
# This must come BEFORE the project() call.
# ---------------------------------------------------------------------------
# Before project() — auto-detect GCC 16 if no compiler is explicitly set
if(NOT DEFINED CMAKE_CXX_COMPILER)
    find_program(GCC16 NAMES g++-16)
    if(GCC16)
        message(STATUS "Found GCC 16: ${GCC16}")
        set(CMAKE_CXX_COMPILER "${GCC16}" CACHE STRING "" FORCE)
        find_program(GCC16_C NAMES gcc-16)
        set(CMAKE_C_COMPILER "${GCC16_C}" CACHE STRING "" FORCE)
    else()
        message(WARNING "GCC 16 not found — falling back to system compiler.\n"
                "Install with: sudo apt install gcc-16 g++-16  (Linux)\n"
                "              brew install gcc@16              (macOS)")
    endif()
endif()

project( SudokuSolver VERSION 1.0 LANGUAGES CXX)

option( ENABLE_DOCS "Enable building documentation" ON)
enable_testing()

# Require GCC 16+
if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR
            "This project requires GCC. Current compiler: ${CMAKE_CXX_COMPILER_ID}\n"
            "Re-run CMake with: -DCMAKE_CXX_COMPILER=g++-16")
endif()

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "16")
    message(FATAL_ERROR
            "GCC 16+ is required for C++26 support. Found: GCC ${CMAKE_CXX_COMPILER_VERSION}")
endif()

add_compile_options( -Wall -Wextra -Wpedantic -Werror -fcontracts -freflection)

set( CMAKE_TOOLCHAIN_FILE "$ENV{HOME}/Developer/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set( CMAKE_OSX_ARCHITECTURES arm64 CACHE STRING "" FORCE)

set( CMAKE_CXX_STANDARD 26)
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

add_subdirectory( lib)
add_subdirectory( cli)
add_subdirectory( gui)
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