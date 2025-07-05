//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <catch2/catch_all.hpp>

#include "CallTime.hpp"

using namespace com::rambrant::sudoku;

TEST_CASE( "CallTime: returns result and duration", "[unit]")
{
    SECTION( "Returns correct result for a simple function")
    {
        auto [result, duration] = CallTime([] { return 42; });

        REQUIRE( result == 42);
        REQUIRE( duration.count() >= 0);  // Should never be negative
    }

    SECTION( "Works with functions taking arguments")
    {
        auto multiply = [](int a, int b) { return a * b; };

        auto [result, duration] = CallTime( multiply, 6, 7);

        REQUIRE( result == 42);
        REQUIRE( duration.count() >= 0);
    }

    SECTION( "Supports functions returning void")
    {
        bool flag = false;

        auto [_, duration] = CallTime([&] { flag = true; });

        REQUIRE( flag);
        REQUIRE( duration.count() >= 0);
    }
}