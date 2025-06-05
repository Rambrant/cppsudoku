//
// Created by Thomas Rambrant on 2025-06-03.
//

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/detail/input/parser.hpp>

#include "SudokuTraits.hpp"

using namespace com::rambrant::sudoku;

#include "CliRunner.hpp"

SCENARIO( "Sudoku help option [acceptance]")
{
    CliRunner runner( SUDOKU_CLI_COMMAND);

    GIVEN( "Help output with short flag")
    {
         WHEN( "The solver is run")
        {
            const auto exitCode = runner.run( { "-h"});

            THEN( "The output should be the help text")
            {
                CHECK( runner.outputContains( "Usage:"));
                CHECK( exitCode == 0);
            }
        }
    }

    GIVEN( "Help output with long flag")
    {
        WHEN( "The solver is run")
        {
            const auto exitCode = runner.run( { "--help"});

            THEN( "The output should be the help text")
            {
                CHECK( runner.outputContains( "Usage:"));
                CHECK( exitCode == 0);
            }
        }
    }
}
