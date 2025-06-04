//
// Created by Thomas Rambrant on 2025-06-03.
//

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/detail/input/parser.hpp>

#include "SudokuTraits.hpp"

using namespace com::rambrant::sudoku;
using Traits = SudokuTraits;

#include "CliRunner.hpp"


TEST_CASE( "CLI tool returns help", "[acceptance]")
{
    CliRunner runner(SUDOKU_CLI_PATH);

    const int exitCode = runner.run({ "--help" });

    REQUIRE( exitCode == 0);
    REQUIRE( runner.outputContains("Usage"));
}