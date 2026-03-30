//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <catch2/catch_test_macros.hpp>

#include "Digit.hpp"

using namespace com::rambrant::sudoku;

TEST_CASE( "Digits: conversions to value for 9x9 board", "[unit]")
{
    constexpr int size = 9;

    SECTION( "Valid numeric digits")
    {
        for( char c = '1'; c <= '9'; ++c)
        {
            REQUIRE( digitToValue<size>( c).value() == c - '0');
        }
    }

    SECTION( "Dot as zero (No Value)")
    {
        REQUIRE( digitToValue<size>( '.').value() == 0);
    }

    SECTION( "Invalid characters")
    {
        CHECK_FALSE( digitToValue<size>( 'A').has_value());
        CHECK_FALSE( digitToValue<size>( 'z').has_value());
        CHECK_FALSE( digitToValue<size>( '!').has_value());
    }

    SECTION( "String overload")
    {
        REQUIRE( digitToValue<size>( std::string{"3"}).value()  == 3);
        REQUIRE( digitToValue<size>( std::string{"."}).value()  == 0);
        CHECK_FALSE( digitToValue<size>( std::string{"AB"}).has_value());
    }
}

TEST_CASE( "Digits: conversions to value for 16x16 board", "[unit]")
{
    constexpr int size = 16;

    SECTION( "Valid 'hex' digits")
    {
        REQUIRE( digitToValue<size>( '9').value() == 9);
        REQUIRE( digitToValue<size>( 'A').value() == 10);
        REQUIRE( digitToValue<size>( 'F').value() == 15);
        REQUIRE( digitToValue<size>( 'G').value() == 16); // out of bounds
    }

    SECTION( "Invalid characters")
    {
        CHECK_FALSE( digitToValue<size>( 'H').has_value());
        CHECK_FALSE( digitToValue<size>( 'z').has_value());
        CHECK_FALSE( digitToValue<size>( '!').has_value());
    }

    SECTION( "Case insensitive hex digits")
    {
        REQUIRE(digitToValue<size>('a').value() == 10);
        REQUIRE(digitToValue<size>('f').value() == 15);
    }
}

TEST_CASE( "Digits: conversion to Digit for 9x9 board", "[unit]")
{
    constexpr int size = 9;

    SECTION( "Valid values")
    {
        for( int i = 0; i <= size; ++i)
        {
            REQUIRE( valueToDigit<size>(i).value() == static_cast<char>( '0' + i));
        }
    }

    SECTION( "Out of range")
    {
        CHECK_FALSE( valueToDigit<size>( -1).has_value());
        CHECK_FALSE( valueToDigit<size>( 10).has_value());
    }
}

TEST_CASE( "Digits: conversion to Digit for 16x16 board", "[unit]")
{
    constexpr int size = 16;

    SECTION( "Numeric values")
    {
        for( int i = 0; i <= 9; ++i)
        {
            REQUIRE( valueToDigit<size>(i).value() == static_cast<char>( '0' + i));
        }
    }

    SECTION( "Alphabetic values")
    {
        REQUIRE( valueToDigit<size>( 10).value() == 'A');
        REQUIRE( valueToDigit<size>( 15).value() == 'F');
        REQUIRE( valueToDigit<size>( 16).value() == 'G');
    }

    SECTION( "Invalid values")
    {
        CHECK_FALSE( valueToDigit<size>( -1).has_value());
        CHECK_FALSE( valueToDigit<size>( 17).has_value());
    }
}
