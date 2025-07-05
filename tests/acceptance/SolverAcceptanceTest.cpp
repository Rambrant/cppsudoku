//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <regex>
#include <catch2/catch_test_macros.hpp>

#include "SudokuTraits.hpp"
#include "catch2/generators/catch_generators.hpp"

using namespace com::rambrant::sudoku;
using Traits = SudokuTraits;

#include "CliRunner.hpp"

SCENARIO( "Sudoku backtracking solver [acceptance]")
{
    struct TestCase
    {
        std::string file;
        std::string expectedContent;
        int         exitCode;
    };

    CliRunner  runner( SUDOKU_CLI_COMMAND, SUDOKU_CLI_WORKING_DIR);

    auto testCase = GENERATE(
        TestCase{ "board_simple.txt", "[backtracking]", 0},
        TestCase{ "board_hard.txt", "No solution", 1});

    GIVEN( "Input file " + testCase.file)
    {
        const std::string inputFile = "tests/test-resources/" + testCase.file;

        WHEN( "The solver is run")
        {
            const auto exitCode = runner.run( { "-v", "-s", "backtracking", "-i" , inputFile});

            THEN( "The output should contain " + testCase.expectedContent)
            {
                CHECK( runner.outputContains( testCase.expectedContent));
                CHECK( exitCode == testCase.exitCode);
            }
        }
    }
}

SCENARIO( "Sudoku constraint propagation solver [acceptance]")
{
    CliRunner  runner( SUDOKU_CLI_COMMAND, SUDOKU_CLI_WORKING_DIR);
    std::regex digitLineRegex( R"(^[ 0-9]+$)");

    std::string testFile = GENERATE( "board_simple.txt", "board_hard.txt");

    GIVEN( "Input file " + testFile)
    {
        const std::string inputFile = "tests/test-resources/" + testFile;

        WHEN( "The solver is run")
        {
            const auto exitCode = runner.run( { "-q", "-O", "line", "-s", "constraint", "-i" , inputFile});

            THEN( "The output should contain the solved puzzle")
            {
                CHECK( std::regex_match( runner.output(), digitLineRegex));  // Only digits should be found
                CHECK( exitCode == 0);
            }
        }
    }
}

SCENARIO( "Sudoku with both solvers [acceptance]")
{
    struct TestCase
    {
        std::string file;
        int         numberOfTriedSolvers;
    };

    CliRunner  runner( SUDOKU_CLI_COMMAND, SUDOKU_CLI_WORKING_DIR);
    std::regex triedSolversRegex( R"(Trying solver)");

    auto testCase = GENERATE(
            TestCase{ "board_simple.txt", 1},
            TestCase{ "board_hard.txt", 2});

    GIVEN( "Input file " + testCase.file)
    {
        const std::string inputFile = "tests/test-resources/" + testCase.file;

        WHEN( "The solver is run")
        {
            const auto exitCode = runner.run( { "-v", "-i" , inputFile});

            THEN( "The number of solver tried should be " + std::to_string( testCase.numberOfTriedSolvers))
            {
                CHECK( runner.countOutputMatches( triedSolversRegex) == testCase.numberOfTriedSolvers);
                CHECK( exitCode == 0);
            }
        }
    }
}
