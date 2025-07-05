//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <catch2/catch_test_macros.hpp>

#include "ConstSqrt.hpp"

using namespace com::rambrant::sudoku;

TEST_CASE( "ConstSqrt: computes integer square roots correctly", "[unit]")
{
    SECTION( "Perfect integer square roots")
    {
        STATIC_REQUIRE( ConstSqrt( 1)    == 1);
        STATIC_REQUIRE( ConstSqrt( 4)    == 2);
        STATIC_REQUIRE( ConstSqrt( 9)    == 3);
        STATIC_REQUIRE( ConstSqrt( 16)   == 4);
        STATIC_REQUIRE( ConstSqrt( 25)   == 5);
        STATIC_REQUIRE( ConstSqrt( 36)   == 6);
        STATIC_REQUIRE( ConstSqrt( 49)   == 7);
        STATIC_REQUIRE( ConstSqrt( 64)   == 8);
        STATIC_REQUIRE( ConstSqrt( 81)   == 9);
        STATIC_REQUIRE( ConstSqrt( 100)  == 10);
        STATIC_REQUIRE( ConstSqrt( 256)  == 16);
        STATIC_REQUIRE( ConstSqrt( 1024) == 32);
        STATIC_REQUIRE( ConstSqrt( 4096) == 64);
    }

    SECTION( "It returns floor(sqrt(n)) for non-perfect squares")
    {
        REQUIRE( ConstSqrt( 2)   == 1);
        REQUIRE( ConstSqrt( 3)   == 1);
        REQUIRE( ConstSqrt( 5)   == 2);
        REQUIRE( ConstSqrt( 10)  == 3);
        REQUIRE( ConstSqrt( 26)  == 5);
        REQUIRE( ConstSqrt( 63)  == 7);
        REQUIRE( ConstSqrt( 80)  == 8);
        REQUIRE( ConstSqrt( 999) == 31);
    }

    SECTION( "Edge cases")
    {
        REQUIRE( ConstSqrt( 0) == 0);
        REQUIRE( ConstSqrt( 1) == 1);
    }
}