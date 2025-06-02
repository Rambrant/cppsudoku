cmake_minimum_required(VERSION 3.10)

# Tell CMake to use vcpkg for dependencies
set(CMAKE_TOOLCHAIN_FILE "$ENV{HOME}/Developer/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")

# ✅ Explicitly declare macOS architecture
set(CMAKE_OSX_ARCHITECTURES arm64 CACHE STRING "" FORCE)  # or "x86_64" if on Intel

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

# Find nlohmann_json provided via vcpkg
find_package(nlohmann_json CONFIG REQUIRED)
set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)

# Create the executable
add_executable(sudoku_solver ${SOURCES} )

target_link_libraries(sudoku_solver PRIVATE nlohmann_json::nlohmann_json)

# Optionally: Organize source files in IDEs like Xcode/VS
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES})

# Install rule (optional)
install(TARGETS sudoku_solver DESTINATION bin)

# Enable testing if you want to expand later
enable_testing()

#Doxygen support
find_package(Doxygen REQUIRED)

# Optionally add a CMake option to toggle doc generation
option(BUILD_DOC "Build documentation with Doxygen" ON)

if(BUILD_DOC)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/docs)

    add_custom_target(doc
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
endif()