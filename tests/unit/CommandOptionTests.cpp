//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <catch2/catch_all.hpp>

#include "CommandOption.hpp"

using namespace com::rambrant::sudoku;

TEST_CASE( "CommandOption: flag matching", "[unit]")
{
    const StringOption opt( "--name", "-n");

    CHECK( opt.isMatched( "--name"));
    CHECK( opt.isMatched( "-n"));
    CHECK_FALSE( opt.isMatched( "--other"));
}

TEST_CASE( "CommandOption: value conversion", "[unit]")
{
    SECTION( "StringOption")
    {
        StringOption opt( "--name", "-n");

        opt.convertValue( "hello");
        REQUIRE( opt.get() == "hello");
    }

    SECTION( "BoolOption")
    {
        BoolOption opt( "--flag", "-f");

        opt.convertValue( "");  // No value needed but triggers conversion
        REQUIRE( opt.get());
    }

    SECTION( "ListOption")
    {
        ListOption opt( "--items", "-i");

        opt.convertValue( "a,b,c");
        REQUIRE( opt.get() == std::vector<std::string>{ "a", "b", "c"});
    }
}

TEST_CASE( "CommandOption: default handling", "[unit]")
{
    SECTION( "Returns explicit value over default")
    {
        StringOption opt( "--out", "-o", "default.txt");

        opt.convertValue( "set.txt");
        REQUIRE( opt.get() == "set.txt");
    }

    SECTION( "Returns default value if no value set")
    {
        StringOption opt( "--out", "-o", "default.txt");

        REQUIRE( opt.get() == "default.txt");
    }

    SECTION( "Throws if neither value nor default exists")
    {
        StringOption opt( "--missing", "-m");

        REQUIRE_THROWS_AS( opt.get(), std::runtime_error);
    }
}

TEST_CASE( "CommandOption: validator checks", "[unit]")
{
    StringOption opt( "--name", "-n");

    opt.setValidator( [](const auto& o)
    {
        return o.isSet() && o.get() == "valid";
    });

    SECTION( "Valid value passes validator")
    {
        opt.convertValue( "valid");
        REQUIRE( opt.isValid());
    }

    SECTION( "Invalid value fails validator")
    {
        opt.convertValue( "invalid");
        REQUIRE_FALSE( opt.isValid());
    }
}

TEST_CASE( "CommandOption: expectsValue logic", "[unit]")
{
    REQUIRE_FALSE( BoolOption( "--flag", "-f").expectsValue());
    REQUIRE( StringOption( "--name", "-n").expectsValue());
    REQUIRE( ListOption( "--list", "-l").expectsValue());
}