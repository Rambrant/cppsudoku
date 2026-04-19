# ---------------------------------------------------------------------------
# sudoku_gui_lib — ViewModel + Commands, no ImGui dependency.
# Linked by both the GUI executable and the unit tests.
# ---------------------------------------------------------------------------
file( GLOB_RECURSE GUI_LIB_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/viewmodel/*.cpp
)

add_library( sudoku_gui_lib STATIC ${GUI_LIB_SOURCES})

# gui/ is the include root — consumers use #include "viewmodel/SudokuViewModel.hpp"
target_include_directories( sudoku_gui_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries( sudoku_gui_lib
        PUBLIC
        sudoku_solver_lib
)

set_target_properties( sudoku_gui_lib PROPERTIES
        CXX_STANDARD          26
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS        OFF
)

# ---------------------------------------------------------------------------
# sudoku_gui — full GUI executable (SDL2 + ImGui, coming soon)
# ---------------------------------------------------------------------------
# add_executable( sudoku_gui main.cpp view/SudokuAppView.cpp view/SudokuGridView.cpp)
# target_link_libraries( sudoku_gui PRIVATE sudoku_gui_lib SDL2::SDL2 imgui::imgui)