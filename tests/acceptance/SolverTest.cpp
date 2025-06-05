//
// Created by Thomas Rambrant on 2025-06-03.
//

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/detail/input/parser.hpp>

#include "SudokuTraits.hpp"

using namespace com::rambrant::sudoku;
using Traits = SudokuTraits;

#include "CliRunner.hpp"

SCENARIO( "Sudoku backtracking solver [acceptance]")
{
    CliRunner runner( SUDOKU_CLI_COMMAND, SUDOKU_CLI_WORKING_DIR);

    GIVEN( "A valid Sudoku input file with an easy puzzle")
    {
        const std::string input_file = "tests/test-resources/board_simple.txt";

        WHEN( "The solver is run")
        {
            const auto exitCode = runner.run( { "-q", "-s", "backtracking", "-i" , input_file});

            THEN( "The output should contain the solved puzzle")
            {
                CHECK( runner.output().find('x') == std::string::npos);  // No unsolved cells should be found
                CHECK( exitCode == 0);
            }
        }
    }

    GIVEN( "A valid Sudoku input file with an hard puzzle")
    {
        const std::string input_file = "tests/test-resources/board_hard.txt";

        WHEN( "The solver is run")
        {
            const auto exitCode = runner.run( { "--quiet", "--solvers", "backtracking", "--input" , input_file});

            THEN( "No solution should be found")
            {
                CHECK( runner.outputContains( "No solution"));
                CHECK( exitCode == 1);
            }
        }
    }
}
