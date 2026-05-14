//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <sstream>
#include <catch2/catch_test_macros.hpp>

#include "core/Logger.hpp"
#include "writers/BlockWriter.hpp"
#include "writers/IWriter.hpp"
#include "writers/JsonWriter.hpp"
#include "writers/LineWriter.hpp"
#include "writers/PrettyWriter.hpp"
#include "writers/WriterFactory.hpp"

using namespace com::rambrant::sudoku;

//
//  Concept
//
TEST_CASE( "WriterPlugin: concept is satisfied by concrete writers", "[unit]")
{
    STATIC_REQUIRE(  WriterPlugin<BlockWriter>);
    STATIC_REQUIRE(  WriterPlugin<JsonWriter>);
    STATIC_REQUIRE(  WriterPlugin<LineWriter>);
    STATIC_REQUIRE(  WriterPlugin<PrettyWriter>);
    STATIC_REQUIRE_FALSE( WriterPlugin<IWriter>);   // abstract base — no formatName
}

//
//  WriterFactory
//
TEST_CASE( "WriterFactory: formats() lists all registered writer keys", "[unit]")
{
    const auto formats = WriterFactory::instance().formats();

    // std::flat_map sorts by key, so the order is deterministic
    REQUIRE( formats == std::vector<std::string>{ "block", "json", "line", "pretty"});
}

TEST_CASE( "WriterFactory: create() returns a writer for every registered format", "[unit]")
{
    Logger logger{};

    for( const auto& format : WriterFactory::instance().formats())
    {
        std::ostringstream stream;

        CAPTURE( format);
        const auto result = WriterFactory::instance().create( format, stream, logger);

        REQUIRE( result.has_value());
    }
}

TEST_CASE( "WriterFactory: create() returns an error for an unknown format", "[unit]")
{
    std::ostringstream stream;
    Logger             logger{};

    const auto result = WriterFactory::instance().create( "xml", stream, logger);

    REQUIRE_FALSE( result.has_value());
    CHECK( result.error().find( "xml") != std::string::npos);
}