//
// Created by Thomas Rambrant on 2025-06-17.
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
            REQUIRE( digitToValue<size>( c) == c - '0');
        }
    }

    SECTION( "Dot as zero (No Value)")
    {
        REQUIRE( digitToValue<size>( '.') == 0);
    }

    SECTION( "Invalid characters")
    {
        REQUIRE( digitToValue<size>( 'A') == -1);
        REQUIRE( digitToValue<size>( 'z') == -1);
        REQUIRE( digitToValue<size>( '!') == -1);
    }

    SECTION( "String overload")
    {
        REQUIRE( digitToValue<size>( std::string{"3"})  == 3);
        REQUIRE( digitToValue<size>( std::string{"."})  == 0);
        REQUIRE( digitToValue<size>( std::string{"AB"}) == -1);
    }
}

TEST_CASE( "Digits: conversions to value for 16x16 board", "[unit]")
{
    constexpr int size = 16;

    SECTION( "Valid 'hex' digits")
    {
        REQUIRE( digitToValue<size>( '9') == 9);
        REQUIRE( digitToValue<size>( 'A') == 10);
        REQUIRE( digitToValue<size>( 'F') == 15);
        REQUIRE( digitToValue<size>( 'G') == 16); // out of bounds
    }

    SECTION( "Invalid characters")
    {
        REQUIRE( digitToValue<size>( 'H') == -1);
        REQUIRE( digitToValue<size>( 'z') == -1);
        REQUIRE( digitToValue<size>( '!') == -1);
    }

    SECTION( "Case insensitive hex digits")
    {
        REQUIRE(digitToValue<size>('a') == 10);
        REQUIRE(digitToValue<size>('f') == 15);
    }
}

TEST_CASE( "Digits: conversion to Digit for 9x9 board", "[unit]")
{
    constexpr int size = 9;

    SECTION( "Valid values")
    {
        for( int i = 0; i <= size; ++i)
        {
            REQUIRE( valueToDigit<size>(i) == static_cast<char>( '0' + i));
        }
    }

    SECTION( "Out of range")
    {
        REQUIRE( valueToDigit<size>( -1) == '?');
        REQUIRE( valueToDigit<size>( 10) == '?');
    }
}

TEST_CASE( "Digits: conversion to Digit for 16x16 board", "[unit]")
{
    constexpr int size = 16;

    SECTION( "Numeric values")
    {
        for( int i = 0; i <= 9; ++i)
        {
            REQUIRE( valueToDigit<size>(i) == static_cast<char>( '0' + i));
        }
    }

    SECTION( "Alphabetic values")
    {
        REQUIRE( valueToDigit<size>( 10) == 'A');
        REQUIRE( valueToDigit<size>( 15) == 'F');
        REQUIRE( valueToDigit<size>( 16) == 'G');
    }

    SECTION( "Invalid values")
    {
        REQUIRE( valueToDigit<size>( -1) == '?');
        REQUIRE( valueToDigit<size>( 17) == '?');
    }
}
