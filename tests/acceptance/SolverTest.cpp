//
// Created by Thomas Rambrant on 2025-06-03.
//

#include <regex>
#include <catch2/catch_test_macros.hpp>

#include "SudokuTraits.hpp"

using namespace com::rambrant::sudoku;
using Traits = SudokuTraits;

#include "CliRunner.hpp"

SCENARIO( "Sudoku backtracking solver [acceptance]")
{
    CliRunner  runner( SUDOKU_CLI_COMMAND, SUDOKU_CLI_WORKING_DIR);

    GIVEN( "A valid Sudoku input file with an easy puzzle")
    {
        const std::string input_file = "tests/test-resources/board_simple.txt";

        WHEN( "The solver is run")
        {
            const auto exitCode = runner.run( { "-v", "-s", "backtracking", "-i" , input_file});

            THEN( "The output should contain the solved puzzle")
            {
                CHECK( runner.outputContains( "[backtracking]"));
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

SCENARIO( "Sudoku constraint propagation solver [acceptance]")
{
    CliRunner  runner( SUDOKU_CLI_COMMAND, SUDOKU_CLI_WORKING_DIR);
    std::regex digitLineRegex( R"(^[ 0-9]+$)");

    GIVEN( "A valid Sudoku input file with an easy puzzle")
    {
        const std::string input_file = "tests/test-resources/board_simple.txt";

        WHEN( "The solver is run")
        {
            const auto exitCode = runner.run( { "-q", "-O", "line", "-s", "constraint", "-i" , input_file});

            THEN( "The output should contain the solved puzzle")
            {
                CHECK( std::regex_match( runner.output(), digitLineRegex));  // Only digits should be found
                CHECK( exitCode == 0);
            }
        }
    }

    GIVEN( "A valid Sudoku input file with an hard puzzle")
    {
        const std::string input_file = "tests/test-resources/board_hard.txt";

        WHEN( "The solver is run")
        {
            const auto exitCode = runner.run( { "--quiet", "-O", "line", "--solvers", "constraint", "--input" , input_file});

            THEN( "No solution should be found")
            {
                CHECK( std::regex_match( runner.output(), digitLineRegex));  // Only digits should be found
                CHECK( exitCode == 0);
            }
        }
    }
}
