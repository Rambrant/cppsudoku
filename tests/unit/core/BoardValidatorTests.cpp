//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <catch2/catch_test_macros.hpp>

#include "core/BoardValidator.hpp"
#include "core/SudokuTraits.hpp"

using namespace com::rambrant::sudoku;

//
// Helpers
//

static SudokuTraits::Board makeEmptyBoard()
{
    SudokuTraits::Board board{};
    for( auto& row : board)
    {
        row.fill( 0);
    }

    return board;
}

static SudokuTraits::Board makeValidPartialBoard()
{
    //
    // Peter Norvig's hard puzzle — valid but unsolved
    //
    return {{ {8,0,0,  0,0,0,  0,0,0},
              {0,0,3,  6,0,0,  0,0,0},
              {0,7,0,  0,9,0,  2,0,0},
              {0,5,0,  0,0,7,  0,0,0},
              {0,0,0,  0,4,5,  7,0,0},
              {0,0,0,  1,0,0,  0,3,0},
              {0,0,1,  0,0,0,  0,6,8},
              {0,0,8,  5,0,0,  0,1,0},
              {0,9,0,  0,0,0,  4,0,0} }};
}

TEST_CASE( "BoardValidator — empty board is valid", "[Unit]")
{
    REQUIRE( isValidBoard( makeEmptyBoard()));
}

TEST_CASE( "BoardValidator — valid partial board is valid", "[Unit]")
{
    REQUIRE( isValidBoard( makeValidPartialBoard()));
}

TEST_CASE( "BoardValidator — duplicate in row is invalid", "[Unit]")
{
    auto board   = makeEmptyBoard();
    board[0][0]  = 1;
    board[0][1]  = 1;

    REQUIRE_FALSE( isValidBoard( board));
}

TEST_CASE( "BoardValidator — duplicate in last row is invalid", "[Unit]")
{
    auto board   = makeEmptyBoard();
    board[8][3]  = 7;
    board[8][7]  = 7;

    REQUIRE_FALSE( isValidBoard( board));
}

TEST_CASE( "BoardValidator — duplicate in column is invalid", "[Unit]")
{
    auto board   = makeEmptyBoard();
    board[0][4]  = 5;
    board[3][4]  = 5;

    REQUIRE_FALSE( isValidBoard( board));
}

TEST_CASE( "BoardValidator — duplicate in top-left box is invalid", "[Unit]")
{
    auto board   = makeEmptyBoard();
    board[0][0]  = 3;
    board[2][2]  = 3;

    REQUIRE_FALSE( isValidBoard( board));
}

TEST_CASE( "BoardValidator — duplicate in bottom-right box is invalid", "[Unit]")
{
    auto board   = makeEmptyBoard();
    board[6][6]  = 9;
    board[8][8]  = 9;

    REQUIRE_FALSE( isValidBoard( board));
}

TEST_CASE( "BoardValidator — multiple zeros in same unit are valid", "[Unit]")
{
    auto board  = makeEmptyBoard();

    //
    // Leave an entire row at zero — must still be valid
    //
    REQUIRE( isValidBoard( board));
}