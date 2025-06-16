//
// Created by Thomas Rambrant on 2025-06-15.
//
#include "RangeArray.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

using namespace com::rambrant::sudoku;

TEST_CASE( "RangeArray: generates correct sequences", "[unit]")
{
    SECTION( "Range of 5 with default int type")
    {
        constexpr auto range = RangeArray<5>( 0, 1);

        static_assert( range.size() == 5);

        CHECK( range[0] == 0);
        CHECK( range[1] == 1);
        CHECK( range[2] == 2);
        CHECK( range[3] == 3);
        CHECK( range[4] == 4);
    }

    SECTION( "Range with step 2")
    {
        constexpr auto range = RangeArray<4>( 1, 2);

        static_assert( range.size() == 4);

        CHECK( range == std::array{ 1, 3, 5, 7});
    }

    SECTION( "Negative steps")
    {
        constexpr auto range = RangeArray<3>( 10, -2);

        CHECK( range == std::array{ 10, 8, 6});
    }

    SECTION( "Character range")
    {
        constexpr auto range = RangeArray<3, char>( 'a', 2);

        CHECK( range[0] == 'a');
        CHECK( range[1] == 'c');
        CHECK( range[2] == 'e');
    }

    SECTION( "Custom type like double")
    {
        constexpr auto range = RangeArray<3, double>( 1.0, 0.5);

        CHECK( range[0] == Catch::Approx( 1.0));
        CHECK( range[1] == Catch::Approx( 1.5));
        CHECK( range[2] == Catch::Approx( 2.0));
    }
}