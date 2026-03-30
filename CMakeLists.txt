cmake_minimum_required( VERSION 3.25)

# ---------------------------------------------------------------------------
# Auto-detect Homebrew LLVM Clang before project() locks in the compiler.
# This must come BEFORE the project() call.
# ---------------------------------------------------------------------------
if( NOT DEFINED CMAKE_CXX_COMPILER)
    find_program( LLVM_CLANGPP
            NAMES clang++
            PATHS /opt/homebrew/opt/llvm/bin   # Apple Silicon
            /usr/local/opt/llvm/bin       # Intel Mac
            NO_DEFAULT_PATH
    )
    if( LLVM_CLANGPP)
        message( STATUS "Found Homebrew LLVM Clang: ${LLVM_CLANGPP}")
        set( CMAKE_CXX_COMPILER "${LLVM_CLANGPP}" CACHE STRING "" FORCE)
        set( CMAKE_C_COMPILER   "${LLVM_CLANGPP}/../clang" CACHE STRING "" FORCE)
    else()
        message( WARNING "Homebrew LLVM not found — falling back to system compiler.\n"
                "Install with: brew install llvm")
    endif()
endif()

project( SudokuSolver VERSION 1.0 LANGUAGES CXX)

option( ENABLE_DOCS "Enable building documentation" ON)
enable_testing()

# Enforce LLVM Clang (not Apple Clang, which lacks C++26 and -fcontracts)
if( CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    message( FATAL_ERROR
            "Apple Clang does not support C++26 or -fcontracts.\n"
            "Install LLVM via Homebrew:  brew install llvm\n"
            "Then delete your build folder and re-run CMake.")
elseif( NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message( FATAL_ERROR
            "This project requires Clang. Current compiler: ${CMAKE_CXX_COMPILER_ID}\n"
            "Re-run CMake with: -DCMAKE_CXX_COMPILER=clang++")
endif()

add_compile_options( -Wall -Wextra -Wpedantic -Werror)

# Use LLVM's libc++ on Linux — GCC's libstdc++ on ubuntu-latest is too old for C++23 features
if( CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_options( -stdlib=libc++)
    add_link_options( -stdlib=libc++)
endif()

# C++26 Contracts — not yet in Clang mainline (as of Clang 22).
# Enable manually once you have a contracts-capable Clang build:
#   cmake -B build -DENABLE_CONTRACTS=ON
if( ENABLE_CONTRACTS)
    message( STATUS "Contracts enabled: -fcontract-semantic=enforce")
    add_compile_options( -fcontract-semantic=enforce)
else()
    message( STATUS "Contracts disabled (ENABLE_CONTRACTS=OFF). "
            "Enable when using a contracts-capable Clang fork.")
endif()

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