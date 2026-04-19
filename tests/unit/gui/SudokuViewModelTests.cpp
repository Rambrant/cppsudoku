//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <catch2/catch_test_macros.hpp>

#include "core/SudokuTraits.hpp"
#include "viewmodel/SudokuViewModel.hpp"
#include "viewmodel/ClearCommand.hpp"
#include "viewmodel/SolveCommand.hpp"

using namespace com::rambrant::sudoku;
using namespace com::rambrant::sudoku::gui;

using Traits = SudokuTraits;

//
// Helpers
//
static Traits::Board makeEmptyBoard()
{
    Traits::Board board{};

    for( auto& row : board)
    {
        row.fill( 0);
    }

    return board;
}

//
// A known-solvable puzzle (Peter Norvig's "hard" example)
//
static Traits::Board makeHardPuzzle()
{
    // clang-format off
    return {{ {8,0,0,  0,0,0,  0,0,0},
              {0,0,3,  6,0,0,  0,0,0},
              {0,7,0,  0,9,0,  2,0,0},

              {0,5,0,  0,0,7,  0,0,0},
              {0,0,0,  0,4,5,  7,0,0},
              {0,0,0,  1,0,0,  0,3,0},

              {0,0,1,  0,0,0,  0,6,8},
              {0,0,8,  5,0,0,  0,1,0},
              {0,9,0,  0,0,0,  4,0,0} }};
    // clang-format on
}

TEST_CASE( "SudokuViewModel — initial state is all zeros", "[viewmodel]")
{
    SudokuViewModel model;

    for( int rowIdx = 0; rowIdx < Traits::BOARD_SIZE; ++rowIdx)
    {
        for( int colIdx = 0; colIdx < Traits::BOARD_SIZE; ++colIdx)
        {
            REQUIRE( model.getValue( rowIdx, colIdx) == 0);
        }
    }
}

TEST_CASE( "SudokuViewModel — initial state is Idle", "[viewmodel]")
{
    SudokuViewModel model;

    REQUIRE( model.getState() == SudokuViewModel::State::Idle);
    REQUIRE( model.getErrorMessage().empty());
}

TEST_CASE( "SudokuViewModel — setValue round-trips correctly", "[viewmodel]")
{
    SudokuViewModel model;

    model.setValue( 0, 0, 5);
    model.setValue( 4, 4, 9);
    model.setValue( 8, 8, 3);

    REQUIRE( model.getValue( 0, 0) == 5);
    REQUIRE( model.getValue( 4, 4) == 9);
    REQUIRE( model.getValue( 8, 8) == 3);
}

TEST_CASE( "SudokuViewModel — setValue resets state to Idle", "[viewmodel]")
{
    SudokuViewModel model;

    model.setState( SudokuViewModel::State::Solved);

    model.setValue( 0, 0, 1);

    REQUIRE( model.getState() == SudokuViewModel::State::Idle);
    REQUIRE( model.getErrorMessage().empty());
}

TEST_CASE( "SudokuViewModel — setBoard round-trips correctly", "[viewmodel]")
{
    SudokuViewModel model;
    auto            puzzle = makeHardPuzzle();

    model.setBoard( puzzle);

    REQUIRE( model.getBoard() == puzzle);
}

TEST_CASE( "ClearCommand — zeros all cells", "[viewmodel][command]")
{
    SudokuViewModel model;

    model.setBoard( makeHardPuzzle());
    model.setState( SudokuViewModel::State::Solved);

    ClearCommand cmd;

    model.execute( cmd);

    for( int rowIdx = 0; rowIdx < Traits::BOARD_SIZE; ++rowIdx)
    {
        for( int colIdx = 0; colIdx < Traits::BOARD_SIZE; ++colIdx)
        {
            REQUIRE( model.getValue( rowIdx, colIdx) == 0);
        }
    }
}

TEST_CASE( "ClearCommand — resets state to Idle", "[viewmodel][command]")
{
    SudokuViewModel model;

    model.setState( SudokuViewModel::State::Failed);
    model.setErrorMessage( "some error");

    ClearCommand cmd;

    model.execute( cmd);

    REQUIRE( model.getState() == SudokuViewModel::State::Idle);
    REQUIRE( model.getErrorMessage().empty());
}

TEST_CASE( "SolveCommand — solves a known puzzle", "[viewmodel][command]")
{
    SudokuViewModel model;

    model.setBoard( makeHardPuzzle());

    SolveCommand cmd;

    model.execute( cmd);

    REQUIRE( model.getState() == SudokuViewModel::State::Solved);

    //
    // Every cell must be filled (1-9)
    //
    for( int rowIdx = 0; rowIdx < Traits::BOARD_SIZE; ++rowIdx)
    {
        for( int colIdx = 0; colIdx < Traits::BOARD_SIZE; ++colIdx)
        {
            REQUIRE( model.getValue( rowIdx, colIdx) > 0);
        }
    }
}

TEST_CASE( "SolveCommand — fails gracefully on an unsolvable board", "[viewmodel][command]")
{
    SudokuViewModel model;

    //
    // Two 1s in the same row — immediately unsolvable
    //
    Traits::Board bad = makeEmptyBoard();

    bad[0][0] = 1;
    bad[0][1] = 1;

    model.setBoard( bad);

    SolveCommand cmd;
    model.execute( cmd);

    REQUIRE( model.getState() == SudokuViewModel::State::Failed);
    REQUIRE( ! model.getErrorMessage().empty());
}