//
// Created by Thomas Rambrant on 2025-06-18.
//
#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include "CommandOption.hpp"
#include "CommandLineParser.hpp"
#include "CommandValidators.hpp"

using namespace com::rambrant::sudoku;

TEST_CASE( "CommandLineParser: parse single options correctly")
{
    StringOption mode( "--mode", "-m", "hard");
    CommandLineParser parser( mode);

    const char* argv[] = { "program", "--mode", "easy" };
    int         argc   = std::size( argv);

    CHECK( parser.parse(argc, const_cast<char**>(argv)));
    CHECK(mode.get() == "easy");
}

TEST_CASE( "CommandLineParse: rejects invalid value via validator")
{
    StringOption mode("--mode", "-m", "hard");
    mode.setValidator( ValuesIn({ "easy", "medium", "hard"}));
    CommandLineParser parser( mode);

    const char* argv[] = { "program", "--mode", "extreme" };
    int         argc   = std::size( argv);

    std::stringstream buffer;
    std::streambuf*   oldCerr = std::cerr.rdbuf( buffer.rdbuf());

    CHECK_FALSE( parser.parse( argc, const_cast<char**>(argv)));
    CHECK( buffer.str().find( "Invalid value 'extreme'") != std::string::npos);

    std::cerr.rdbuf( oldCerr);
}

TEST_CASE( "CommandLineParser: enforces NotWith validator")
{
    BoolOption verbose( "--verbose", "-v", "Enable verbose logging");
    BoolOption quiet( "--quiet", "-q", "Enable quiet mode");

    verbose.setValidator( NotWith( quiet));
    quiet.setValidator( NotWith( verbose));

    CommandLineParser parser( verbose, quiet);

    const char* argv[] = { "program", "--verbose", "--quiet" };
    int         argc   = std::size( argv);

    std::stringstream buffer;
    std::streambuf* oldCerr = std::cerr.rdbuf(buffer.rdbuf());

    CHECK_FALSE( parser.parse( argc, const_cast<char**>(argv)));
    CHECK( buffer.str().find("may not be used together") != std::string::npos);

    std::cerr.rdbuf( oldCerr);
}

TEST_CASE( "CommandLineParser: handle multiple types")
{
    StringOption mode( "--mode", "-m", "Select mode");
    mode.setValidator( ValuesIn({"easy", "hard"}));

    BoolOption debug( "--debug", "-d", "Enable debugging");
    ListOption tags( "--tags", "-t", {"Comma", "separated", "tags"});

    CommandLineParser parser( mode, debug, tags);

    const char* argv[] = { "program", "--mode", "hard", "--debug", "--tags", "a,b,c" };
    int         argc   = std::size( argv);

    CHECK( parser.parse(argc, const_cast<char**>(argv)));

    CHECK( mode.get() == "hard");
    CHECK( debug.get());
    CHECK( tags.get() == std::vector<std::string>{ "a", "b", "c"});
}