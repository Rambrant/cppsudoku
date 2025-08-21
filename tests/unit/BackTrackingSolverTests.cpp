//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <any>
#include <catch2/catch_all.hpp>

#include "BackTrackingSolver.hpp"
#include "ConstraintPropagationSolver.hpp"
#include "Logger.hpp"
#include "SudokuTraits.hpp"

using namespace com::rambrant::sudoku;

namespace
{
    constexpr SudokuTraits::Board board
    {{
        { 5, 6, 0 ,8 ,4 ,7 ,0 ,0 ,0},
        { 6, 0, 0, 1, 9, 5, 0, 0, 0},
        { 0, 9, 8, 0, 0, 0, 0, 6, 0},
        { 8, 0, 0, 0, 6, 0, 0, 0, 3},
        { 4, 0, 0, 8, 0, 3, 0, 0, 1},
        { 7, 0, 0, 0, 2, 0, 0, 0, 6},
        { 0, 6, 0, 0, 0, 0, 2, 8, 0},
        { 0, 0, 0, 4, 1, 9, 0, 0, 5},
        { 0, 0, 0, 0, 8, 0, 0, 7, 9}
    }};

    auto filterUnused( std::vector<int> used)
    {
        std::vector<int> unused;
        std::copy_if( SudokuTraits::VALUE_RANGE.begin(), SudokuTraits::VALUE_RANGE.end(),
             std::back_inserter( unused),
             [&used]( const int n) { return std::find( used.begin(), used.end(), n) == used.end(); }
             );

        return unused;
    }
}

TEST_CASE( "BackTrackingSolver: checkValue", "[unit]")
{
    Traits::BoardArray boardUnit{ 0,0,1,2,3,6,7,8,0};

    SECTION( "Given value exists", "[unit]")
    {
        constexpr Traits::Value value{2};

        const auto result = detail::checkValue( value, boardUnit);

        REQUIRE( result == false);
    }

    SECTION( "Given value don't exist", "[unit]")
    {
        constexpr Traits::Value value{4};

        const auto result = detail::checkValue( value, boardUnit);

        REQUIRE( result == true);
    }
}

TEST_CASE( "BackTrackingSolver: rowConstraint", "[unit]")
{
    constexpr int rowPos{0};

    const std::vector used{ 5, 6, 8 ,4 ,7};
    const std::vector unused = filterUnused( used);

    SECTION( "Given value exists", "[unit]")
    {
        for( const auto value : used)
        {
            const auto result = detail::rowConstraint( board, value, rowPos);

            REQUIRE( result == false);
        }
    }

    SECTION( "Given value don't exist", "[unit]")
    {
        for( const auto value : unused)
        {
            const auto result = detail::rowConstraint( board, value, rowPos);

            REQUIRE( result == true);
        }
    }
}

TEST_CASE( "BackTrackingSolver: columnConstraint", "[unit]")
{
    constexpr int colPos{4};

    const std::vector used{ 4, 9, 6, 2, 1, 8};
    const std::vector unused = filterUnused( used);

    SECTION( "Given value exists", "[unit]")
    {
        for( const auto value : used)
        {
            const auto result = detail::columnConstraint( board, value, colPos);

            REQUIRE( result == false);
        }
    }

    SECTION( "Given value don't exist", "[unit]")
    {
        for( const auto value : unused)
        {
            const auto result = detail::columnConstraint( board, value, colPos);

            REQUIRE( result == true);
        }
    }
}

TEST_CASE( "BackTrackingSolver: boxConstraint", "[unit]")
{
    constexpr int colPos{5};
    constexpr int rowPos{4};

    const std::vector used{ 0, 6, 0, 8, 0, 3, 0, 2, 0};
    const std::vector unused = filterUnused( used);

    SECTION( "Given value exists", "[unit]")
    {
        for( const auto value : used)
        {
            const auto result = detail::boxConstraint( board, value, rowPos, colPos);

            REQUIRE( result == false);
        }
    }

    SECTION( "Given value don't exists", "[unit]")
    {
        for( const auto value : unused)
        {
            const auto result = detail::boxConstraint( board, value, rowPos, colPos);

            REQUIRE( result == true);
        }
    }
}
