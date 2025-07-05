//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <catch2/catch_test_macros.hpp>
#include <sstream>

#include "CommandOption.hpp"
#include "CommandValidators.hpp"

using namespace com::rambrant::sudoku;

TEST_CASE( "CommandValidator: ValuesIn validator works for single string option")
{
    StringOption mode( "--mode", "-m");
    mode.setValidator( ValuesIn({ "easy", "medium", "hard" }));

    SECTION( "Accepts allowed value")
    {
        mode.convertValue( "easy");
        CHECK( mode.isValid());
    }

    SECTION( "Rejects disallowed value")
    {
        mode.convertValue( "expert");

        std::stringstream buffer;
        std::streambuf* orgBuff = std::cerr.rdbuf( buffer.rdbuf());

        CHECK_FALSE( mode.isValid());
        CHECK( buffer.str().find( "Invalid value 'expert'") != std::string::npos);

        std::cerr.rdbuf( orgBuff); // Restore std::cerr
    }

    SECTION( "Unset value is still valid")
    {
        CHECK( mode.isValid());
    }
}

TEST_CASE( "CommandValidator: ValuesIn validator works for string list option")
{
    ListOption tags("--tags", "-t");
    tags.setValidator( ValuesIn({ "one", "two", "three" }));

    SECTION( "All values allowed")
    {
        tags.convertValue( "one,two");

        CHECK(tags.isValid());
    }

    SECTION( "Some values not allowed")
    {
        tags.convertValue( "one,four");

        std::stringstream buffer;
        std::streambuf* orgBuff = std::cerr.rdbuf( buffer.rdbuf());

        CHECK_FALSE( tags.isValid());
        CHECK( buffer.str().find("Invalid value 'four'") != std::string::npos);

        std::cerr.rdbuf(orgBuff);
    }
}

TEST_CASE( "CommandValidator: NotWith validator ensures exclusivity between options")
{
    BoolOption verbose( "--verbose", "-v");
    BoolOption quiet( "--quiet", "-q");

    verbose.setValidator( NotWith( quiet));
    quiet.setValidator( NotWith( verbose));

    SECTION( "Only one set - valid")
    {
        verbose.convertValue("");   // Needs to set the flag even if no value needs to be given

        CHECK( verbose.isValid());
        CHECK( quiet.isValid());
    }

    SECTION( "Both set - invalid")
    {
        verbose.convertValue( "");  // Needs to set the flag even if no value needs to be give
        quiet.convertValue( "");    // Needs to set the flag even if no value needs to be give

        std::stringstream buffer;
        std::streambuf* orgBuff = std::cerr.rdbuf( buffer.rdbuf());

        CHECK_FALSE( verbose.isValid());
        CHECK_FALSE( quiet.isValid());
        CHECK( buffer.str().find( "may not be used together") != std::string::npos);

        std::cerr.rdbuf( orgBuff);
    }
}