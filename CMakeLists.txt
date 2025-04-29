cmake_minimum_required(VERSION 3.10)
project(SudokuSolver VERSION 1.0 LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable debugging
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# Set output binaries to build/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include header files
include_directories(${PROJECT_SOURCE_DIR}/includes)

# Gather all source files
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Create the executable
add_executable(sudoku_solver ${SOURCES}
        Doxyfile)

# Optionally: Organize source files in IDEs like Xcode/VS
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES})

# Install rule (optional)
install(TARGETS sudoku_solver DESTINATION bin)

# Enable testing if you want to expand later
enable_testing()