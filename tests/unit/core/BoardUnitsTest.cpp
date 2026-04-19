//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <catch2/catch_test_macros.hpp>

#include "core/BoardUnits.hpp"
#include "core/SudokuTraits.hpp"

using namespace com::rambrant::sudoku;
using Traits = SudokuTraits;

//
// Helper — board with cell (rowIdx,colIdx) = rowIdx * 9 + colIdx + 1  so every value is unique
// and easy to reason about
//
static Traits::Board makeSequentialBoard()
{
    Traits::Board board{};

    for( int rowIdx = 0; rowIdx < Traits::BOARD_SIZE; ++rowIdx)
    {
        for( int colIdx = 0; colIdx < Traits::BOARD_SIZE; ++colIdx)
        {
            board[rowIdx][colIdx] = colIdx * Traits::BOARD_SIZE + colIdx + 1;
        }
    }

    return board;
}

TEST_CASE( "extractColumn — returns correct values for column 0", "[units]")
{
    auto board = makeSequentialBoard();
    auto col   = extractColumn( board, 0);

    for( int rowIdx = 0; rowIdx < Traits::BOARD_SIZE; ++rowIdx)
    {
        REQUIRE( col[rowIdx] == board[rowIdx][0]);
    }
}

TEST_CASE( "extractColumn — returns correct values for column 8", "[units]")
{
    auto board = makeSequentialBoard();
    auto col   = extractColumn( board, 8);

    for( int rowIdx = 0; rowIdx < Traits::BOARD_SIZE; ++rowIdx)
    {
        REQUIRE( col[rowIdx] == board[rowIdx][8]);
    }
}

TEST_CASE( "extractBox — top-left box contains correct values", "[units]")
{
    auto board = makeSequentialBoard();
    auto box   = extractBox( board, 0, 0);

    //
    // Expected: rows 0-2, cols 0-2 in row-major order
    //
    Traits::BoardArray  expected{};
    int                 boardIdx = 0;

    for( int rowIdx = 0; rowIdx < 3; ++rowIdx)
    {
        for( int colIdx = 0; colIdx < 3; ++colIdx)
        {
            expected[ boardIdx++] = board[rowIdx][colIdx];
        }
    }

    REQUIRE( box == expected);
}

TEST_CASE( "extractBox — bottom-right box contains correct values", "[units]")
{
    auto board = makeSequentialBoard();
    auto box   = extractBox( board, 8, 8);

    Traits::BoardArray  expected{};
    int                 boardIdx = 0;

    for( int rowIdx = 6; rowIdx < 9; ++rowIdx)
    {
        for( int colIdx = 6; colIdx < 9; ++colIdx)
        {
            expected[ boardIdx++] = board[rowIdx][colIdx];
        }
    }

    REQUIRE( box == expected);
}

TEST_CASE( "extractBox — any cell inside the box gives the same result", "[units]")
{
    auto board = makeSequentialBoard();

    //
    // All four corners of the centre box should give the same extraction
    //
    auto box00 = extractBox( board, 3, 3);
    auto box02 = extractBox( board, 3, 5);
    auto box20 = extractBox( board, 5, 3);
    auto box22 = extractBox( board, 5, 5);

    REQUIRE( box00 == box02);
    REQUIRE( box00 == box20);
    REQUIRE( box00 == box22);
}
