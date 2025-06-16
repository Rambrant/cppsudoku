//
// Created by Thomas Rambrant on 2025-06-03.
//
#include <list>
#include <catch2/catch_test_macros.hpp>

#include "SudokuTraits.hpp"

using namespace com::rambrant::sudoku;
using Traits = SudokuTraits;


TEST_CASE( "Traits: Board structure and values", "[unit]")
{
    constexpr Traits::BoardArray array{};
    constexpr Traits::Board      board{};

    SECTION( "BoardArray has correct size")
    {
        REQUIRE( array.size() == Traits::BOARD_SIZE);
    }

    SECTION(" Board has correct size")
    {
        REQUIRE( board.size()    == Traits::BOARD_SIZE);
        REQUIRE( board[0].size() == Traits::BOARD_SIZE);
    }

    SECTION( "Board is initialized to NO_VALUE")
    {
        for( const auto& row : board)
        {
            for( const int square : row)
            {
                REQUIRE( square == Traits::NO_VALUE);
            }
        }
    }
}

TEST_CASE( "Traits: Constants are consistent", "[unit]")
{
    SECTION( "BOX_SIZE squared equals BOARD_SIZE")
    {
        REQUIRE( Traits::BOX_SIZE * Traits::BOX_SIZE == Traits::BOARD_SIZE);
    }
}

TEST_CASE( "Traits: Index and value ranges", "[unit]")
{
    SECTION( "INDEX_RANGE contains values [0..BOARD_SIZE-1]")
    {
        std::vector<int> expected;

        expected.reserve( Traits::BOARD_SIZE);

        for( Traits::Value value = 0; value < Traits::BOARD_SIZE; ++value)
        {
            expected.push_back(value);
        }

        REQUIRE( std::equal(expected.begin(), expected.end(), Traits::INDEX_RANGE.begin()));
    }

    SECTION( "VALUE_RANGE contains values [1..MAX_VALUE]")
    {
        std::vector<int> expected;
        expected.reserve( Traits::MAX_VALUE);

        for(Traits::Value value = 1; value <= Traits::MAX_VALUE; ++value)
        {
            expected.push_back( value);
        }

        REQUIRE( std::equal( expected.begin(), expected.end(), Traits::VALUE_RANGE.begin()));
    }
}

TEST_CASE( "Traits: Is Random Access Container trait", "[unit]")
{
    static_assert( is_random_access_container<std::vector<int>>);
    static_assert( is_random_access_container<std::array<int, 9>>);
    static_assert( ! is_random_access_container<std::list<int>>);
}
