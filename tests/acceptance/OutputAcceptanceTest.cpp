//
// Created by Thomas Rambrant on 2025-06-03.
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

SCENARIO( "Writing Sudoku boards [acceptance]")
{
    struct TestCase
    {
        std::string flag;
        std::string format;
        std::regex  regex;
    };

    CliRunner  runner( SUDOKU_CLI_COMMAND, SUDOKU_CLI_WORKING_DIR);

    auto testCase = GENERATE(
        TestCase{ "-O", "pretty"},
        TestCase{ "-output_format", "block"},
        TestCase{ "-O", "line"},
        TestCase{ "--output_format", "json"});

    GIVEN( "Standard output with format " + testCase.format)
    {
        const std::string inputFile = "tests/test-resources/board_simple.txt";

        std::regex digitCountRegex( R"([0-9])");


        WHEN( "The solver is run")
        {
            const auto exitCode = runner.run( { "-q", testCase.flag, testCase.format, "-i", inputFile});

            THEN( "The result should be success")
            {
                CHECK( runner.countOutputMatches( digitCountRegex) == Traits::BOARD_SIZE * Traits::BOARD_SIZE);
                CHECK( exitCode == 0);
            }
        }
    }

    GIVEN( "Standard output to file with format " + testCase.format)
    {
        const std::string inputFile    = "tests/test-resources/board_simple.txt";
        const std::string outputFile   = "build/board_test.txt";
        const std::string fullFilePath = std::string(SUDOKU_CLI_WORKING_DIR) + "/" + outputFile;

        std::filesystem::remove( fullFilePath);

        WHEN( "The solver is run")
        {
            const auto exitCode = runner.run( { "-q", "-i", inputFile, "-o", outputFile, testCase.flag, testCase.format});

            THEN( "The result should be success")
            {
                CHECK( std::filesystem::exists( fullFilePath));
                CHECK( exitCode == 0);
            }
        }
    }
}