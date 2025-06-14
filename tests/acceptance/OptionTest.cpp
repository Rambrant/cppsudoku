//
// Created by Thomas Rambrant on 2025-06-03.
//

#include <regex>
#include <catch2/catch_test_macros.hpp>

#include "SudokuTraits.hpp"
#include "catch2/generators/catch_generators.hpp"

using namespace com::rambrant::sudoku;

#include "CliRunner.hpp"

SCENARIO( "Sudoku help option [acceptance]")
{
    CliRunner runner( SUDOKU_CLI_COMMAND, SUDOKU_CLI_WORKING_DIR);

    std::string helpFlag = GENERATE( "-h", "--help");

    GIVEN( "The flag: " + helpFlag)
    {
         WHEN( "The solver is run")
        {
            const auto exitCode = runner.run( { helpFlag});

            THEN( "The output should be the help text")
            {
                CHECK( runner.outputContains( "Usage:"));
                CHECK( exitCode == 0);
            }
        }
    }
}

SCENARIO( "Sudoku quiet options [acceptance]")
{
    CliRunner         runner( SUDOKU_CLI_COMMAND, SUDOKU_CLI_WORKING_DIR);
    const std::string input_file = "tests/test-resources/board_simple.txt";
    std::regex        digitLineRegex( R"(^[ 0-9]+$)");

    std::string quietFlag = GENERATE( "-q", "--quiet");

    GIVEN( "The flag: " + quietFlag)
    {
        WHEN( "A solver is run in quiet mode")
        {
            const auto exitCode = runner.run( { quietFlag, "-O", "line", "-s", "backtracking", "-i" , input_file});

            THEN( "The output should only contain a line with the solver puzzle")
            {
                CHECK( std::regex_match( runner.output(), digitLineRegex));  // Only digits should be found
                CHECK( exitCode == 0);
            }
        }
    }
}

SCENARIO( "Sudoku verbose options [acceptance]")
{
    CliRunner         runner( SUDOKU_CLI_COMMAND, SUDOKU_CLI_WORKING_DIR);
    const std::string input_file = "tests/test-resources/board_simple.txt";

    std::string verboseFlag = GENERATE( "-v", "--verbose");

    GIVEN( "The flag: " + verboseFlag)
    {
        WHEN( "A solver is run in verbose mode")
        {
            const auto exitCode = runner.run( { verboseFlag, "-O", "line", "-s", "backtracking", "-i" , input_file});

            THEN( "The output should contain extra information")
            {
                CHECK( runner.outputContains( "Initializing"));
                CHECK( runner.outputContains( "...Logger"));
                CHECK( exitCode == 0);
            }
        }
    }
}

SCENARIO( "Sudoku no verbosity options [acceptance]")
{
    CliRunner         runner( SUDOKU_CLI_COMMAND, SUDOKU_CLI_WORKING_DIR);
    const std::string input_file = "tests/test-resources/board_simple.txt";

    GIVEN( "Normal output")
    {
        WHEN( "A solver is run in verbose mode")
        {
            const auto exitCode = runner.run( { "-O", "line", "-s", "backtracking", "-i" , input_file});

            THEN( "The output should contain no extra information")
            {
                CHECK( runner.outputContains( "Initializing"));
                CHECK_FALSE( runner.outputContains( "...Logger"));
                CHECK( exitCode == 0);
            }
        }
    }
}

SCENARIO( "Sudoku both verbosity and quiet options [acceptance]")
{
    CliRunner runner( SUDOKU_CLI_COMMAND, SUDOKU_CLI_WORKING_DIR);

    GIVEN( "Normal output")
    {
        WHEN( "A solver is run in verbose mode")
        {
            const auto exitCode = runner.run( { "-v", "-q"});

            THEN( "The call should fail")
            {
                CHECK( exitCode == 1);
            }
        }
    }
}
