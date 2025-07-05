//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <fstream>
#include <regex>
#include <catch2/catch_test_macros.hpp>

#include "SudokuTraits.hpp"
#include "catch2/generators/catch_generators.hpp"

using namespace com::rambrant::sudoku;
using Traits = SudokuTraits;

#include "CliRunner.hpp"

inline auto readFileToString( const std::string& path ) -> std::string
{
    std::ifstream      file( path);
    std::ostringstream buffer;

    buffer << file.rdbuf();

    return buffer.str();
}

SCENARIO( "Reading Sudoku boards [acceptance]")
{
    struct TestCase
    {
        std::string flag;
        std::string file;
        std::string format;
    };

    CliRunner  runner( SUDOKU_CLI_COMMAND, SUDOKU_CLI_WORKING_DIR);

    auto testCase = GENERATE(
        TestCase{ "-I", "board_simple.txt", "text"},
        TestCase{ "--input_format", "board_json.txt", "json"});

    GIVEN( "Standard input from " + testCase.file + " with format " + testCase.format)
    {
        const std::string inputFile = readFileToString( std::string("") + SUDOKU_CLI_WORKING_DIR + "/tests/test-resources/" + testCase.file);

        WHEN( "The solver is run")
        {
            const auto exitCode = runner.run( { testCase.flag, testCase.format}, inputFile);

            THEN( "The result should be success")
            {
                CHECK( exitCode == 0);
            }
        }
    }

    GIVEN( "Input file " + testCase.file + " with format " + testCase.format)
    {
        const std::string inputFile = "tests/test-resources/" + testCase.file;

        WHEN( "The solver is run")
        {
            const auto exitCode = runner.run( { "-i", inputFile, testCase.flag, testCase.format});

            THEN( "The result should be success")
            {
                CHECK( exitCode == 0);
            }
        }
    }
}