//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <sstream>
#include <catch2/catch_test_macros.hpp>

#include "Logger.hpp"

using namespace com::rambrant::sudoku;

TEST_CASE( "Logger: streaming messages", "[unit]")
{
    SECTION( "Logger respects default log level")
    {
        std::ostringstream out;
        const Logger       logger( Logger::LogLevel::Normal, out);

        logger << Logger::normal << "Hello Normal" << std::endl;
        logger << Logger::verbose << "Hello Verbose" << std::endl;

        std::string result = out.str();

        CHECK( result.find( "Hello Normal") != std::string::npos);
        CHECK_FALSE( result.find( "Hello Verbose") != std::string::npos);
    }

    SECTION( "Logger with Verbose level shows all output")
    {
        std::ostringstream out;
        const Logger       logger( Logger::LogLevel::Verbose, out);

        logger << Logger::normal << "Normal output" << std::endl;
        logger << Logger::verbose << "Verbose output" << std::endl;

        std::string result = out.str();
        CHECK( result.find( "Normal output") != std::string::npos);
        CHECK( result.find( "Verbose output") != std::string::npos);
    }

    SECTION( "Logger with Quiet level shows no output")
    {
        std::ostringstream out;
        const Logger       logger( Logger::LogLevel::Quiet, out);

        logger << Logger::normal << "Should not appear" << std::endl;
        logger << Logger::verbose << "Also should not appear" << std::endl;

        REQUIRE( out.str().empty());
    }

    SECTION( "Logger resets level after std::endl")
    {
        std::ostringstream out;
        const Logger       logger( Logger::LogLevel::Normal, out);

        logger << Logger::verbose << "Verbose" << std::endl << "Normal" << std::endl;

        std::string result = out.str();

        CHECK_FALSE( result.find( "Verbose") != std::string::npos);
        CHECK( result.find( "Normal") != std::string::npos);
    }
}

TEST_CASE( "Logger: chaining supported", "[unit]")
{
    std::ostringstream out;
    const Logger       logger( Logger::LogLevel::Normal, out);

    logger << Logger::normal << "A: " << 123 << ", B: " << 4.5 << std::endl;

    std::string result = out.str();
    REQUIRE( result.find( "A: 123, B: 4.5") != std::string::npos);
}

TEST_CASE( "Logger: isVerbose works correctly", "[unit]")
{
    const Logger logger1( Logger::LogLevel::Quiet);
    const Logger logger2( Logger::LogLevel::Verbose);

    CHECK_FALSE( logger1.isVerbose());
    CHECK( logger2.isVerbose());
}

TEST_CASE( "Logger: Default values", "[unit]")
{
    std::ostringstream out;

    auto* orgBuff = std::clog.rdbuf();           // Save the original buffer of std::clog
    std::clog.rdbuf( out.rdbuf());               // Redirect std::clog to out

    const Logger logger{};
    logger << "Captured output!" << std::endl;

    std::clog.rdbuf( orgBuff);                   // Restore original std::clog buffer
    std::string captured = out.str();            // Now we can use the captured output

    CHECK( captured.find( "Captured output!") != std::string::npos);
}