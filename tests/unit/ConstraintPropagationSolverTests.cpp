//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <any>
#include <catch2/catch_all.hpp>

#include "BackTrackingSolver.hpp"
#include "ConstraintPropagationSolver.hpp"
#include "SudokuTraits.hpp"

using namespace com::rambrant::sudoku;

namespace
{
    constexpr SudokuTraits::Board simpleBoard
    {{
       { 0, 0, 3, 0, 2, 0, 6, 0, 0 },
       { 9, 0, 0, 3, 0, 5, 0, 0, 1 },
       { 0, 0, 1, 8, 0, 6, 4, 0, 0 },
       { 0, 0, 8, 1, 0, 2, 9, 0, 0 },
       { 7, 0, 0, 0, 0, 0, 0, 0, 8 },
       { 0, 0, 6, 7, 0, 8, 2, 0, 0 },
       { 0, 0, 2, 6, 0, 9, 5, 0, 0 },
       { 8, 0, 0, 2, 0, 3, 0, 0, 9 },
       { 0, 0, 5, 0, 1, 0, 3, 0, 0 }
    }};

    //
    // A board with a contradiction (duplicate 5's in top row)
    //
    constexpr SudokuTraits::Board invalidBoard
    {{
        std::array{ 5, 3, 5, 0, 7, 0, 0, 0, 0},
        std::array{ 6, 0, 0, 1, 9, 5, 0, 0, 0},
        std::array{ 0, 9, 8, 0, 0, 0, 0, 6, 0},
        std::array{ 8, 0, 0, 0, 6, 0, 0, 0, 3},
        std::array{ 4, 0, 0, 8, 0, 3, 0, 0, 1},
        std::array{ 7, 0, 0, 0, 2, 0, 0, 0, 6},
        std::array{ 0, 6, 0, 0, 0, 0, 2, 8, 0},
        std::array{ 0, 0, 0, 4, 1, 9, 0, 0, 5},
        std::array{ 0, 0, 0, 0, 8, 0, 0, 7, 9}
    }};

    //
    // An empty board (all zeroes)
    //
    constexpr SudokuTraits::Board emptyBoard
    {{
        std::array{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array{ 0, 0, 0, 0, 0, 0, 0, 0, 0}
    }};
}

TEST_CASE( "ConstraintPropagationSolver: BoardStructure", "[unit]")
{
    detail::BoardStructure boardStructure{};

    SECTION( "BoardStructure init squares, peers and units", "[unit]")
    {
        CHECK( boardStructure.mSquares.size() == SudokuTraits::BOARD_SIZE * SudokuTraits::BOARD_SIZE);
        CHECK( boardStructure.mPeers.size()   == boardStructure.mSquares.size());
        CHECK( boardStructure.mUnits.size()   == boardStructure.mSquares.size());
    }

    SECTION( "BoardStructure peers", "[unit]")
    {
        std::size_t peerNumber = SudokuTraits::BOARD_SIZE -1 +          // Peers in the row minus the start cell
                                 SudokuTraits::BOARD_SIZE -1 +          // Peers in the column minus the start cell
                                 SudokuTraits::BOARD_SIZE -1 - (2 + 2); // Peers in the box minus the start cell and the row and column;

        for( const auto& [_, peers] : boardStructure.mPeers)
        {
            CHECK( peers.size() == peerNumber ); // row, column and box
        }
    }

    SECTION( "BoardStructure units", "[unit]")
    {
        for( const auto& [_, units] : boardStructure.mUnits)
        {
            constexpr  std::size_t unitNumber = 3; // rows, columns and box

            CHECK( units.size() == unitNumber ); // row, column and box

            for( const auto& unit : units)
            {
                CHECK( unit.size() == SudokuTraits::BOARD_SIZE);
            }
        }
    }
}

TEST_CASE( "ConstraintPropagationSolver: parseGrid", "[unit]")
{
    SECTION( "Given a valid board", "[unit]")
    {
        detail::SquareValues values{ detail::parseGrid( simpleBoard)};

        CHECK( values.size() == SudokuTraits::BOARD_SIZE * SudokuTraits::BOARD_SIZE);

        //
        // Check that all initially set values in the given board are initialized correctly
        //
        for( int rowIdx : SudokuTraits::INDEX_RANGE)
        {
            for( int colIdx : SudokuTraits::INDEX_RANGE)
            {
                if( auto val = simpleBoard[ rowIdx][colIdx]; val != SudokuTraits::NO_VALUE)
                {
                    CHECK( values[ {rowIdx, colIdx}].size() == 1);
                    CHECK( values[ {rowIdx, colIdx}][0]     == val);
                }
            }
        }
    }

    SECTION( "Given an invalid board", "[unit]")
    {
        REQUIRE_THROWS_WITH( detail::parseGrid( invalidBoard), "Illegal board: contradiction at [[0,2]] for digit 5");
    }
}

TEST_CASE( "ConstraintPropagationSolver: assign", "[unit]")
{
    detail::BoardStructure        boardStructure{};
    detail::SquareValues          squareValues{ detail::parseGrid( emptyBoard)}; // Fill all values with [1-9]
    constexpr detail::Square      square{0,0};
    constexpr SudokuTraits::Value value{1};

    CHECK( detail::assign( squareValues, square, value));
    CHECK( squareValues[square] == std::vector<int>{1});

    //
    // Check the pears for the assigned square, the value should be gone from all other peers
    //
    for( auto peer : boardStructure.mPeers[square])
    {
        auto const& possibleValues = squareValues[ peer];

        CHECK( std::find( possibleValues.begin(), possibleValues.end(), value) == possibleValues.end());
    }

    //
    // Check another square that should be untouched
    //
    CHECK( squareValues[{8,8}] == std::vector<int>(SudokuTraits::VALUE_RANGE.begin(), SudokuTraits::VALUE_RANGE.end()));
}

TEST_CASE( "ConstraintPropagationSolver: eliminate", "[unit]")
{
    detail::BoardStructure   boardStructure{};
    detail::SquareValues     squareValues{ detail::parseGrid( emptyBoard)}; // Fill all values with [1-9]
    constexpr detail::Square s00{0,0};

    SECTION( "Basic elimination", "[unit]")
    {
        CHECK( detail::eliminate( squareValues, s00, 1));

        CHECK( squareValues [s00] == std::vector<int>{ 2, 3, 4, 5, 6, 7, 8 ,9} );

        for( auto peer : boardStructure.mPeers[s00])
        {
            CHECK( squareValues[ peer] == std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8 ,9} );
        }
    }

    SECTION( "Eliminate to force an assign", "[unit]")
    {
        //
        // Eliminate all but the value 2 to force an assign
        //
        for( auto value : std::vector{ 1, 3, 4, 5, 6, 7, 8 ,9} )
        {
            CHECK( detail::eliminate( squareValues, s00, value));
        }

        CHECK( squareValues [s00] == std::vector<int>{ 2} );

        //
        // Check all peers, the value 2 should be missing
        //
        for( auto peer : boardStructure.mPeers[s00])
        {
            CHECK( squareValues[ peer] == std::vector<int>{ 1, 3, 4, 5, 6, 7, 8 ,9} );
        }

        //
        // Elimination the last value, 2, to force a failure
        //
        CHECK_FALSE( detail::eliminate( squareValues, s00, 2));
    }
}
