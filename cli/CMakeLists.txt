# Library containing reusable CLI logic (testable)
add_library( sudoku_cli_lib STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineParser.cpp
)

target_include_directories( sudoku_cli_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries( sudoku_cli_lib
        PUBLIC
        sudoku_solver_lib
)

# Executable — just main.cpp, links everything via the two libs
add_executable( ${SUDOKU_CLI_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries( ${SUDOKU_CLI_NAME}
        PRIVATE
        sudoku_cli_lib
)