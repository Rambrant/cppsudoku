# Create the core library (excluding main.cpp)
file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
list(FILTER LIB_SOURCES EXCLUDE REGEX ".*/main.cpp$")

add_library(sudoku_solver_lib STATIC ${LIB_SOURCES})

# Include path for headers
target_include_directories(sudoku_solver_lib
        PUBLIC
        ${PROJECT_SOURCE_DIR}/includes
)

# Link required libraries
target_link_libraries(sudoku_solver_lib
        PUBLIC
        nlohmann_json::nlohmann_json
)

# Set C++ standard
set_target_properties(sudoku_solver_lib PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
)

# CLI app
add_executable(sudoku_solver ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(sudoku_solver PRIVATE sudoku_solver_lib)