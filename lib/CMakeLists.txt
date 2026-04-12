find_package( nlohmann_json CONFIG REQUIRED)

file( GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp
)

add_library( sudoku_solver_lib STATIC ${LIB_SOURCES})

# lib/ is the include root — consumers use #include "core/SudokuTraits.hpp" etc.
target_include_directories( sudoku_solver_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries( sudoku_solver_lib
        PUBLIC
        nlohmann_json::nlohmann_json
)

set_target_properties( sudoku_solver_lib PROPERTIES
        CXX_STANDARD          26
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS        OFF
)