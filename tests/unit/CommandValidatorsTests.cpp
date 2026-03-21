//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "CommandOption.hpp"
#include "CommandValidators.hpp"

using namespace com::rambrant::sudoku;

TEST_CASE( "CommandValidator: ValuesIn validator works for single string option")
{
    SECTION( "Accepts allowed value")
    {
        StringOption mode( "--mode", "-m");

        mode.setValidator( ValuesIn({ "easy", "medium", "hard" }));
        mode.convertValue( "easy");

        CHECK( mode.isValid());
    }

    SECTION( "Rejects disallowed value and reports it")
    {
        std::stringstream out;
        StringOption      mode( "--mode", "-m");

        mode.setValidator( ValuesIn({ "easy", "medium", "hard" }, out));
        mode.convertValue( "expert");

        CHECK_FALSE( mode.isValid());
        CHECK( out.str().find( "Invalid value 'expert'") != std::string::npos);
    }

    SECTION( "Unset value is still valid")
    {
        StringOption mode( "--mode", "-m");

        mode.setValidator( ValuesIn({ "easy", "medium", "hard" }));

        CHECK( mode.isValid());
    }
}

TEST_CASE( "CommandValidator: ValuesIn validator works for string list option")
{
    SECTION( "All values allowed")
    {
        ListOption tags( "--tags", "-t");

        tags.setValidator( ValuesIn({ "one", "two", "three" }));
        tags.convertValue( "one,two");

        CHECK( tags.isValid());
    }

    SECTION( "Some values not allowed — reports the offending value")
    {
        std::stringstream out;
        ListOption        tags( "--tags", "-t");

        tags.setValidator( ValuesIn({ "one", "two", "three" }, out));
        tags.convertValue( "one,four");

        CHECK_FALSE( tags.isValid());
        CHECK( out.str().find( "Invalid value 'four'") != std::string::npos);
    }
}

TEST_CASE( "CommandValidator: NotWith validator ensures exclusivity between options")
{
    SECTION( "Only one set — valid")
    {
        BoolOption verbose( "--verbose", "-v");
        BoolOption quiet( "--quiet", "-q");

        verbose.setValidator( NotWith( quiet));
        verbose.convertValue( "");

        CHECK( verbose.isValid());
        CHECK( quiet.isValid());
    }

    SECTION( "Both set — invalid, reports conflict")
    {
        std::stringstream out;
        BoolOption        verbose( "--verbose", "-v");
        BoolOption        quiet( "--quiet", "-q");

        verbose.setValidator( NotWith( quiet, out));
        verbose.convertValue( "");
        quiet.convertValue( "");

        CHECK_FALSE( verbose.isValid());
        CHECK( out.str().find( "may not be used together") != std::string::npos);
    }
}